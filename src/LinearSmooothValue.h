#pragma once

#include <atomic>

class LinearSmooothValue {
private:
	std::atomic<float> target;
	float currentValue, currentTarget, step;
	std::atomic<bool> hasReacheTarget;
	std::atomic<uint32_t> stepsToTarget;
	uint32_t countdown;

public:
	
	LinearSmooothValue();
	LinearSmooothValue(float initial, double sampleRate, double rampLengthSeconds) noexcept;

	uint32_t GetRemainingFadeSteps() const;
	bool HasReachedTarget() const;

	void SetValue(float newValue);
	void SetFadeLength(uint32_t samples);
	
	void UpdateTarget();
	float GetNext();
};
