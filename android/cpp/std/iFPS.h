#pragma once

#include "iType.h"
#if (OS == OS_WINDOW)
#include "iWindow.h"
#elif (OS == OS_ANDROID)
#endif

#define DISPLAY_FPS

class iFPS
{
public:
	static iFPS* instance();
private:
	iFPS();
public:
	virtual ~iFPS();

	float update();

	void setSlowTime(float delta, float delay);

public:
#if (OS == OS_WINDOW)
	unsigned long now;
#elif (OS == OS_ANDROID)
	timeval now;
#endif

#ifdef DISPLAY_FPS
	int framesPerSec;
	float tickDt;
	int num;
#endif // DISPLAY_FPS
	float lastDt;

	bool ingSlowTime;
	float slowDelta, _slowDelta;
	float slowDelay, _slowDelay;
};
