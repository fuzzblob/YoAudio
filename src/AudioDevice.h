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
#elif TARGET_BITDEPTH == 24
#define AUDIO_FORMAT AUDIO_S32SYS
#elif TARGET_BITDEPTH == 32
#define AUDIO_FORMAT AUDIO_S32SYS
#else
#error unsupported target bitdepth
#endif;

struct AudioDevice
{
	SDL_AudioDeviceID DeviceID;
	SDL_AudioSpec SpecWanted;
	SDL_AudioSpec SpecObtained;

	const char* GetDeviceName() {
		return SDL_GetCurrentAudioDriver();
	}

	int BitSize() const {
		return SDL_AUDIO_BITSIZE(SpecObtained.format);
	}
	bool IsFloat() const {
		return SDL_AUDIO_ISFLOAT(SpecObtained.format);
	}
	bool IsSigned() {
		return SDL_AUDIO_ISSIGNED(SpecObtained.format);
	}
	bool IsBigEndian() {
		return SDL_AUDIO_ISBIGENDIAN(SpecObtained.format);
	}
};
