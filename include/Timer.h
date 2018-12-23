#ifndef _TIMER_H
#define _TIMER_H

#include "Platform.h"
#include "DllImportExport.h"

#include <chrono>
#include <atomic>

class YOA_API Timer
{
private:
	double mTime = 0.0;
	double mDeltaTime = 0.0;
	std::atomic<double> mRenderTime = 0.0;
	std::chrono::high_resolution_clock::time_point mEpoch;
public:
	Timer() noexcept;
	~Timer() noexcept;

	void ResetDeltaTime() noexcept;
	double DeltaTime() noexcept;

	void Update() noexcept;
	double GetTime() noexcept;

	void AdvancemRenderTime(double change);
	double RenderTime() const;
};

#endif // !_TIMER_H
