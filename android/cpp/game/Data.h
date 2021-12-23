#pragma once

#include "iStd.h"

void loadData();
void saveData();

void setTutoclear(bool clear);
void setBestTime(float time);
void setLanguage(int index);

bool getTutoclear();
float getBestTime();

struct Data
{
	bool tutoClear;
	float bestTime;
	int language;
};
extern Data data;
