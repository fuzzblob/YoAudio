#pragma once

#include <chrono>
#include <atomic>

namespace YoaEngine
{
	class Timer
	{
        using clockType = std::chrono::high_resolution_clock;
	private:
		double mTime = 0.0;
		double mDeltaTime = 0.0;
		std::atomic<double> mRenderTime = 0.0;
		clockType::time_point mEpoch;
	public:
          explicit Timer(clockType::time_point start = clockType::time_point::max()) noexcept;
          Timer(const Timer &) = delete;
          Timer(Timer &&) = delete;
          Timer &operator=(const Timer &) = delete;
          Timer &operator=(Timer &&) = delete;
          ~Timer() = default;

          void ResetDeltaTime() noexcept;
          double DeltaTime() const noexcept;

          void Update() noexcept;
          double GetTime() noexcept;

          void AdvancemRenderTime(double change) noexcept;
          double RenderTime() const noexcept;
	};
}  // namespace YoaEngine
