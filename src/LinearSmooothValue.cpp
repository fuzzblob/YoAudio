#include "LinearSmooothValue.h"
#include <cmath>

LinearSmooothValue::LinearSmooothValue()
{
	currentTarget = currentValue = 1.0f;
	step = 0.0f;
	countdown = 0;
	stepsToTarget = (int)std::floor(0.01 * 48000);
}

LinearSmooothValue::LinearSmooothValue(float initial, double sampleRate, double rampLengthSeconds) noexcept
	: currentValue(initial), currentTarget(initial), step(0), countdown(0),
	stepsToTarget((int)std::floor(rampLengthSeconds * sampleRate))
{
}

int LinearSmooothValue::GetRemainingFadeSteps() const
{
	return stepsToTarget.load() - step;
}
bool LinearSmooothValue::HasReachedTarget() const
{
	return hasReacheTarget.load();
}

void LinearSmooothValue::SetValue(float newValue) {
	target.store(newValue);
}
void LinearSmooothValue::SetFadeLength(int samples)
{
	stepsToTarget.store(samples);
}
void LinearSmooothValue::UpdateTarget() {
	float newTarget = target.load();
	if (newTarget == currentTarget)
		return;
	currentTarget = newTarget;
	countdown = stepsToTarget.load();
	if (countdown <= 0)
	{
		currentValue = currentTarget;
		hasReacheTarget.store(true);
	}
	else
	{
		step = (currentTarget - currentValue) / countdown;
		hasReacheTarget.store(false);
	}
}
float LinearSmooothValue::GetNext() {
	if (hasReacheTarget.load())
		return currentValue;
	countdown--;
	if (countdown <= 0) {
		hasReacheTarget.store(true);
		return currentValue;
	}
	currentValue += step;
	return currentValue;
}
