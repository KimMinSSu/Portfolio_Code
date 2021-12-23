#include "Ui.h"

#include "Proc.h"
#include "Player.h"
#include "Script.h"
#include "PopMethod.h"
#include "Sound.h"

#define _uiDt 2.0f
float uiDt;

void loadUI()
{
	createHpBar();
	createStamina();
	createClock();
	createCurse();
	createJoystick();
	createPauseBtn();
	uiDt = 0.0f;
}

void freeUI()
{
	freeHpBar();
	freeStamina();
	freeClock();
	freeCurse();
	freeJoystick();
	freePauseBtn();
}

void drawUI(float dt, float _dt)
{
	drawHpBar(dt);
	drawStamina(dt);
	drawClock(_dt);
	drawCurse(dt);
	drawJoystick(dt);
	drawPauseBtn(dt);
}

//+++++++++++++++++++++++++++++
//+          HpBar            +
//+++++++++++++++++++++++++++++
iPopup* popHpBar;
Texture* texHpBar;

void drawHpBarAfter(iPopup* pop, float dt, float rate);

#define hpBarMax 200.0f * SIZE_RATE
#define hpRate 50.0f * SIZE_RATE

void createHpBar()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(hpBarMax + 8 * SIZE_RATE, 18 * SIZE_RATE);
	tex = methodBar(size, 0);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(hpBarMax, 10 * SIZE_RATE);
	tex = methodBar(size, 1);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4 * SIZE_RATE, 4 * SIZE_RATE);
	pop->addObject(img);

	pop->_aniDt = 2.0f;
	pop->style = iPopupMove;
	pop->openPoint = iPointMake(20 * SIZE_RATE, -40 * SIZE_RATE);
	pop->closePoint = iPointMake(20 * SIZE_RATE, 20 * SIZE_RATE);
	pop->methodAfter = drawHpBarAfter;
	popHpBar = pop;

	size = iSizeMake(hpBarMax, 10 * SIZE_RATE);
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
		drawImage(texHpBar, p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE, TOP | LEFT, 0, 0,
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
		drawImage(texHpBar, p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE, TOP | LEFT, 0, 0,
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

	size = iSizeMake(hpBarMax + 8 * SIZE_RATE, 18 * SIZE_RATE);
	tex = methodBar(size, 0);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(maxStamina * SIZE_RATE, 10 * SIZE_RATE);
	tex = methodBar(size, 2);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4 * SIZE_RATE, 4 * SIZE_RATE);
	pop->addObject(img);

	size = iSizeMake(maxStamina * SIZE_RATE, 10 * SIZE_RATE);
	tex = methodBar(size, 2);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4 * SIZE_RATE, 4 * SIZE_RATE);
	pop->addObject(img);

#if 1
	imgGameReady = new iImage * [2];

	iGraphics* g = iGraphics::share();
	size = iSizeMake(200 * SIZE_RATE, 30 * SIZE_RATE);
	g->init(size);

	setStringSize(20 * SIZE_RATE);
	setStringRGBA(RGBA_WHITE);
	setStringBorder(3 * SIZE_RATE);
	setStringBorderRGBA(RGBA_BLACK);

	g->drawString(0, 0, TOP | LEFT, script[language].strWords.ready);

	tex = g->getTexture();
	float readyW = tex->width;

	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(0, 25 * SIZE_RATE);
	pop->addObject(img);
	imgGameReady[0] = img;
	img->alpha = 0.0f;

	tex = createImage("Assets/Cursor/Cursor.png");
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(readyW + 15 * SIZE_RATE, 40 * SIZE_RATE);
	img->anc = VCENTER | HCENTER;
	pop->addObject(img);
	imgGameReady[1] = img;
	img->alpha = 0.0f;
#endif

	pop->_aniDt = 2.0f;
	pop->style = iPopupMove;
	pop->openPoint = iPointMake(20 * SIZE_RATE, -20 * SIZE_RATE);
	pop->closePoint = iPointMake(20 * SIZE_RATE, 40 * SIZE_RATE);
	pop->methodAfter = drawStaminaAfter;
	pop->methodOpen = openPopStamina;
	popStamina = pop;

	size = iSizeMake(maxStamina * SIZE_RATE, 10 * SIZE_RATE);
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
		texStaminaWidth = easeIn(uiDt / _uiDt, 0, player->stamina * SIZE_RATE);
		//for (int i = 0; i < 2; i++)
		//	imgGameReady[i]->alpha = 1.0f;
		imgGameReady[1]->degree += 720 * dt;
		if (imgGameReady[1]->degree > 360)
			imgGameReady[1]->degree -= 360;
	}
	else
	{
		texStaminaWidth = player->stamina * SIZE_RATE;
		for (int i = 0; i < 2; i++)
			imgGameReady[i]->alpha = 0.0f;
	}

	//drawImage(texStamina, 24, 44, TOP | LEFT, 0, 0,
	drawImage(texStamina, p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE, TOP | LEFT, 0, 0,
			  texStaminaWidth, texStamina->height,
			  1, 1, 0, 0);

	//drawImage(texExhauste, texStamina->width + 24, 44, TOP | RIGHT, 0, 0,
	drawImage(texExhauste, texStamina->width + 24 * SIZE_RATE, p.y + 4 * SIZE_RATE, TOP | RIGHT, 0, 0,
			  player->exhausteRate * SIZE_RATE, texStamina->height,
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
	iSize size = iSizeMake(200 * SIZE_RATE, 40 * SIZE_RATE);
	g->init(size);

	setStringSize(40 * SIZE_RATE);
	setStringRGBA(RGBA_WHITE);
	setStringBorder(5 * SIZE_RATE);
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
		clockHeight = easeIn(popHpBar->aniDt / popHpBar->_aniDt, -20 * SIZE_RATE, 40 * SIZE_RATE);
	else if (popHpBar->stat == iPopupProc)
		clockHeight = 40 * SIZE_RATE;
	else if (popHpBar->stat == iPopupClose)
		clockHeight = easeOut(popHpBar->aniDt / popHpBar->_aniDt, 40 * SIZE_RATE, -20 * SIZE_RATE);

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
		drawImage(curseNum[i], 20 + (i * 30 * SIZE_RATE), 60 * SIZE_RATE, TOP | LEFT, 0, 0,
			curseNum[i]->width, curseNum[i]->height, 1.5f, 1.5f, 0, 0);
}

//+++++++++++++++++++++++++++++
//+          Joystick         +
//+++++++++++++++++++++++++++++
iPopup* popJoyStick;
iImage** imgJoyStick;

void drawJoystickBefore(iPopup* pop, float dt, float rate);

void createJoystick()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();
	imgJoyStick = new iImage*[3];

	iGraphics* g = iGraphics::share();
	size = iSizeMake(200, 200);
	g->init(size);

	setRGBA(1,1,1,0.5f);
	g->fillRect(0, 0, 200, 200, 100);
	setRGBA(RGBA_WHITE);
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	img->position = iPointMake(50, 0);
	freeImage(tex);
	pop->addObject(img);
	imgJoyStick[0] = img;

	size = iSizeMake(150, 150);
	g->init(size);
	setRGBA(RGB_BLUE,0.5f);
	g->fillRect(0, 0, 150, 150, 75);
	setRGBA(RGBA_WHITE);
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	img->position = iPointMake(75, 25);
	freeImage(tex);
	pop->addObject(img);
	imgJoyStick[1] = img;

	img = new iImage();
	size = iSizeMake(200, 200);
	for(int i = 0; i < 2; i++)
	{
		g->init(size);
		if (i == 0) setRGBA(RGB_YELLOW,0.5f);
		else setRGBA(RGB_GREEN, 0.5f);
		g->fillRect(0, 0, 200, 200, 100);
		setRGBA(RGBA_WHITE);
		tex = g->getTexture();
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(devSize.width - 250, 0);
	pop->addObject(img);
	imgJoyStick[2] = img;

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(0, devSize.height);
	pop->closePoint = iPointMake(0, devSize.height - 250);
	pop->methodBefore = drawJoystickBefore;
	popJoyStick = pop;
}

void freeJoystick()
{
	delete imgJoyStick;
	delete popJoyStick;
}

void drawJoystickBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgJoyStick[2]->setTexObject(i == popJoyStick->selected);
}

void drawJoystick(float dt)
{
	popJoyStick->paint(dt);
}

void showJoystick(bool show)
{
	popJoyStick->show(show);
}

void keyJoystick(iKeyState state, iPoint touchP)
{
	if (popJoyStick == NULL)
		return;

	if (popJoyStick->bShow == false)
		return;
	if (popJoyStick->stat != iPopupProc)
		return;

	popJoyStick->selected = 0;
	switch (state)
	{
		case iKeyStateMoved:
			player->run = false;
			if (containPoint(touchP, imgJoyStick[1]->touchRect(popJoyStick->closePoint)))
			{
				iPoint t = touchP - popJoyStick->closePoint - 75;
				iPoint c = imgJoyStick[0]->position + 25;

				iPoint d = t - c;
				player->vec = iPointVector(d);
				if (d.x > 75)
					t.x = c.x + 75;
				else if (d.x < -75)
					t.x = c.x - 75;

				if (d.y > 75)
					t.y = c.y + 75;
				else if (d.y < -75)
					t.y = c.y - 75;

				imgJoyStick[1]->position = t;
                player->move = true;
			}
			else
			{
				imgJoyStick[1]->position = imgJoyStick[0]->position + 25;
				player->vec = iPointZero;
			}

			break;

		case iKeyStateEnded:
			imgJoyStick[1]->position = imgJoyStick[0]->position + 25;
			player->vec = iPointZero;
			break;
	}
}

void keyJoystick(iKeyState state, int touchNum, iPoint* touchP)
{
	if (popJoyStick == NULL)
		return;

	if (popJoyStick->bShow == false)
		return;
	if (popJoyStick->stat != iPopupProc)
		return;

	switch (state)
	{
		case iKeyStateMoved:
			if (containPoint(touchP[0], imgJoyStick[1]->touchRect(popJoyStick->closePoint)))
			{
				iPoint t = touchP[0] - popJoyStick->closePoint - 75;
				iPoint c = imgJoyStick[0]->position + 25;

				iPoint d = t - c;
				player->vec = iPointVector(d);
				if (d.x > 75)
					t.x = c.x + 75;
				else if (d.x < -75)
					t.x = c.x - 75;

				if (d.y > 75)
					t.y = c.y + 75;
				else if (d.y < -75)
					t.y = c.y - 75;

				imgJoyStick[1]->position = t;
                player->move = true;
			}
			else
			{
				imgJoyStick[1]->position = imgJoyStick[0]->position + 25;
				player->vec = iPointZero;
			}

			if (containPoint(touchP[1], imgJoyStick[2]->touchRect(popJoyStick->closePoint)))
			{
				popJoyStick->selected = 1;
				player->run = true;
			}
			else
				player->run = false;

			break;
	}
}

//+++++++++++++++++++++++++++++
//+          PauseBtn         +
//+++++++++++++++++++++++++++++
iPopup* popPauseBtn;
iImage* imgPauseBtn;

void drawPauseBtnBefore(iPopup* pop, float dt, float rate);

void createPauseBtn()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(150 * SIZE_RATE, 50 * SIZE_RATE);
	imgPauseBtn = methodStrBtn(script[language].strWords.pause, iPointZero, size);
	pop->addObject(imgPauseBtn);

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(devSize.width - 150 * SIZE_RATE, -50 * SIZE_RATE);
	pop->closePoint = iPointMake(devSize.width - 150 * SIZE_RATE, 0);
	pop->methodBefore = drawPauseBtnBefore;
	popPauseBtn = pop;
}

void freePauseBtn()
{
	delete popPauseBtn;
}

void drawPauseBtnBefore(iPopup* pop, float dt, float rate)
{
    imgPauseBtn->setTexObject(0 == popPauseBtn->selected);
}

void drawPauseBtn(float dt)
{
	popPauseBtn->paint(dt);
}

void showPauseBtn(bool show)
{
	popPauseBtn->show(show);
}

bool keyPauseBtn(iKeyState state, iPoint touchP)
{
	if (popPauseBtn->bShow == false)
		return false;
	if (popPauseBtn->stat != iPopupProc)
		return true;

	if (otherPopShow == true)
	    return false;

	int i, j = -1;

	switch (state)
	{
		case iKeyStateBegan:
			break;

		case iKeyStateMoved:
			for (i = 0; i < 2; i++)
			{
				if (containPoint(touchP, imgPauseBtn->touchRect(popPauseBtn->closePoint)))
				{
					j = i;
					break;
				}
			}
			if (popPauseBtn->selected != j)
			{
                playSfxSound(0);
				popPauseBtn->selected = j;
			}
			break;

		case iKeyStateEnded:
			i = popPauseBtn->selected;
			if (i == -1)
				break;

			if (i == 0)
			{
				if(!popProcBtn[0]->bShow && !procPopShow)
				{
					procPopShow = true;
					showPopProc(true);
				}
				else if(popProcBtn[0]->bShow && procPopShow)
				{
					procPopShow = false;
					showPopProc(false);
				}
			}
			popPauseBtn->selected = -1;
			break;
	}

	return true;
}

bool keyPauseBtn(iKeyState state, int touchNum, iPoint* touchP)
{
	if (popPauseBtn->bShow == false)
		return false;
	if (popPauseBtn->stat != iPopupProc)
		return true;

	if (otherPopShow == true)
		return false;

	int i, j = -1;

	switch (state)
	{
		case iKeyStateBegan:
			break;

		case iKeyStateMoved:
			for(int k = 0; k < touchNum; k++)
			{
				for (i = 0; i < 2; i++)
				{
					if (containPoint(touchP[k], imgPauseBtn->touchRect(popPauseBtn->closePoint)))
					{
						j = i;
						break;
					}
				}
			}
			if (popPauseBtn->selected != j)
			{
				playSfxSound(0);
				popPauseBtn->selected = j;
			}
			break;

		case iKeyStateEnded:
			i = popPauseBtn->selected;
			if (i == -1)
				break;

			if (i == 0)
			{
				if(!popProcBtn[0]->bShow && !procPopShow)
				{
					procPopShow = true;
					showPopProc(true);
				}
				else if(popProcBtn[0]->bShow && procPopShow)
				{
					procPopShow = false;
					showPopProc(false);
				}
			}
			popPauseBtn->selected = -1;
			break;
	}

	return true;
}
