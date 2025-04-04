#include "AudioDevice.h"
#include "Log.h"
#include "Utility.h"

namespace YoaEngine
{
	bool AudioDevice::IsPaused() const noexcept {
		return mPaused;
	}

	void AudioDevice::SetPaused(const bool pause) {
		if (mPaused == pause) {
			return;
		}
		YOA_TRACE("audio engine state set to \"paused = {0}\"", pause);
		mPaused = pause;
		SDL_PauseAudioDevice(mDeviceID, pause ? 1 : 0);
	}

	void AudioDevice::Lock() const noexcept {
		SDL_LockAudioDevice(mDeviceID);
	}

	void AudioDevice::Unlock() const noexcept {
		SDL_UnlockAudioDevice(mDeviceID);
	}

	const char* AudioDevice::GetDeviceName() const noexcept {
		return SDL_GetCurrentAudioDriver();
	}

	SampleFormat AudioDevice::GetYoaFormat(const SDL_AudioSpec& spec) noexcept {
		if (SDL_AUDIO_ISFLOAT(spec.format)) {
			return YOA_Format_Float;
		}
		else {
			YOA_ASSERT(SDL_AUDIO_ISINT(spec.format));
			// set up constants
			constexpr auto eightBit = 8;
			constexpr auto sixteenBit = 16;
			constexpr auto thirtytwoBit = 32;
			// format bitsize
			const auto bitsize = SDL_AUDIO_BITSIZE(spec.format);
			if (SDL_AUDIO_ISSIGNED(spec.format))
			{
				// format is signed int
				switch (bitsize) {
				case eightBit:
					// TODO(maris): not implemented
					[[fallthrough]];
				case sixteenBit:
					return YOA_Format_Sint16;
				case thirtytwoBit:
					return YOA_Format_Sint32;
				default:
					YOA_ASSERT(false);
					return YOA_Format_Unknown;
				}
			}
			else
			{
				// format is unsigned int
				switch (bitsize) {
				case eightBit:
					return YOA_Format_Uint8;
				case sixteenBit:
					// TODO(maris): not implemented
					[[fallthrough]];
				case thirtytwoBit:
					// TODO(maris): not implemented
					[[fallthrough]];
				default:
					YOA_ASSERT(false);
					return YOA_Format_Unknown;
				}
			}
		}
	}

	AudioDevice::AudioDevice(void* userData, void(*callback)(void* userdata, uint8_t* stream, int len)) {
		SDL_AudioSpec want;
		SDL_memset(&want, 0, sizeof(want));
		want.freq = TARGET_SAMPLERATE;
		want.format = AUDIO_FORMAT;
		want.channels = TARGET_CHANNELS;
		want.samples = TARGET_BUFFER;
		want.callback = callback;
		want.userdata = userData;
		SDL_AudioSpec get;

		mDeviceID = SDL_OpenAudioDevice(nullptr, 0, &want, &get, ALLOWED_CHANGES);
		if (mDeviceID == 0) {
			YOA_CRITICAL("Failed to open audio device: {0}", SDL_GetError());
			return;
		}
		else if (get.freq != TARGET_SAMPLERATE
			|| get.format != AUDIO_FORMAT
			|| get.channels != TARGET_CHANNELS
			|| get.samples != TARGET_BUFFER)
		{
			YOA_ERROR("AudioDevice opened with different parameters than requested!");
		}
		SetPaused(true);

		mSamples = get.samples;
		mChannels = get.channels;
		mFrequency = get.freq;
		mFormat = AudioDevice::GetYoaFormat(get);
		mDeviceName = GetDeviceName();

		YOA_INFO("Opened AudioDevice \"{0}\" ID: {1}\n\t{2} sample rate, {5}bit, {3} channels, buffer size {4}",
			mDeviceName, mDeviceID, mFrequency, mChannels, mSamples, (unsigned char)(mFormat));
	}

	AudioDevice::~AudioDevice() {
		SetPaused(true);
		// close SDL audio
		SDL_CloseAudioDevice(mDeviceID);
	}
}  // namespace YoaEngine
