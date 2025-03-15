#include "Voice.h"

namespace YoaEngine
{
	void Voice::AdvancePlayhead(const uint32_t samples) noexcept {
		NextSample += samples;
		if (IsLooping) {
			NextSample %= Sound->Samples;
		}
	}

	uint32_t Voice::GetSamplesRemaining() const noexcept {
		return Sound->Samples - NextSample;
	}

	float Voice::GetSample(float position, const uint8_t channel) {
		const float delta = (position - int(position));
		if (delta == 0.0f)
			return LoadSample(position, channel);
		const uint32_t pos0 = NextSample + int(position);
		const uint32_t pos1 = pos0 + 1;
		const float s0 = LoadSample(pos0, channel);
		const float s1 = LoadSample(pos1, channel);
		return (delta * (s1 - s0)) + s0;
	}

	float Voice::LoadSample(uint32_t position, const uint8_t channel) {
		position += NextSample;
		if (IsLooping)
			position %= Sound->Samples;
		return Sound->GetSample(position + channel);
	}
}