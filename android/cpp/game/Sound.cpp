#include "Sound.h"

#define _audioMax 4
int onoffBGM = 1;
int onoffSFX = 1;
void loadSound()
{
	const char* audio[_audioMax] =
	{
		"Sound/bgm.wav",		//0
		"Sound/btn.wav", 		//0
		"Sound/explosion.wav",  //1
		"Sound/book.wav", 		//2
	};

	JNIEnv* env = getEnv();
	for(int i=0; i<_audioMax; i++)
	{
		jstring path = javaNewStringChar(env, audio[i]);

		if(i==0) ndkLoadBgmSound(path);
		else ndkLoadSfxSound(path);

		env->DeleteLocalRef(path);
	}
}

void playBgmSound(int index)
{
	ndkPlayBgmSound(index);
}

void playSfxSound(int index)
{
	ndkPlaySfxSound(index);
}

void stopBgmSound(int index)
{
	ndkStopBgmSound(index);
}

void stopSfxSound(int index)
{
	ndkStopSfxSound(index);
}

void volumeBgmSound(float volume)
{
	ndkVolumeBgmSound(volume);
}

void volumeSfxSound(float volume)
{
	ndkVolumeSfxSound(volume);
}

void pauseBgmSound(bool pause)
{
	ndkPauseBgmSound(pause);
}
