#ifndef _SOUND_H
#define _SOUND_H

#include "Platform.h"

struct Sound
{
	uint32_t Length;
	uint8_t * Buffer;

	SDL_AudioSpec Spec;
};

#endif // !_SOUND_H