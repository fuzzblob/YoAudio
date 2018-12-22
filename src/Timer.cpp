#include "Timer.h"

void Timer::ResetDeltaTime() noexcept
{
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime() noexcept
{
	return mDeltaTime;
}

void Timer::Update() noexcept
{
	double lastTime = mTime;
	mTime = GetTime();
	mDeltaTime += mTime - lastTime;
}

double Timer::GetTime() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mEpoch).count() / 1000000000.0;
}

Timer::Timer() noexcept
{
	mEpoch = std::chrono::high_resolution_clock::now();
	mTime = 0.0;
	mDeltaTime = 0.0f;
}

Timer::~Timer() noexcept { }
