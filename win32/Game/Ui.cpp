#include "UI.h"

#include "Proc.h"
#include "Player.h"
#include "PopMethod.h"
#include "Script.h"

#define _uiDt 2.0f
float uiDt;

void loadUI()
{
	createHpBar();
	createStamina();
	createClock();
	createCurse();

	uiDt = 0.0f;
}

void freeUI()
{
	freeHpBar();
	freeStamina();
	freeClock();
	freeCurse();
}

void drawUI(float dt, float _dt)
{
	drawHpBar(dt);
	drawStamina(dt);
	drawClock(_dt);
	drawCurse(dt);
}

//+++++++++++++++++++++++++++++
//+          HpBar            +
//+++++++++++++++++++++++++++++
iPopup* popHpBar;
Texture* texHpBar;

void drawHpBarAfter(iPopup* pop, float dt, float rate);

#define hpBarMax 200.0f
#define hpRate 50.0f

void createHpBar()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(hpBarMax + 8, 18);
	tex = methodBar(size, 0);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(hpBarMax, 10);
	tex = methodBar(size, 1);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4, 4);
	pop->addObject(img);

	pop->_aniDt = 2.0f;
	pop->style = iPopupMove;
	pop->openPoint = iPointMake(20, -40);
	pop->closePoint = iPointMake(20, 20);
	pop->methodAfter = drawHpBarAfter;
	popHpBar = pop;

	size = iSizeMake(hpBarMax, 10);
	texHpBar = methodBar(size, 3);
}

void freeHpBar()
{
	freeImage(texHpBar);
	delete popHpBar;
}

void drawHpBar(float dt)
{
	popHpBar->paint(dt);
}

iPoint currPopPosition(iPopup* pop)
{
	iPoint p = pop->closePoint;

	if (pop->stat == iPopupOpen)
		p = easeIn(pop->aniDt / pop->_aniDt, pop->openPoint, pop->closePoint);
	else if (pop->stat == iPopupClose)
		p = easeOut(pop->aniDt / pop->_aniDt, pop->closePoint, pop->openPoint);

	return p;
}

void drawHpBarAfter(iPopup* pop, float dt, float rate)
{
	float hpWidth = player->hp * (hpBarMax / playerHp);

	if (popHpBar->aniDt >= 2.0f)
		uiDt += dt;

	iPoint p = currPopPosition(pop);

	if (uiDt < 2.0f)
	{
		float texHpWidth = easeIn(uiDt / _uiDt, 0, hpWidth);
		//drawImage(texHpBar, 24, 24, TOP | LEFT, 0, 0,
		drawImage(texHpBar, p.x + 4, p.y + 4, TOP | LEFT, 0, 0,
			texHpWidth, texHpBar->height,
			1, 1, 0, 0);
	}
	else
	{
		texHpBar->width -= hpRate * dt;
		if (texHpBar->width < hpWidth)
			texHpBar->width = hpWidth;
		if (texHpBar->width < 0)
			texHpBar->width = 0;
		//drawImage(texHpBar, 24, 24, TOP | LEFT, 0, 0,
		drawImage(texHpBar, p.x + 4, p.y + 4, TOP | LEFT, 0, 0,
			texHpBar->width, texHpBar->height,
			1, 1, 0, 0);
	}
}

void showPopHpBar(bool show)
{
	popHpBar->show(show);
}

//+++++++++++++++++++++++++++++
//+          Stamina          +
//+++++++++++++++++++++++++++++
iPopup* popStamina;
Texture* texStamina;
Texture* texExhauste;

void drawStaminaAfter(iPopup* pop, float dt, float rate);

iImage** imgGameReady;
void openPopStamina(iPopup* pop)
{
	for (int i = 0; i < 2; i++)
		imgGameReady[i]->alpha = 1.0f;
}

void createStamina()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(hpBarMax + 8, 18);
	tex = methodBar(size, 0);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(maxStamina, 10);
	tex = methodBar(size, 2);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4, 4);
	pop->addObject(img);

	size = iSizeMake(maxStamina, 10);
	tex = methodBar(size, 2);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4, 4);
	pop->addObject(img);

#if 1
	imgGameReady = new iImage * [2];

	iGraphics* g = iGraphics::share();
	size = iSizeMake(200, 30);
	g->init(size);

	setStringSize(20);
	setStringRGBA(RGBA_WHITE);
	setStringBorder(3);
	setStringBorderRGBA(RGBA_BLACK);

	g->drawString(0, 0, TOP | LEFT, script[language].strWords.ready);

	tex = g->getTexture();
	float w = tex->width;

	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(0, 25);
	pop->addObject(img);
	imgGameReady[0] = img;
	img->alpha = 0.0f;

	tex = createImage("Assets/Cursor/Cursor.png");
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(w + 15, 40);
	img->anc = VCENTER | HCENTER;
	pop->addObject(img);
	imgGameReady[1] = img;
	img->alpha = 0.0f;
#endif

	pop->_aniDt = 2.0f;
	pop->style = iPopupMove;
	pop->openPoint = iPointMake(20, -20);
	pop->closePoint = iPointMake(20, 40);
	pop->methodAfter = drawStaminaAfter;
	pop->methodOpen = openPopStamina;
	popStamina = pop;

	size = iSizeMake(maxStamina, 10);
	texStamina = methodBar(size, 4);
	texExhauste = methodBar(size, 5);
}

void freeStamina()
{
	delete popStamina;
	freeImage(texExhauste);
	freeImage(texStamina);
	delete imgGameReady;
}

void drawStamina(float dt)
{
	popStamina->paint(dt);
}

void drawStaminaAfter(iPopup* pop, float dt, float rate)
{
	float texStaminaWidth;

	iPoint p = currPopPosition(pop);

	if (uiDt < 2.0f)
	{
		texStaminaWidth = easeIn(uiDt / _uiDt, 0, player->stamina);
		//for (int i = 0; i < 2; i++)
		//	imgGameReady[i]->alpha = 1.0f;
		imgGameReady[1]->degree += 720 * dt;
		if (imgGameReady[1]->degree > 360)
			imgGameReady[1]->degree -= 360;
	}
	else
	{
		texStaminaWidth = player->stamina;
		for (int i = 0; i < 2; i++)
			imgGameReady[i]->alpha = 0.0f;
	}

	//drawImage(texStamina, 24, 44, TOP | LEFT, 0, 0,
	drawImage(texStamina, p.x + 4, p.y + 4, TOP | LEFT, 0, 0,
		texStaminaWidth, texStamina->height,
		1, 1, 0, 0);

	//drawImage(texExhauste, texStamina->width + 24, 44, TOP | RIGHT, 0, 0,
	drawImage(texExhauste, texStamina->width + 24, p.y + 4, TOP | RIGHT, 0, 0,
		player->exhausteRate, texStamina->height,
		1, 1, 0, 0, REVERSE_WIDTH);
}

void showPopStamina(bool show)
{
	popStamina->show(show);
}

//+++++++++++++++++++++++++++++
//+           Clock           +
//+++++++++++++++++++++++++++++
iStrTex* stClock;

#define _sizeDt 0.5f
float sizeDt;

Texture* methodClock(const char* str);

void createClock()
{
	stClock = new iStrTex(methodClock);
	stClock->setString("0");
	sizeDt = 0.0f;
}

Texture* methodClock(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(200, 40);
	g->init(size);

	setStringSize(40);
	setStringRGBA(RGBA_WHITE);
	setStringBorder(5);
	setStringBorderRGBA(RGB_GRAY, 1);
#if 0
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);
#else
	char s[2][8];// 1.24      "1"   ".24"
	s[0][0] = 0;
	s[1][0] = 0;

	int i, len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i] == '.')
		{
			memcpy(s[0], str, i);
			s[0][i] = 0;

			len -= i;
			memcpy(s[1], &str[i], len);
			s[1][len] = 0;
			break;
		}
	}
	g->drawString(size.width / 2, size.height / 2, VCENTER | RIGHT, s[0]);
	g->drawString(size.width / 2, size.height / 2, VCENTER | LEFT, s[1]);
#endif
	return g->getTexture();
}

void freeClock()
{
	delete stClock;
}

void drawClock(float dt)
{
	if (!popHpBar->bShow)
		return;

	float rx, ry;
	float clockHeight;

	if (!player->alive)
		sizeDt = 0.1f;

	if (playTime < bestTime)
	{
		rx =
		ry = 1.0f;
	}
	else// if (playTime >= bestTime)
	{
		sizeDt += dt;
		if (sizeDt > _sizeDt)
			sizeDt = 0.0f;
		if (sizeDt < 0.1f)
		{
			rx = ry = linear(sizeDt / 0.1f, 1.5, 1.0f);

			setRGBA((rand() % 100) * 0.01f, (rand() % 100) * 0.01f, (rand() % 100) * 0.01f, 1.0f);
		}
		else
			rx = ry = 1.0f;
	}

	if (popHpBar->stat == iPopupOpen)
		clockHeight = easeIn(popHpBar->aniDt / popHpBar->_aniDt, -20, 40);
	else if (popHpBar->stat == iPopupProc)
		clockHeight = 40;
	else if (popHpBar->stat == iPopupClose)
		clockHeight = easeOut(popHpBar->aniDt / popHpBar->_aniDt, 40, -20);

	stClock->paint(devSize.width / 2, clockHeight, VCENTER | HCENTER, rx, ry, "%.2f", playTime);
	setRGBA(RGBA_WHITE);
}

//+++++++++++++++++++++++++++++
//+           Curse           +
//+++++++++++++++++++++++++++++
Texture** curseNum;

void createCurse()
{
	curseNum = new Texture * [3];
	for (int i = 0; i < 3; i++)
		curseNum[i] = createImage("Assets/ProcUI/Curse.png");
}

void freeCurse()
{
	for (int i = 0; i < 3; i++)
		freeImage(curseNum[i]);
}

void drawCurse(float dt)
{
	for (int i = 0; i < player->curse; i++)
		drawImage(curseNum[i], 20 + (i * 30), 60, TOP | LEFT);
}

