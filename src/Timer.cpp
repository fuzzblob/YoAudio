#include "Timer.h"

void Timer::Reset()
{
	mSTartTicks = SDL_GetTicks();
	mElapsedTicks = 0;
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime()
{
	return mDeltaTime;
}

void Timer::SetTimeScale(float t)
{
	mTimeScale = t;
}

float Timer::TimeScale()
{
	return mTimeScale;
}

void Timer::Update()
{
	mElapsedTicks = SDL_GetTicks() - mSTartTicks;
	mDeltaTime = mElapsedTicks * 0.001f;
}

Timer::Timer()
{
	Reset();
	mTimeScale = 1.0f;
}

Timer::~Timer()
{

}
