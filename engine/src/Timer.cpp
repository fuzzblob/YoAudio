#include "Timer.h"

namespace YoaEngine
{
	Timer::Timer(clockType::time_point start) noexcept : mEpoch(start)
	{
		if (start == clockType::time_point::max()) {
			mEpoch = clockType::now();
		}
	}

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

	void Timer::AdvancemRenderTime(double change) noexcept
	{
		const double current = mRenderTime.load();
		mRenderTime.store(current + change);
	}

	double Timer::RenderTime() const noexcept
	{
		return mRenderTime.load();
	}
} // namespace YoaEngine
