#ifndef _TIMER_H
#define _TIMER_H

#include "Defs.h"
#include "DllImportExport.h"

class YOA_API Timer
{
private:
	unsigned int mSTartTicks = 0;
	unsigned int mElapsedTicks = 0;
	float mDeltaTime = 0.0f;
public:
	Timer() noexcept;
	~Timer() noexcept;

	void Reset() noexcept;
	float DeltaTime() noexcept;

	void Update() noexcept;
};

#endif // !_TIMER_H
