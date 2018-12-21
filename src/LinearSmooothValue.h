#ifndef _LINEARMOOTHVALUE_H
#define _LINEARMOOTHVALUE_H

#include <atomic>

class LinearSmooothValue {
private:
	std::atomic<float> target;
	float currentValue, currentTarget, step;
	std::atomic<bool> hasReacheTarget;
	std::atomic<int> stepsToTarget;
	int countdown;

public:
	
	LinearSmooothValue();
	LinearSmooothValue(float initial, double sampleRate, double rampLengthSeconds) noexcept;

	bool HasReachedTarget();

	void SetValue(float newValue);
	void SetFadeLength(int samples);
	void UpdateTarget();
	float GetNext();
};

#endif _LINEARMOOTHVALUE_H