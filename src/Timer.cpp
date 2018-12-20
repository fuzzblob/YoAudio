#include "Timer.h"

void Timer::Reset() noexcept
{
	mSTartTicks = SDL_GetTicks();
	mElapsedTicks = 0;
	mDeltaTime = 0.0f;
}

float Timer::DeltaTime() noexcept
{
	return mDeltaTime;
}

void Timer::Update() noexcept
{
	mElapsedTicks = SDL_GetTicks() - mSTartTicks;
	mDeltaTime = mElapsedTicks * 0.001f;
}

Timer::Timer() noexcept
{
	SDL_Init(SDL_INIT_TIMER);
	Reset();
}

Timer::~Timer() noexcept { }
