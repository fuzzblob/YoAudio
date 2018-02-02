#ifndef _VOICE_H
#define _VOICE_H

#include "Defs.h"
#include "Sound.h"
#include <memory>

enum VoiceState
{
	Initialize = 0,
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
	std::shared_ptr<Sound> Sound;
	uint32_t LengthRemaining;
	uint8_t * PlayHead;
	bool IsLooping;
	float Volume;
	float Pitch;
};

#endif // !_VOICE_H
