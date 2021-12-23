#pragma once

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
	unsigned long now;

	int framesPerSec;
	float tickDt;
	int num;
	float lastDt;

	bool ingSlowTime;
	float slowDelta, _slowDelta;
	float slowDelay, _slowDelay;
};




