#pragma once

#include <atomic>
#include "YoaConfig.h"

class LinearSmooothValue {
private:
	std::atomic<float> target;
	float currentValue;
	float currentTarget;
	
	std::atomic<uint32_t> stepsToTarget;
	std::atomic_bool hasReacheTarget;
	uint32_t countdown;
	float stepSize;
public:
	LinearSmooothValue(float initial = 1.0f, double sampleRate = TARGET_SAMPLERATE,
		double rampLengthSeconds = 0.01) noexcept;
	void Reset(const float startValue) noexcept;

	uint32_t GetRemainingFadeSteps() const noexcept;
	bool HasReachedTarget() const noexcept;

	void SetValue(float newValue) noexcept;
	void SetFadeLength(uint32_t samples) noexcept;
	
	void UpdateTarget() noexcept;
	float GetNext() noexcept;
};
