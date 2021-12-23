#pragma once

#include "iType.h"

#if (OS == OS_WINDOW)

#include <al.h>
#include <alc.h>
#pragma comment(lib, "OpenAL32.lib")

#elif (OS == OS_ANDROID)

//api 1 ~ 29
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Platform.h>

#include <SLES/OpenSLES_Android.h>
#include<SLES/OpenSLES_AndroidConfiguration.h>

//api 30 ~ aaudio

#endif

class iOpenAL
{
public:
	iOpenAL(int audioNum);
	virtual ~iOpenAL();

	void initBuffer(int idx, const char* szFormat, ...);
	void initSource(int idx, bool repeat);

	void play(int idx, bool repeat);
	void pause(int idx);
	void stop(int idx);
	void volume(int idx, float vol);

public:
#if (OS == OS_WINDOW)
	ALuint* gBuffer;
	ALuint* gSource;
#endif
	int bufCount;
};

struct AudioInfo
{
	const char* fileName;
	bool repeat;
	float volume;
};

void loadAudio(AudioInfo* ai, int aiNum);
void freeAudio();

void pauseAudio();
void resumeAudio();

void audioPlay(int idx);
void audioStop(int index);
void audioStop();
void audioVolume(float bgm, float eff, int effNum);

