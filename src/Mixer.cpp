#include "Mixer.h"

#include "AudioThread.h"
#include "Log.h"

Mixer::Mixer()
{
	mDevice = std::make_unique<AudioDevice>();
	SDL_memset(&(mDevice->SpecWanted), 0, sizeof(mDevice->SpecWanted));

	(mDevice->SpecWanted).freq = AUDIO_FREQUENCY;
	(mDevice->SpecWanted).format = AUDIO_FORMAT;
	(mDevice->SpecWanted).channels = AUDIO_CHANNELS;
	(mDevice->SpecWanted).samples = AUDIO_SAMPLES;
	(mDevice->SpecWanted).callback = AudioCallback;
	(mDevice->SpecWanted).userdata = this;

	mDevice->DeviceID = SDL_OpenAudioDevice(nullptr, 0, &(mDevice->SpecWanted), &(mDevice->SpecObtained), ALLOWED_CHANGES);
	if (mDevice->DeviceID == 0) {
		YOA_CRITICAL("Warning: failed to open audio device: {0}", SDL_GetError());
		return;
	}
	else if (mDevice->SpecObtained.freq != AUDIO_FREQUENCY
		|| mDevice->SpecObtained.format != AUDIO_FORMAT
		|| mDevice->SpecObtained.channels != AUDIO_CHANNELS
		|| mDevice->SpecObtained.samples != AUDIO_SAMPLES)
	{
		YOA_ERROR("Failed to open audio device with requested parameters!");
	}

	mMixStream.reserve(mDevice->SpecObtained.channels * mDevice->SpecObtained.samples);
	for (size_t i = 0; i < mMixStream.capacity(); i++) {
		mMixStream.push_back(0.0f);
	}

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

	if (pause)
	{
		if (m_Paused)
		{
			YOA_WARN("YoAudio already paused");
			return;
		}

		YOA_INFO("YoAudio pausing");
		SDL_PauseAudioDevice(mDevice->DeviceID, 1);
		m_Paused = true;
	}
	else
	{
		if (!m_Paused)
		{
			YOA_WARN("YoAudio already playing");
			return;
		}

		YOA_INFO("YoAudio resuming");
		SDL_PauseAudioDevice(mDevice->DeviceID, 0);
		m_Paused = false;
	}
}

bool Mixer::IsPaused() noexcept
{
	return m_Paused;
}

uint16_t Mixer::PlayWavFile(const std::string & filename, const bool loop, const float volume, const float pitch, const float fadeIn)
{
	if (mDevice == nullptr)
	{
		YOA_CRITICAL("Can't play audio. No Device present!");
		return 0;
	}

	std::shared_ptr<Sound> sound = mResources->GetSound(filename);
	if (sound == nullptr)
	{
		YOA_CRITICAL("Can't play audio. Could not load sound at path: {0}", filename);
		return 0;
	}

	std::shared_ptr<Voice> voice = mResources->GetVoice();
	if (voice == nullptr)
	{
		YOA_ERROR("Can't play audio. Could not aquire a Voice!");
		return 0;
	}
	voice->Sound = sound;
	voice->Sound->Spec.userdata = this;

	voice->PlayHead = voice->Sound->Buffer;
	voice->LengthRemaining = voice->Sound->Length;
	voice->Volume = volume;
	if (fadeIn > 0.0f) {
		// ensure current target and value are at 0.0f
		voice->smoothVolume.SetValue(0.0f);
		voice->smoothVolume.SetFadeLength(0);
		voice->smoothVolume.UpdateTarget();
		voice->smoothVolume.GetNext();
		// set fade duration (so the audio callback doesn't snap to the newly set value)
		voice->smoothVolume.SetFadeLength(static_cast<int>(fadeIn * mDevice->SpecObtained.freq));
	}
	// set fader target
	voice->smoothVolume.SetValue(1.0f);
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

	if (id == 0 || id > mResources->GetVoiceCount())
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
		voice->smoothVolume.SetFadeLength(static_cast<int>(fade * mDevice->SpecObtained.freq));

		SDL_LockAudioDevice(mDevice->DeviceID);
		voice->State = Stopping;
		//m_playingAudio.erase(it);
		SDL_UnlockAudioDevice(mDevice->DeviceID);
		return true;
	}
	return false;
}

inline void Mixer::AudioCallback(void * userdata, uint8_t * stream, int len)
{
	Mixer* mixer = (Mixer*)userdata;
	// convert from bytes to amount of 16bit samples expected
	const uint32_t streamLen = uint32_t(len / 2);
	// fill float buffer with silence
	std::vector<float> mixBuffer = mixer->mMixStream;
	for (size_t i = 0; i < streamLen; i++) {
		mixBuffer[i] = 0.0f;
	}

	// s16 to normalized float
	const float sampleFactor = 1.0f / 32768.0f;
	for (auto voice : mixer->mPlayingAudio)
	{
		if (voice->State == ToPlay)
			voice->State = Playing;
		if (voice->State == Playing || voice->State == Stopping)
		{
			const float volumeFactor = voice->Volume;
			const float pitch = voice->Pitch;

			uint32_t length = ((streamLen > voice->LengthRemaining / 2) ? voice->LengthRemaining / 2 : streamLen);

			const Sint16* samples = (Sint16*)voice->PlayHead;
			float sampleIndex = 0;
			float sample;

			voice->smoothVolume.UpdateTarget();
			for (uint32_t i = 0; i < length; i++)
			{
				sample = samples[static_cast<int>(sampleIndex)] * sampleFactor;
				mixBuffer[i] += sample * volumeFactor * voice->smoothVolume.GetNext();
				// TODO: implement interpolating pitching & resampling
				sampleIndex += pitch; // non-interpolating pitching
			}

			voice->PlayHead += (int)(length * 2 * pitch);
			voice->LengthRemaining -= (int)(length * 2 * pitch);

			if (voice->State == Stopping
				&& voice->smoothVolume.HasReachedTarget())
				voice->State = Stopped;
			if (voice->LengthRemaining <= 0)
			{
				if (voice->IsLooping == true)
				{
					// TODO: make seamless
					voice->PlayHead = voice->Sound->Buffer;
					voice->LengthRemaining = voice->Sound->Length;

					length = streamLen - length;
				}
				else
				{
					// Non looping sound has no more mixable samples
					voice->State = Stopped;
				}
			}
		}
	}
	// fill 16bit output buffer
	Sint16* current = (Sint16*)stream;
	for (uint32_t i = 0; i < streamLen; i++)
	{
		// clipping if float buffer outside of render range
		if (mixBuffer[i] > 1.0f)
			mixBuffer[i] = 1.0f;
		else if (mixBuffer[i] < -1.0f)
			mixBuffer[i] = -1.0f;
		// convert float back to 16bit
		current[i] = static_cast<Sint16>(mixBuffer[i] * 32767);
	}
	// update render time
	AudioThread::GetInstance()->mTimer->AdvancemRenderTime(streamLen);
}
