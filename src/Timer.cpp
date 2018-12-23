#include "Timer.h"

void Timer::ResetDeltaTime() noexcept
{
	mDeltaTime = 0.0f;
}

double Timer::DeltaTime() noexcept
{
	return mDeltaTime;
}

void Timer::Update() noexcept
{
	const double lastTime = mTime;
	mTime = GetTime();
	mDeltaTime += mTime - lastTime;
}

double Timer::GetTime() noexcept
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mEpoch).count() / 1000000000.0;
}

void Timer::AdvancemRenderTime(double change)
{
	const double current = mRenderTime.load();
	mRenderTime.store(current + change);
}

double Timer::RenderTime() const
{
	return mRenderTime.load();
}

Timer::Timer() noexcept
{
	mEpoch = std::chrono::high_resolution_clock::now();
	mTime = 0.0;
	mDeltaTime = 0.0f;
}

Timer::~Timer() noexcept { }
