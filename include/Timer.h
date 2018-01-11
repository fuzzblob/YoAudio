#ifndef _TIMER_H
#define _TIMER_H

#include "Defs.h"

class IMPORT_EXPORT Timer
{
private:
	unsigned int mSTartTicks;
	unsigned int mElapsedTicks;
	float mDeltaTime;
	float mTimeScale;
public:
	Timer();
	~Timer();

	void Reset();
	float DeltaTime();

	void SetTimeScale(float t);
	float TimeScale();

	void Update();
};

#endif // !_TIMER_H
