#pragma once

#include "Platform.h"

struct Sound
{
	uint32_t Length;
	uint8_t * Buffer;

	SDL_AudioSpec Spec;
};
