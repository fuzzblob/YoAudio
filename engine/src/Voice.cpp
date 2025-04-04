#include "Voice.h"
#include <cmath>
#include <algorithm>

namespace YoaEngine
{
	void Voice::AdvancePlayhead(const float samples) noexcept {
		if (IsLooping) {
			NextSample = std::fmod(NextSample + samples, Sound->Samples);
		}
		else {
			NextSample += samples;
		}
	}

	float Voice::GetSamplesRemaining() const noexcept {
		return std::max(0.0f, (Sound->Samples - NextSample));
	}

	float Voice::GetSample(const float position, const uint8_t channel) {
		float sample = NextSample + position;
		double intPart = 0;
		const double fractPart = std::modf(sample, &intPart);
		if (fractPart <  std::numeric_limits<double>::epsilon() &&
			fractPart > -std::numeric_limits<double>::epsilon())
		{
			return LoadSample(sample, channel);
		}
		// linear interpolation between 2 samples
		const float a = LoadSample(intPart, channel);
		const float b = LoadSample(intPart + 1, channel);
		return std::lerp(a, b, fractPart);
		// naive lerp implemntation
		// return a + (fractPart * (b - a));
	}

	float Voice::LoadSample(uint32_t position, const uint8_t channel) {
#ifdef _DEBUG
		YOA_ASSERT(channel <= this->Sound->Channels);
#endif
		return Sound->GetSample((position % Sound->Samples) + channel);
	}
}  // namespace YoaEngine
