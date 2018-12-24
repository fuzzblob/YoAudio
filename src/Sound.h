#pragma once

#include "Platform.h"

struct Sound
{
	uint32_t Length;
	uint8_t * Buffer;

	SDL_AudioSpec Spec;

	int ChannelCount() const {
		return Spec.channels;
	}
	int SampleRate() const {
		return Spec.freq;
	}
	int BitSize() const {
		return SDL_AUDIO_BITSIZE(Spec.format);
	}
	bool IsFloat() const {
		return SDL_AUDIO_ISFLOAT(Spec.format);
	}
	bool IsSigned() const {
		return SDL_AUDIO_ISSIGNED(Spec.format);
	}
	bool IsBigEndian() const {
		return SDL_AUDIO_ISBIGENDIAN(Spec.format);
	}
};
