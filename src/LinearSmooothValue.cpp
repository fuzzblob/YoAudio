#include "LinearSmooothValue.h"

#include <cmath>

LinearSmooothValue::LinearSmooothValue(float initial, double sampleRate, double rampLengthSeconds) noexcept
	: currentValue(initial), currentTarget(initial), stepSize(0), countdown(0),
	stepsToTarget((int)std::floor(rampLengthSeconds * sampleRate))
{
}

void LinearSmooothValue::Reset(const float startValue)
{
	SetValue(startValue);
	uint32_t steps = stepsToTarget.load();
	stepsToTarget.store(0);
	UpdateTarget();
	stepsToTarget.store(steps);
}

uint32_t LinearSmooothValue::GetRemainingFadeSteps() const
{
	return countdown;
}
bool LinearSmooothValue::HasReachedTarget() const
{
	return hasReacheTarget.load();
}

void LinearSmooothValue::SetValue(float newValue) {
	target.store(newValue);
}
void LinearSmooothValue::SetFadeLength(uint32_t samples)
{
	stepsToTarget.store(samples);
}
void LinearSmooothValue::UpdateTarget() {
	const float newTarget = target.load();
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
		stepSize = (currentTarget - currentValue) / countdown;
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
	currentValue += stepSize;
	return currentValue;
}
