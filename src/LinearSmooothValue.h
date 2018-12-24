#pragma once

#include <atomic>
#include "YoaConfig.h"

class LinearSmooothValue {
private:
	std::atomic<float> target;
	float currentValue;
	float currentTarget;
	
	std::atomic<bool> hasReacheTarget;
	std::atomic<uint32_t> stepsToTarget;
	uint32_t countdown;
	float stepSize;
public:
	LinearSmooothValue(float initial = 1.0f, double sampleRate = TARGET_SAMPLERATE,
		double rampLengthSeconds = 0.01) noexcept;
	void Reset(const float startValue);

	uint32_t GetRemainingFadeSteps() const;
	bool HasReachedTarget() const;

	void SetValue(float newValue);
	void SetFadeLength(uint32_t samples);
	
	void UpdateTarget();
	float GetNext();
};
