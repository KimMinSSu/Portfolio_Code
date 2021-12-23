#pragma once

#include "iType.h"

#include "iImage.h"

enum iButtonStat
{
	iButtonStatStd = 0,
	//iButtonStatHighlight,
	iButtonStatPushed
};

class iButton;
typedef void (*iButton_Method_Click)(iButton* btn, void* parm);

class iButton
{
public:
	iButton(iImage* img, iButton_Method_Click method, void* parm);
	virtual ~iButton();

	void paint(float dt, iPoint off);
	bool key(iKeyState stat, iPoint point);

	iButton_Method_Click methodClick;
	void* parmClick;

	iImage* img;
	bool pushed;
	float aniDt, _aniDt;
};

#define iButton_AniDt 0.2f

