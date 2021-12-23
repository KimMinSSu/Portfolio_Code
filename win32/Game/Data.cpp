#include "Data.h"

Data data;
bool loadDataOk = false;

void loadData()
{
	if (loadDataOk)
		return;
	loadDataOk = true;

	FILE* fp = fopen("data.data", "rb");
	if (fp == NULL)
	{
		data.tutoClear = false;
		data.bestTime = 0.0f;
		return;
	}

	fread(&data, sizeof(data), 1, fp);
	fclose(fp);
}

void saveData()
{
	FILE* fp = fopen("data.data", "wb");
	fwrite(&data, sizeof(data), 1, fp);
	fclose(fp);
}

void setTutoclear(bool clear)
{
	data.tutoClear = clear;
	saveData();
}

void setBestTime(float time)
{
	data.bestTime = time;
	saveData();
}

bool getTutoclear()
{
	return data.tutoClear;
}

float getBestTime()
{
	return data.bestTime;
}
