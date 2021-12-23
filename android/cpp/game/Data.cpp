#include "Data.h"
#include "Script.h"

Data data;
bool loadDataOk = false;

void loadData()
{
	if (loadDataOk)
		return;
	loadDataOk = true;

	char path[1024];
	sprintf(path, "%s/data.data", getDevicePath());

	FILE* fp = fopen(path, "rb");
	if (fp == NULL)
	{
		data.tutoClear = false;
		data.bestTime = 0.0f;
		language = Kor;
		return;
	}
	fread(&data, sizeof(data), 1, fp);

	language = data.language;

	fclose(fp);
}

void saveData()
{
	char path[1024];
	sprintf(path, "%s/data.data", getDevicePath());

	FILE* fp = fopen(path, "wb");
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

void setLanguage(int index)
{
	data.language = index;
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
