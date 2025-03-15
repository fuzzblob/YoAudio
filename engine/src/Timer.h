#pragma once

#include <chrono>
#include <atomic>

namespace YoaEngine
{
	class Timer
	{
	private:
		double mTime = 0.0;
		double mDeltaTime = 0.0;
		std::atomic<double> mRenderTime;
		std::chrono::high_resolution_clock::time_point mEpoch;
	public:
		Timer() noexcept;
		~Timer() noexcept;

		void ResetDeltaTime() noexcept;
		double DeltaTime() noexcept;

		void Update() noexcept;
		double GetTime() noexcept;

		void AdvancemRenderTime(double change) noexcept;
		double RenderTime() const noexcept;
	};
};