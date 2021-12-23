#include "Intro.h"

#include "Loading.h"
#include "Menu.h"

#include "PopMethod.h"
#include "Script.h"
#include "Sound.h"

float introDt;

void loadIntro()
{
	createPopIntro();
	showPopIntro(true);

	introDt = 0.0f;
}

void freeIntro()
{
	freePopIntro();
}

void drawIntro(float dt)
{
	introDt += dt;

	if (introDt >3.0f && keyDown > 0)
		setLoading(gs_menu, freeIntro, loadMenu);

	drawPopIntro(dt);
}

void keyIntro(iKeyState state, iPoint p)
{
	switch (state)
	{
	case iKeyStateEnded:
		if (introDt < 3.0f)
			break;
			setLoading(gs_menu, freeIntro, loadMenu);
			playSfxSound(1);
		break;
	}
	
}

//+++++++++++++++++++++++++++++
//+          PopIntro         +
//+++++++++++++++++++++++++++++
iPopup* popIntro;
Texture* texPress;

void drawPopIntroAfter(iPopup* pop, float dt, float rate);

void createPopIntro()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();
	iGraphics* g = iGraphics::share();

	tex = methodIntro();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->_aniDt = 3.0f;
	pop->openPoint = iPointMake(0, devSize.height);
	pop->closePoint = iPointMake(0, 0);
	pop->methodAfter = drawPopIntroAfter;
	popIntro = pop;

	size = iSizeMake(devSize.width * 0.8f, 40 * SIZE_RATE);
	g->init(size);
	setStringSize(30 * SIZE_RATE);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, script[language].strWords.press);
	texPress = g->getTexture();
}

void freePopIntro()
{
	delete popIntro;
	freeImage(texPress);
}

void showPopIntro(bool show)
{
	popIntro->show(show);
}

void drawPopIntroAfter(iPopup* pop, float dt, float rate)
{
	if (pop->stat != iPopupProc)
		return;

	float a = _sin(180 * introDt);
	if (a < 0)
		a *= -1;

	setRGBA(1, 1, 1, a);
	drawImage(texPress, devSize.width / 2, devSize.height / 2, VCENTER | HCENTER);
	setRGBA(RGBA_WHITE);
	setRGBA(1, 1, 1, 1);
}

void drawPopIntro(float dt)
{
	popIntro->paint(dt);
}
