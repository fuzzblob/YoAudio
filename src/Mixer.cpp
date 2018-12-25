#include "Mixer.h"

#include "AudioThread.h"
#include "Log.h"

Mixer::Mixer()
{
	mDevice = std::make_unique<AudioDevice>();
	SDL_AudioSpec want;
	SDL_AudioSpec get;
	SDL_memset(&want, 0, sizeof(want));

	want.freq = TARGET_SAMPLERATE;
	want.format = AUDIO_FORMAT;
	want.channels = TARGET_CHANNELS;
	want.samples = TARGET_BUFFER;
	want.callback = AudioCallback;
	want.userdata = this;

	mDevice->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &want, &get, ALLOWED_CHANGES);
	if (mDevice->DeviceID == 0) {
		YOA_CRITICAL("Warning: failed to open audio device: {0}", SDL_GetError());
		return;
	}
	else if (get.freq != TARGET_SAMPLERATE
		|| get.format != AUDIO_FORMAT
		|| get.channels != TARGET_CHANNELS
		|| get.samples != TARGET_BUFFER)
	{
		YOA_ERROR("Failed to open audio device with requested parameters!");
	}

	mDevice->Samples = get.samples;
	mDevice->Channels = get.channels;
	mDevice->Frequency = get.freq;
	mDevice->Format = AudioDevice::ConvertFormat(get);
	YOA_INFO("Opened AudioDevice \"{0}\" ID: {1}\n\t{2} sample rate, {5}bit, {3} channels, buffer size {4}", 
		mDevice->GetDeviceName(), mDevice->DeviceID, mDevice->Frequency,
		mDevice->Channels, mDevice->Samples, mDevice->Format);

	mixL.reserve(mDevice->Samples);
	for (size_t i = 0; i < mixL.capacity(); i++)
		mixL.push_back(0.0f);
	mixR.reserve(mDevice->Samples);
	for (size_t i = 0; i < mixR.capacity(); i++)
		mixR.push_back(0.0f);

	mPlayingAudio.reserve(MAX_VOICES > 0 ? MAX_VOICES : 32);
	
	mResources = std::make_unique<ResourceManager>();
}

Mixer::~Mixer()
{
	Pause(true);
	// close SDL audio
	SDL_CloseAudioDevice(mDevice->DeviceID);
	mDevice = nullptr;
	// clean up resources
	mResources = nullptr;
}

void Mixer::Pause(const bool pause) noexcept
{
	if (mDevice == nullptr)
	{
		YOA_ERROR("No Device present!");
		return;
	}

	if (mPaused == pause)
		return;
	mPaused = pause;
	SDL_PauseAudioDevice(mDevice->DeviceID, pause ? 1 : 0);
	YOA_INFO("audio engine state set to \"paused = {0}\"", pause);
}

bool Mixer::IsPaused() noexcept
{
	return mPaused;
}

uint16_t Mixer::PlayWavFile(const std::string & filename, const bool loop, const float volume, const float pitch, const float fadeIn)
{
	if (mDevice == nullptr) {
		YOA_CRITICAL("Can't play audio. No Device present!");
		return 0;
	}

	std::shared_ptr<Sound> sound = mResources->GetSound(filename);
	if (sound == nullptr) {
		YOA_CRITICAL("Can't play audio. Could not load sound at path: {0}", filename);
		return 0;
	}

	std::shared_ptr<Voice> voice = GetVoice();
	if (voice == nullptr) {
		YOA_ERROR("Can't play audio. Could not aquire a Voice!");
		return 0;
	}
	
	voice->Sound = sound;
	voice->NextSample = 0;
	voice->Volume = volume;
	if (fadeIn > 0.0f) {
		// ensure current target and value are at 0.0f
		voice->smoothVolume.Reset(0.0f);
		// set fade duration (so the audio callback doesn't snap to the newly set value)
		voice->smoothVolume.SetFadeLength(static_cast<int>(fadeIn * mDevice->Frequency));
		// set fader target
		voice->smoothVolume.SetValue(1.0f);
	}
	else {
		// ensure current target and value are at 0.0f
		voice->smoothVolume.Reset(1.0f);
	}
	voice->Pitch = pitch;
	voice->IsLooping = loop;

	// avoid duplicate Voices
	if (this->StopVoice(voice->ID) == true)
	{
		YOA_WARN("Had to stop Voice: {0}", voice->ID);
	}

	// add voice to playing voices vector
	SDL_LockAudioDevice(mDevice->DeviceID);
	this->mPlayingAudio.push_back(voice);
	voice->State = ToPlay;
	SDL_UnlockAudioDevice(mDevice->DeviceID);

	YOA_INFO("Playing audio: {0} with voiceID: {1}", filename.c_str(), voice->ID);
	return voice->ID;
}

bool Mixer::StopVoice(const uint16_t id, const float fadeOut)
{
	if (mDevice == nullptr)
	{
		YOA_CRITICAL("Voice stopping failed! no Device present!");
		return false;
	}

	if (id == 0 || id > mVoiceCount)
	{
		YOA_ERROR("Can't stop specified Voice. Invadid voiceID: {0}", id);
		return false;
	}

	// loop throu sound channels removing stopped voices
	std::vector<std::shared_ptr<Voice>>::iterator it = mPlayingAudio.begin();
	std::vector<std::shared_ptr<Voice>>::iterator end = mPlayingAudio.end();
	for (; it != end; ++it) {
		auto voice = (*it);
		if (voice->ID != id)
			continue;
		float fade = fadeOut;
		if (fadeOut <= 0.0f)
			fade = 0.01f;
		voice->smoothVolume.SetValue(0.0f);
		voice->smoothVolume.SetFadeLength(static_cast<int>(fade * mDevice->Frequency));

		// TODO is lock needed? maybe make state atomic?
		SDL_LockAudioDevice(mDevice->DeviceID);
		voice->State = Stopping;
		SDL_UnlockAudioDevice(mDevice->DeviceID);
		return true;
	}
	return false;
}

std::shared_ptr<Voice> Mixer::GetVoice()
{
	std::shared_ptr<Voice> newVoice = nullptr;

	// Try getting a voice for recycling
	// TODO: use stack
	if (mAvailableVoices.size() > 0) {
		newVoice = mAvailableVoices.top();
		mAvailableVoices.pop();
		YOA_ASSERT(newVoice->State == Stopped);
	}
	else {
#if MAX_VOICES > 0
		if (lastVoice >= MAX_VOICES) {
			YOA_ERROR("Can't create new voice. Reached Voice limit of {0}", MAX_VOICES);
			return nullptr;
		}
#endif
		newVoice = std::make_shared<Voice>();
		if (newVoice == nullptr) {
			YOA_ERROR("Memory allocation error while making new Voice");
			return nullptr;
		}
		// set Voice ID
		newVoice->ID = ++mVoiceCount;
	}

	newVoice->Sound = nullptr;
	newVoice->NextSample = 0;
	return newVoice;
}

void Mixer::FillBuffer()
{
	// fill float buffer with silence
	for (size_t i = 0; i < mixL.size(); i++)
		mixL[i] = 0.0f;
	for (size_t i = 0; i < mixR.size(); i++)
		mixR[i] = 0.0f;

	for (auto voice : mPlayingAudio)
	{
		if (voice->State == ToPlay)
			voice->State = Playing;
		if (voice->State == Playing || voice->State == Stopping)
		{
			const float volumeFactor = voice->Volume;
			const float pitch = voice->Pitch;
			voice->smoothVolume.UpdateTarget();

			uint32_t length = mixL.size();
			if (voice->IsLooping == false)
			{
				uint32_t samplesRemaining = voice->GetSamplesRemaining() / voice->Sound->Channels;
				if (samplesRemaining >= 0 && samplesRemaining < length)
					length = samplesRemaining;
			}
			if (voice->State == Stopping) {
				uint32_t steps = voice->smoothVolume.GetRemainingFadeSteps();
				if (steps < length)
					length = steps;
			}
			YOA_ASSERT(length <= mixL.size());
			YOA_ASSERT(voice->Sound->Channels <= 2);
			float sampleIndex = 0;
			if (voice->Sound->Channels == 1) {
				float sample;
				for (uint32_t i = 0; i < length; i++) {
					sample = voice->GetSample(uint16_t(sampleIndex)) * volumeFactor * voice->smoothVolume.GetNext();
					mixL[i] += sample;
					mixR[i] += sample;
					sampleIndex++;// += pitch; // non-interpolating pitching
				}
				voice->AdvancePlayhead(length);
			}
			else if (voice->Sound->Channels == 2) {
				float volume;
				for (uint32_t i = 0; i < length; i++) {
					volume = volumeFactor * voice->smoothVolume.GetNext();
					mixL[i] += voice->GetSample(uint16_t(sampleIndex));
					sampleIndex++;
					mixR[i] += voice->GetSample(uint16_t(sampleIndex));
					sampleIndex++;
					//sampleIndex += pitch; // non-interpolating pitching
				}
				voice->AdvancePlayhead(length * voice->Sound->Channels);
			}

			if (voice->State == Stopping
				&& voice->smoothVolume.HasReachedTarget())
			{
				voice->State = Stopped;
			}
			if (voice->GetSamplesRemaining() <= 0
				&& voice->IsLooping == false)
			{
				// Non looping sound has no more mixable samples
				voice->State = Stopped;
			}
		}
	}

	// remove stopped voies
	for (int i = mPlayingAudio.size() - 1; i >= 0; i--) {
		if (mPlayingAudio[i]->State != Stopped)
			continue;
		auto voice = mPlayingAudio[i];
		voice->smoothVolume.SetFadeLength(0);
		voice->smoothVolume.SetValue(1.0f);
		voice->smoothVolume.UpdateTarget();
		mAvailableVoices.push(voice);
		mPlayingAudio.erase(mPlayingAudio.begin() + i);
	}

	for (uint32_t i = 0; i < mixL.size(); i++) {
		// clipping if float buffer outside of render range
		if (mixL[i] > 1.0f)
			mixL[i] = 1.0f;
		else if (mixL[i] < -1.0f)
			mixL[i] = -1.0f;
	}
	for (uint32_t i = 0; i < mixR.size(); i++) {
		// clipping if float buffer outside of render range
		if (mixR[i] > 1.0f)
			mixR[i] = 1.0f;
		else if (mixR[i] < -1.0f)
			mixR[i] = -1.0f;
	}

	// update render time
	AudioThread::GetInstance()->mTimer->AdvancemRenderTime(mixL.size());
}

inline void Mixer::AudioCallback(void * userdata, uint8_t * stream, int len)
{
	Mixer* mixer = (Mixer*)userdata;
	// render to mixing buffer
	mixer->FillBuffer();
	// fill output buffer
	uint32_t sampleIndex = 0;
	switch (mixer->mDevice->Format)
	{
	case YOA_Format_Float:
	{
		float* outF = (float*)stream;
		for (uint32_t i = 0; i < mixer->mixL.size(); i++)
		{
			outF[sampleIndex++] = mixer->mixL[i];
			outF[sampleIndex++] = mixer->mixR[i];
		}
		break;
	}
	case YOA_Format_Sint8:
	{
		int8_t* out8 = (int8_t*)stream;
		for (uint32_t i = 0; i < mixer->mixL.size(); i++)
		{
			out8[sampleIndex++] = static_cast<int8_t>(mixer->mixL[i] * 127);
			out8[sampleIndex++] = static_cast<int8_t>(mixer->mixR[i] * 127);
		}
		break;
	}
	case YOA_Format_Sint16:
	{
		int16_t* out16 = (int16_t*)stream;
		for (uint32_t i = 0; i < mixer->mixL.size(); i++)
		{
			out16[sampleIndex++] = static_cast<int16_t>(mixer->mixL[i] * 32767);
			out16[sampleIndex++] = static_cast<int16_t>(mixer->mixR[i] * 32767);
		}
		break;
	}
	case YOA_Format_Sint32:
	{
		int32_t* out32 = (int32_t*)stream;
		for (uint32_t i = 0; i < mixer->mixL.size(); i++)
		{
			out32[sampleIndex++] = static_cast<int32_t>(mixer->mixL[i] * 2147483647);
			out32[sampleIndex++] = static_cast<int32_t>(mixer->mixR[i] * 2147483647);
		}
		break;
	}
	default:
		YOA_ERROR("Unsupported output format: {0}", mixer->mDevice->Format);
		break;
	}
}
