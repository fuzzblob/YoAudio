#pragma once

#include <memory>

#include "Sample.h"
#include "LinearSmooothValue.h"
#include "StereoPanner.h"

enum VoiceState
{
	ToPlay = 1,
	Playing = 2,
	Paused = 3,
	Stopping = 4,
	Stopped = 5
};

class Voice
{
public:
	void AdvancePlayhead(const uint32_t samples) noexcept;
	uint32_t GetSamplesRemaining() const noexcept;
	float GetSample(uint32_t position, const uint8_t channel = 0);
	float GetReSample(float position, const uint8_t channel = 0);

	Voice(uint16_t id) : ID(id) { }
public:
	const uint16_t ID;
	VoiceState State = Stopped;
	std::shared_ptr<Sample> Sound;
	LinearSmooothValue Volume;
	StereoPanner Panning;
	bool IsLooping = false;
	float Pitch = 1.0f;
	uint32_t NextSample = 0;
};