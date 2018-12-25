#pragma once

#include "YoaConfig.h"
#include "Platform.h"

// 0                                    No changes
// SDL_AUDIO_ALLOW_FREQUENCY_CHANGE     frequency changes (e.g. AUDIO_FREQUENCY = 48000, but allow files to play at 44.100)
// SDL_AUDIO_ALLOW_FORMAT_CHANGE        format change (e.g. AUDIO_FORMAT = S32LSB, but allow wave files of S16LSB to play)
// SDL_AUDIO_ALLOW_CHANNELS_CHANGE      channel number (e.g. AUDIO_CHANNELS = 2, allow actual 1)
// SDL_AUDIO_ALLOW_ANY_CHANGE           all changes above
#define ALLOWED_CHANGES 0//SDL_AUDIO_ALLOW_CHANNELS_CHANGE | SDL_AUDIO_ALLOW_FREQUENCY_CHANGE

#if TARGET_BITDEPTH == 8
#define AUDIO_FORMAT AUDIO_S8
#elif TARGET_BITDEPTH == 16
#define AUDIO_FORMAT AUDIO_S16SYS
#elif TARGET_BITDEPTH == 32
#define AUDIO_FORMAT AUDIO_S32SYS
#else
#error unsupported target bitdepth
#endif;

struct AudioDevice {
	SDL_AudioDeviceID DeviceID;

	// is the audio device paused
	bool mPaused = true;
	// audio callback buffer size (per channel)
	uint32_t Samples;
	// audio callback channel count
	uint8_t Channels;
	// audio callback sample rate
	uint32_t Frequency;
	// audio callback bit depth
	SampleFormat Format;

	~AudioDevice() {
		SetPaused(true);
		// close SDL audio
		SDL_CloseAudioDevice(DeviceID);
	}

	void SetPaused(const bool pause) noexcept {
		mPaused = pause;
		SDL_PauseAudioDevice(DeviceID, pause ? 1 : 0);
	}

	const char* GetDeviceName() const {
		return SDL_GetCurrentAudioDriver();
	}

	static SampleFormat ConvertFormat(const SDL_AudioSpec& spec) {
		if (SDL_AUDIO_ISFLOAT(spec.format)) {
			return YOA_Format_Float;
		}
		else {
			switch (SDL_AUDIO_BITSIZE(spec.format)) {
			case 8:
				return YOA_Format_Sint8;
			case 16:
				return YOA_Format_Sint16;
			case 32:
				return YOA_Format_Sint32;
			default:
				return YOA_Format_Unknown;
			}
		}
	}
};
