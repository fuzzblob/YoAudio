#ifndef _TIMER_H
#define _TIMER_H

#include "Platform.h"
#include "DllImportExport.h"

#include <chrono>

class YOA_API Timer
{
private:
	double mTime = 0.0;
	float mDeltaTime = 0.0f;

	std::chrono::high_resolution_clock::time_point mEpoch;
public:
	Timer() noexcept;
	~Timer() noexcept;

	void ResetDeltaTime() noexcept;
	float DeltaTime() noexcept;

	void Update() noexcept;
	double GetTime() noexcept;
};

#endif // !_TIMER_H
