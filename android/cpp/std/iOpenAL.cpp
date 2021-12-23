#include "iOpenAL.h"

#include "iStd.h"

iOpenAL::iOpenAL(int audioNum)
{
}

iOpenAL::~iOpenAL()
{

}

typedef struct _WaveHeader0 {
	char riff[4];				// Chunk ID: "RIFF"
	unsigned int riffSize;		// Chunk size: 4+n
	char wave[4];				// WAVE ID: "WAVE"

	char fmt[4];				// Chunk ID: "fmt"
	unsigned int fmtSize;		// Chunk size: 16 or 18 or 40
	unsigned short format;		// Format code
	unsigned short channels;	// Number of interleaved channels
	unsigned int samplesPerSec;	// Sampling rate (blocks per second)
	unsigned int bytesPerSec;	// Data rate
	unsigned short blockAlign;	// Data block size(bytes)
	unsigned short bitsPerSample;// Bits per sample
} WaveHeader0;

typedef struct _WaveHeader1 {
	char data[4];				//'data'
	unsigned int dataSize;
} WaveHeader1;

void iOpenAL::initBuffer(int idx, const char* szFormat, ...)
{

}

void iOpenAL::initSource(int idx, bool repeat)
{

}

void iOpenAL::play(int idx, bool repeat)
{

}

void iOpenAL::pause(int idx)
{

}

void iOpenAL::stop(int idx)
{

}

void iOpenAL::volume(int idx, float vol)
{

}

static iOpenAL* al;
AudioInfo* audioInfo;
static int audioNum;
static int prevBgm;

void loadAudio(AudioInfo* ai, int aiNum)
{

}

void freeAudio()
{

}

void pauseAudio()
{

}

void resumeAudio()
{

}

void audioPlay(int list)
{

}

void audioStop(int index)
{

}

void audioStop()
{

}

void audioVolume(float bgm, float eff, int effNum)
{

}
