#include "Mixer.h"

#include <algorithm>
#include <cmath>
#include <memory>

#include "AudioFormat.h"
#include "Log.h"
#include "Utility.h"

namespace YoaEngine
{
	Mixer::Mixer()
	{
		// initialize audio device
		mDevice = std::make_unique<AudioDevice>(this, AudioCallback);
		if (mDevice->GetFormat() == YOA_Format_Unknown) {
			return;
		}
		// reserve mix buffers for callback
		auto sampleCount = mDevice->GetSamples();
		mixL.reserve(sampleCount);
		for (size_t i = 0; i < mixL.capacity(); i++) {
			mixL.push_back(0.0f);
		}
		mixR.reserve(sampleCount);
		for (size_t i = 0; i < mixR.capacity(); i++) {
			mixR.push_back(0.0f);
		}
		// unpause audio device
		mDevice->SetPaused(false);
		// initialize other resources
		mTimer = std::make_unique<YoaEngine::Timer>();
		constexpr int reserveAmt = (MAX_VOICES > 0) ? MAX_VOICES : 32;
		mPlayingAudio.reserve(reserveAmt);
		mResources = std::make_unique<ResourceManager>();
	}

	Mixer::~Mixer()
	{
		mDevice = nullptr;
		mTimer = nullptr;
		mResources = nullptr;
	}

	void Mixer::Pause(const bool pause)
	{
		if (!mDevice)
		{
			YOA_ERROR("No Device present!");
			return;
		}

		mDevice->SetPaused(pause);
	}

	bool Mixer::IsPaused() noexcept
	{
		return mDevice->IsPaused();
	}

	uint32_t Mixer::PlayWavFile(const std::string& filename, const bool loop, const float volume,
		const float pitch, const float fadeIn, const float pan, const bool startPaused)
	{
		if (!mDevice) {
			YOA_CRITICAL("Can't play audio. No Device present!");
			return 0;
		}
		// load sound
		std::shared_ptr<Sample> sound = mResources->GetSound(filename);
		if (!sound) {
			YOA_CRITICAL("Can't play audio. Could not load sound at path: {0}", filename);
			return 0;
		}
		// get voice
		std::shared_ptr<Voice> voice = GetVoiceAvailable();
		if (!voice) {
			YOA_ERROR("Can't play audio. Could not aquire a Voice!");
			return 0;
		}
		// set audio properties
		voice->Sound = sound;
		voice->NextSample = 0;
		voice->Pitch = pitch;
		voice->IsLooping = loop;
		voice->Panning.Set(pan);
		// set volume & fade
		if (fadeIn > 0.0f) {
			// ensure current target and value are at 0.0f
			voice->Volume.Reset(0.0f);
			// set fade duration (so the audio callback doesn't snap to the newly set value)
			voice->Volume.SetFadeLength(static_cast<int>(fadeIn * mDevice->GetFrequency()));
			// set fader target
			voice->Volume.SetValue(std::max(0.0f, std::min(1.0f, volume)));
		}
		else {
			// ensure current target and value are at 0.0f
			voice->Volume.Reset(std::max(0.0f, std::min(1.0f, volume)));
		}

		// add voice to playing voices vector
		if (!startPaused)
		{
			voice->State = ToPlay;
			mDevice->Lock();
			this->mPlayingAudio.push_back(voice);
			mDevice->Unlock();

			YOA_INFO("Playing audio: {0} with voiceID: {1}", filename.c_str(), voice->ID);
		}
		else {
			YOA_INFO("Readied audio: {0} with voiceID: {1}, but kept it paused.", filename.c_str(), voice->ID);
		}
		return voice->ID;
	}

	bool Mixer::StopVoice(const uint32_t id, float fadeOut)
	{
		auto voice = GetVoiceActive(id);
		if (!voice) {
			return false;
		}
		fadeOut = std::max(0.01f, fadeOut);
		voice->Volume.SetValue(0.0f);
		voice->Volume.SetFadeLength(static_cast<int>(fadeOut * mDevice->GetFrequency()));

		// TODO(maris): is lock needed? maybe make state atomic?
		mDevice->Lock();
		voice->State = Stopping;
		mDevice->Unlock();
		return true;
	}

	void Mixer::SetVoiceVolume(const uint32_t id, const float value, const float fade)
	{
		auto voice = GetVoiceActive(id);
		if (!voice) {
			return;
		}
		voice->Volume.SetValue(std::max(0.0f, std::min(1.0f, value)));
		voice->Volume.SetFadeLength(static_cast<int>(fade * mDevice->GetFrequency()));
	}

	void Mixer::SetVoicePan(const uint32_t id, const float value)
	{
		auto voice = GetVoiceActive(id);
		if (!voice) {
			return;
		}
		voice->Panning.Set(std::max(-1.0f, std::min(1.0f, value)));
	}

	void Mixer::StopSound(const float fade)
	{
		// loop throu sound channels removing stopped voices
		for (auto& voice : mPlayingAudio) {
			StopVoice(voice->ID, fade);
		}
	}

	std::shared_ptr<Voice> Mixer::GetVoiceAvailable()
	{
		std::shared_ptr<Voice> newVoice = nullptr;

		// Try getting a voice for recycling
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
			newVoice = std::make_unique<Voice>(++mVoiceCount);
		}

		newVoice->Sound = nullptr;
		newVoice->NextSample = 0;
		return newVoice;
	}

	std::shared_ptr<Voice> Mixer::GetVoiceActive(const uint32_t id)
	{
		if (!mDevice) {
			YOA_CRITICAL("Voice stopping failed! no Device present!");
			return nullptr;
		}
		else if (id == 0 || id > mVoiceCount) {
			YOA_ERROR("Can't stop specified Voice. Invadid voiceID: {0}", id);
			return nullptr;
		}

		// loop throu sound channels removing stopped voices
		for (auto& voice : mPlayingAudio) {
			if (voice->ID != id) {
				continue;
			}
			return voice;
		}
		return nullptr;
	}

	void Mixer::FillBuffer()
	{
		const uint32_t bufferSize = mixL.size();
		// fill float buffer with silence
		for (size_t i = 0; i < bufferSize; i++) {
			mixL[i] = 0.0f;
		}
		for (size_t i = 0; i < bufferSize; i++) {
			mixR[i] = 0.0f;
		}
		// loop over voices and mix their audio into buffer
		for (auto& voice : mPlayingAudio)
		{
			// switch voices that are ready to playing
			if (voice->State == ToPlay) {
				voice->State = Playing;
			}
			// mix only actively playing voices
			if (voice->State == Playing || voice->State == Stopping)
			{
				// get pitch and resampling factor
				const float pitch = voice->Pitch * (1.0f * voice->Sound->Frequency) / mDevice->GetFrequency();
				// Update the LinearSmoothValue objects
				voice->Volume.UpdateTarget();
				voice->Panning.Pan.UpdateTarget();
				// set amount of samples to be rendered
				uint32_t length = bufferSize;
				if (voice->IsLooping == false) {
					// if not looping, will we run out of samples?
					const uint32_t samplesRemaining = static_cast<uint32_t>(voice->GetSamplesRemaining() / pitch);
					if (samplesRemaining > 0 && samplesRemaining < length) {
						length = samplesRemaining;
					}
				}
				if (voice->State == Stopping) {
					// if stopping will the fadeout be finished in fewer samples?
					const uint32_t steps = voice->Volume.GetRemainingFadeSteps();
					if (steps < length) {
						length = steps;
					}
				}
				//YOA_ASSERT(length <= bufferSize);
				//YOA_ASSERT(voice->Sound->Channels <= 2);
				float sampleIndex = 0.0f;
				if (voice->Sound->Channels == 1) {
					for (uint32_t i = 0; i < length; i++) {
						const float volume = voice->Volume.GetNext();
						const float sample = voice->GetSample(sampleIndex);
						voice->Panning.CalculateNext();
						mixL[i] += sample * volume * voice->Panning.volL;
						mixR[i] += sample * volume * voice->Panning.volR;
						sampleIndex += pitch;
						//voice->AdvancePlayhead(pitch);
					}
					voice->AdvancePlayhead(length * pitch);
				}
				else if (voice->Sound->Channels == 2) {
					for (uint32_t i = 0; i < length; i++) {
						const float volume = voice->Volume.GetNext();
						voice->Panning.CalculateNext();
						mixL[i] += voice->GetSample(sampleIndex, 0) * volume * voice->Panning.volL;
						mixR[i] += voice->GetSample(sampleIndex, 1) * volume * voice->Panning.volR;
						sampleIndex += pitch;
						//voice->AdvancePlayhead(pitch);
					}
					voice->AdvancePlayhead(length * pitch);
				}

				if (voice->State == Stopping && voice->Volume.HasReachedTarget()) {
					voice->State = Stopped;
				}
				else if (voice->IsLooping == false
					&& voice->GetSamplesRemaining() <= 0.0f)
				{
					// Non looping sound has no more mixable samples
					voice->State = Stopped;
				}
			}
		}

		// remove stopped voies
		for (int i = mPlayingAudio.size() - 1; i >= 0; i--) {
			// check if already stopped
			if (mPlayingAudio[i]->State != Stopped) {
				continue;
			}
			auto& voice = mPlayingAudio[i];
			voice->Volume.SetFadeLength(0);
			voice->Volume.SetValue(1.0f);
			voice->Volume.UpdateTarget();
			mAvailableVoices.push(voice);
			mPlayingAudio.erase(mPlayingAudio.begin() + i);
		}

		// clip both mix buffers
		for (uint32_t i = 0; i < bufferSize; i++) {
			// clipping if float buffer outside of render range
			if (mixL[i] > 1.0f) {
				mixL[i] = 1.0f;
			}
			else if (mixL[i] < -1.0f) {
				mixL[i] = -1.0f;
			}
		}
		for (uint32_t i = 0; i < bufferSize; i++) {
			// clipping if float buffer outside of render range
			if (mixR[i] > 1.0f) {
				mixR[i] = 1.0f;
			}
			else if (mixR[i] < -1.0f) {
				mixR[i] = -1.0f;
			}
		}

		// update render time
		mTimer->AdvancemRenderTime(bufferSize);
	}

	inline void Mixer::AudioCallback(void* userdata, uint8_t* stream, int len)
	{
		UNUSED(len);
		//Mixer* mixer = (Mixer*)userdata;
		Mixer* mixer = static_cast<Mixer*>(userdata);
		// render to mixing buffer
		mixer->FillBuffer();
		// fill output buffer
		uint32_t sampleIndex = 0;
		switch (mixer->mDevice->GetFormat())
		{
		case YOA_Format_Float:
		{
			float* outF = (float*)stream;
			for (uint32_t mix = 0; mix < mixer->mixL.size(); mix++)
			{
				outF[sampleIndex++] = mixer->mixL[mix];
				outF[sampleIndex++] = mixer->mixR[mix];
			}
			return;
		}
		case YOA_Format_Uint8:
		{
			int8_t* out8 = (int8_t*)stream;
			for (uint32_t mix = 0; mix < mixer->mixL.size(); mix++)
			{
				out8[sampleIndex++] = static_cast<uint8_t>((mixer->mixL[mix] + 1.0f) * 127);
				out8[sampleIndex++] = static_cast<uint8_t>((mixer->mixR[mix] + 1.0f) * 127);
			}
			return;
		}
		case YOA_Format_Sint16:
		{
			int16_t* out16 = (int16_t*)stream;
			for (uint32_t mix = 0; mix < mixer->mixL.size(); mix++)
			{
				out16[sampleIndex++] = static_cast<int16_t>(mixer->mixL[mix] * 32767);
				out16[sampleIndex++] = static_cast<int16_t>(mixer->mixR[mix] * 32767);
			}
			return;
		}
		case YOA_Format_Sint32:
		{
			int32_t* out32 = (int32_t*)stream;
			for (uint32_t mix = 0; mix < mixer->mixL.size(); mix++)
			{
				out32[sampleIndex++] = static_cast<int32_t>(mixer->mixL[mix] * 2147483647.0f);
				out32[sampleIndex++] = static_cast<int32_t>(mixer->mixR[mix] * 2147483647.0f);
			}
			return;
		}
		default:
			const auto f = (unsigned char)(mixer->mDevice->GetFormat());
			YOA_ERROR("Unsupported output format: {0}", f);
			int32_t* out32 = (int32_t*)stream;
			// zero output / fill with silence
			for (uint32_t mix = 0; mix < mixer->mixL.size(); mix++)
			{
				out32[sampleIndex++] = 0;
				out32[sampleIndex++] = 0;
			}
			return;
		}
	}
}  // namespace YoaEngine
