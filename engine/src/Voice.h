#pragma once

#include "Sample.h"
#include "LinearSmooothValue.h"
#include "StereoPanner.h"

#include <memory>

namespace YoaEngine
{
	enum VoiceState : unsigned char
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
		void AdvancePlayhead(const float samples) noexcept;
		float GetSamplesRemaining() const noexcept;
		float GetSample(const float position, const uint8_t channel = 0);

		explicit Voice(uint32_t id) : ID(id) {}
	private:
		float LoadSample(uint32_t position, const uint8_t channel = 0);
	public:
		const uint32_t ID;
		VoiceState State = Stopped;
		std::shared_ptr<Sample> Sound;
		LinearSmooothValue Volume;
		StereoPanner Panning;
		bool IsLooping = false;
		float Pitch = 1.0f;
		float NextSample = 0;
	};
}  // namespace YoaEngine
