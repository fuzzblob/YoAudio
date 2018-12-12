#ifndef _TIMER_H
#define _TIMER_H

#include "Defs.h"
#include "DllImportExport.h"

class IMPORT_EXPORT Timer
{
private:
	unsigned int mSTartTicks;
	unsigned int mElapsedTicks;
	float mDeltaTime;
public:
	Timer();
	~Timer();

	void Reset();
	float DeltaTime();

	void Update();
};

#endif // !_TIMER_H
