#ifndef _VOICE_H
#define _VOICE_H

#include "Defs.h"
#include "Sound.h"
#include <memory>

enum VoiceState
{
	ToPlay = 1,
	Playing = 2,
	Paused = 3,
	Stopped = 5
};

struct Voice
{
	uint16_t ID = 0;
	VoiceState State = Stopped;
	std::shared_ptr<Sound> Sound;
	uint32_t LengthRemaining = 0;
	uint8_t * PlayHead;
	bool IsLooping;
	float Volume;
	float Pitch;
};

#endif // !_VOICE_H
