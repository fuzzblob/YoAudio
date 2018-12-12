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

void Timer::Update()
{
	mElapsedTicks = SDL_GetTicks() - mSTartTicks;
	mDeltaTime = mElapsedTicks * 0.001f;
}

Timer::Timer()
{
	SDL_Init(SDL_INIT_TIMER);
	Reset();
}

Timer::~Timer()
{

}
