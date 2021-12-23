#include "Sound.h"

#define _audioMax 4
int onoffBGM = 1;
int onoffSFX = 1;
void loadSound()
{
	AudioInfo audio[_audioMax] = {
		{"Assets/Sound/btn.wav", false, 1.0f},
		{"Assets/Sound/bgm.wav", true, 1.0f},
		{"Assets/Sound/explosion.wav", false, 1.0f},
		{"Assets/Sound/book.wav", false, 1.0f},
	};
	loadAudio(audio, _audioMax);
	audioVolume(1.0f, 1.0f, _audioMax);
	audioPlay(1);
}

void playSound(int index)
{
	if (onoffSFX == 0)
		return;

	switch (index)
	{
	case 0: audioPlay(0); break;
	case 2: audioPlay(2); break;
	case 3: audioPlay(3); break;
	}
}

