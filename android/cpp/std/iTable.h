#pragma once

#include "iType.h"

#include "iImage.h"
#include "iRect.h"

#define TABLE_USE_FBO 1

typedef void (*iTable_Method_Draw)(float dt, iPoint off);
typedef void (*iTable_Method_Select)(int selected);

struct iTableBar
{
	bool horizontal;
	iRect rt;

	void paint(	float dt,
				iPoint off,
				iPoint offMin,
				iPoint offMax,
				float alpha);
};

class iTable
{
public:
	iTable();
	virtual ~iTable();

	void init(	iRect rt,
				iSize sz,
				iTable_Method_Draw mDraw,
				iTable_Method_Select mSelect,
				iImage** imgs, int num);
	bool addBar(bool horizontal, iRect rt);

	void paint(float dt, iPoint off);
	bool key(iKeyState stat, iPoint point);

public:
	iTable_Method_Draw methodDraw;
	iTable_Method_Select methodSelect;
	iRect rt;
	iPoint off, offMin, offMax;

#if TABLE_USE_FBO
	Texture* texFBO;
#endif
	iImage** imgBtn;
	int numBtn, selectedBtn;

	bool touching;
	iPoint prevPoint;

#if (OS == OS_WINDOW)
	unsigned long prevTime;
#elif (OS == OS_ANDROID)
	timeval prevTime;
#endif

	iPoint vector;
	float force;

	iTableBar* tableBar;
	int tableBarNum;
	float tableBarAlpha;
};

