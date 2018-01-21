#ifndef _VOICE_H
#define _VOICE_H

#include "Defs.h"
#include "Sound.h"

enum VoiceState
{
	ToPlay = 1,
	Playing = 2,
	Paused = 3,
	Stopping = 4,
	Stopped = 5
};

struct Voice
{
	uint16_t ID;
	VoiceState State;
	Sound * Sound;
	uint32_t LengthRemaining;
	uint8_t * PlayHead;
	bool IsLooping;
	float Volume;
	float Pitch;
};


#endif // !_VOICE_H
