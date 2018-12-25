#pragma once

#include "Platform.h"

#include "Sound.h"
#include "LinearSmooothValue.h"
#include <memory>

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
	uint16_t ID = 0;
	VoiceState State = Stopped;
	std::shared_ptr<Sound> Sound;
	uint32_t NextSample = 0;
	bool IsLooping;
	LinearSmooothValue smoothVolume;
	float Volume;
	float Pitch;

	void AdvancePlayhead(uint32_t samples) {
		NextSample += samples;
		if (IsLooping) {
			NextSample %= Sound->Samples;
		}
	}
	uint32_t GetSamplesRemaining() const {
		return Sound->Samples - NextSample;
	}
	float GetSample(uint32_t position) {
		position += NextSample;
		if (IsLooping)
			position %= Sound->Samples;
		return Sound->GetSample(position);
	}
};
