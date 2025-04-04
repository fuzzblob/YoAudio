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

	double Timer::DeltaTime() const noexcept
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
		constexpr auto nanosecondFactor = 1000000000.0;
		return std::chrono::duration_cast<std::chrono::nanoseconds>(clockType::now() - mEpoch).count() / nanosecondFactor;
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
