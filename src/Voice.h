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
	float Pitch;

	void AdvancePlayhead(const uint32_t samples) {
		NextSample += samples;
		if (IsLooping) {
			NextSample %= Sound->Samples;
		}
	}
	uint32_t GetSamplesRemaining() const {
		return Sound->Samples - NextSample;
	}
	float GetSample(uint32_t position, const uint8_t channel = 0) {
		position += NextSample;
		if (IsLooping)
			position %= Sound->Samples;
		return Sound->GetSample(position + channel);
	}
	float GetReSample(float position, const uint8_t channel = 0) {
		uint32_t pos0 = NextSample + int(position);
		uint32_t pos1 = pos0 + 1;
		float s0 = GetSample(pos0, channel);
		float s1 = GetSample(pos1, channel);
		float t = position - int(position);
		float output = s0 + t * (s1 - s0);
		return output;
	}
};
