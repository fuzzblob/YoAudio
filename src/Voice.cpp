#include "Voice.h"

void Voice::AdvancePlayhead(const uint32_t samples) noexcept {
	NextSample += samples;
	if (IsLooping) {
		NextSample %= Sound->Samples;
	}
}

uint32_t Voice::GetSamplesRemaining() const noexcept {
	return Sound->Samples - NextSample;
}

float Voice::GetSample(uint32_t position, const uint8_t channel) {
	position += NextSample;
	if (IsLooping)
		position %= Sound->Samples;
	return Sound->GetSample(position + channel);
}

float Voice::GetReSample(float position, const uint8_t channel) {
	const uint32_t pos0 = NextSample + int(position);
	const uint32_t pos1 = pos0 + 1;
	const float s0 = GetSample(pos0, channel);
	const float s1 = GetSample(pos1, channel);
	return ((position - int(position)) * (s1 - s0)) + s0;
}