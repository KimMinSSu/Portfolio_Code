#pragma once

#define OS_WINDOW 0
#define OS_ANDROID 1

#define OS OS_ANDROID

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if (OS == OS_WINDOW)

#include <windows.h>

#elif (OS == OS_ANDROID)
#include <jni.h>
#include <android/log.h>

#include <sys/time.h>
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef void (*VOID_METHOD)();
typedef void (*FLOAT_METHOD)(float);

struct Texture
{
	uint32 texID;// GLuint texID;
	float width, height;
	float potWidth, potHeight;
	int retainCount;
};

enum iKeyState
{
	iKeyStateBegan = 0,
	iKeyStateMoved,
	iKeyStateEnded,
};

struct iPoint;
typedef void (*KEY_METHOD)(iKeyState, iPoint);
struct iSize;
struct iRect;

#define DEV_WIDTH 1280
#define DEV_HEIGHT 720
#define SIZE_RATE 1.5f
//+++++++++++++++++++++++++++++
//+            Key            +
//+++++++++++++++++++++++++++++
#define keysLeftArrow	1
#define keysRightArrow	2
#define keysUpArrow		4
#define keysDownArrow	8

#define keysShift		16
#define keysESC			32
#define keysFull		64
#define keysEnter		128

#define keysDie			256

#define AnyKeys			512

//+++++++++++++++++++++++++++++
//+            Anc            +
//+++++++++++++++++++++++++++++
#define LEFT			1
#define RIGHT			2
#define HCENTER			4

#define TOP				8
#define BOTTOM			18
#define VCENTER			32

//+++++++++++++++++++++++++++++
//+          Reverse          +
//+++++++++++++++++++++++++++++
#define REVERSE_NONE	0
#define	REVERSE_WIDTH	1
#define	REVERSE_HEIGHT	2

//+++++++++++++++++++++++++++++
//+           Color           +
//+++++++++++++++++++++++++++++
#define RGBA_WHITE		1, 1, 1, 1
#define RGBA_BLACK		0, 0, 0, 1

#define RGB_BLACK		0, 0, 0
#define RGB_RED			1, 0, 0
#define RGB_GREEN		0, 1, 0
#define RGB_BLUE		0, 0, 1

#define RGB_GRAY		0.4f, 0.4f, 0.4f
#define RGB_MINT		0, 1, 0.6f
#define RGB_PINK		1, 0.7f, 1
#define RGB_YELLOW		1, 1, 0.4f
#define RGB_ORANGE		1, 0.4f, 0
#define RGB_SKY			0.47f, 0.92f, 1

#define RGB_SLOW		0.2f, 0.4f, 1
#define RGB_POISON		0, 0.6f, 0
#define RGB_CURSE		0.2f, 0, 0.6f
#define RGB_EXHAUSTE	1, 0.8f, 0