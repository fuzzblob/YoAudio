#pragma once

#include "../include/EngineConfig.h"
#include "../include/Platform.h"
#include "AudioFormat.h"
#include <string>

// 0                                    No changes
// SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     frequency changes (e.g. AUDIO_FREQUENCY = 48000, but allow files to play at 44.100)
// SDL_AUDIO_ALLOW_FORMAT_CHANGE        format change (e.g. AUDIO_FORMAT = S32LSB, but allow wave files of S16LSB to play)
// SDL_AUDIO_ALLOW_CHANNELS_CHANGE      channel number (e.g. AUDIO_CHANNELS = 2, allow actual 1)
// SDL_AUDIO_ALLOW_ANY_CHANGE           all changes above
#define ALLOWED_CHANGES 0

#if TARGET_BITDEPTH == 8
#define AUDIO_FORMAT AUDIO_S8
#elif TARGET_BITDEPTH == 16
#define AUDIO_FORMAT AUDIO_S16SYS
#elif TARGET_BITDEPTH == 32
#define AUDIO_FORMAT AUDIO_S32SYS
#else
#error unsupported target bitdepth
#endif

namespace YoaEngine
{
	class AudioDevice {
	public:
		static SampleFormat GetYoaFormat(const SDL_AudioSpec& spec) noexcept;
	public:
		bool IsPaused() const noexcept;
		void SetPaused(const bool pause);

		void Lock() const noexcept;
		void Unlock() const noexcept;

		const char* GetDeviceName() const noexcept;
		AudioDevice(void* userData, void(*callback)(void* userdata, uint8_t* stream, int len));
		~AudioDevice();
	public:
		// audio callback buffer size (per channel)
		uint32_t Samples = 0;
		// audio callback sample rate
		uint32_t Frequency = 0;
		// audio callback bit depth
		SampleFormat Format = YOA_Format_Unknown;
		// audio callback channel count
		uint8_t Channels = 0;
	private:
		// is the audio device paused
		bool mPaused = true;
		SDL_AudioDeviceID DeviceID;
		std::string DeviceName;
	};
}  // namespace YoaEngine
