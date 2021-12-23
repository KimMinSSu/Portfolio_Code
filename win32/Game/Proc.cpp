#include "Proc.h"

#include "Loading.h"
#include "Tutorial.h"
#include "Menu.h"
#include "Player.h"
#include "Enemy.h"
#include "Ui.h"
#include "Effect.h"
#include "Item.h"

#include "PopMethod.h"
#include "Script.h"
#include "Sound.h"
#include "Data.h"

int indexCloseProc;
int indexCloseProcCommon;

bool procPopShow, otherPopShow;
bool onceOver, onceHidden;

float blackDt;
float playTime;
float bestTime;
bool bPlayTime;

void loadProc()
{
	loadItem();
	loadEnemy();
	loadEffect();
	loadPlayer();
	loadUI();

	indexCloseProc = -1;
	indexCloseProcCommon = 0;

	procPopShow = false;
	otherPopShow = false;
	onceOver = false;
	onceHidden = false;
	blackDt = 0.0f;

	playTime = 0.0f;
	bestTime = getBestTime();
	bPlayTime = true;

	createPopProc();
	createProcReStart();
	createProcInfo();
	createProcSet();
	createProcGoMenu();
}

void freeProc()
{
	freeItem();
	freeEnemy();
	freeEffect();
	freePlayer();
	freeUI();

	freePopProc();
	freeProcReStart();
	freeProcInfo();
	freeProcSet();
	freeProcGoMenu();

	if (onceOver)
		freePopOver();
	if (onceHidden)
		freePopHidden();
}

void drawProc(float dt)
{
	float _dt, _playerDt;
	iPoint off = iPointZero;

	_playerDt = player->alive ? dt : 0.0f;

	extern bool bCursor;
	if (player->hp < 1 || procPopShow)
	{
		bCursor = true;
		sizeDt = 0.1f;
		_dt = 0.0f;
	}
	else
	{
		if (tutorial)
			bCursor = true;
		else
			bCursor = false;
		
		_dt = dt;
	}

	if (bPlayTime)
		playTime += _dt;

	if (player->alive)
	{
		drawItem(_dt);
		drawEnemy(_dt, off);
		drawPlayer(_playerDt, off);

		drawPopProc(dt);
		drawProcInfo(dt);
		drawProcSet(dt);
	}
	else
	{
		if (blackDt < 1.0f)
			blackDt += dt * 0.5f;

		setRGBA(0, 0, 0, blackDt);
		fillRect(-1, -1, devSize.width + 2, devSize.height + 2);
		setRGBA(RGBA_WHITE);

		if (player->move)
		{
			if (!onceOver)
			{
				if (playTime > bestTime)
				{
					bestTime = playTime;
					setBestTime(bestTime);
				}
				indexCloseProcCommon = 1;
				createPopOver();
				showPopOver(true);
				onceOver = true;
			}
			drawPopOver(dt);
		}
		else
		{
			if (!onceHidden)
			{
				playTime = 0.0f;
				indexCloseProcCommon = 2;
				createPopHidden();
				showPopHidden(true);
				onceHidden = true;
			}
			drawPopHidden(dt);
		}
	}
	drawProcReStart(dt);
	drawProcGoMenu(dt);

	drawUI(dt, _dt);
}

void keyProc(iKeyState state, iPoint mouse)
{
	if (keyPopProc(state, mouse)	||
		keyProcReStart(state, mouse)||
		keyProcInfo(state, mouse)	||
		keyProcSet(state, mouse)	||
		keyProcGoMenu(state, mouse))
		return;

	if (!player->alive)
	{
		if (onceOver)
			if (keyPopOver(state, mouse))
				return;
		if (onceHidden)
			if (keyPopHidden(state, mouse))
				return;
	}
}

//========================================
void closePopProc(iPopup* pop)
{
	if (indexCloseProc == -1)
		return;

	void (*MethodShow[4])(bool show) =
	{
		showProcReStart,	//0
		showProcInfo,		//1
		showProcSet,		//2
		showProcGoMenu,		//3
	};
	MethodShow[indexCloseProc](true);
}

void closePopProcCommon(iPopup* pop)
{
	if (indexCloseProcCommon == -1)
		return;

	void (*MethodShow[3])(bool show) =
	{
		showPopProc,	//0
		showPopOver,	//1
		showPopHidden,	//2
	};
	MethodShow[indexCloseProcCommon](true);
}

//++++++++++++++++++++++++++++++++++++++++
//+                popProc               +
//++++++++++++++++++++++++++++++++++++++++
iPopup** popProc;
iImage** imgProcBtn;
iStrTex*** stProc;
iStrTex* stPause;

void drawPopProcBefore(iPopup* pop, float dt, float rate);

void createPopProc()
{
	const char* str[6] =
	{
		script[language].strBtn.continueGame,
		script[language].strBtn.restartGame,
		script[language].strBtn.info,
		script[language].strBtn.set,
		script[language].strBtn.goMenu,
		script[language].strWords.pause
	};

	stProc = new iStrTex * *[5];
	imgProcBtn = new iImage * [5];
	popProc = new iPopup * [6];

	for (int i = 0; i < 5; i++)
	{
		iPopup* pop = new iPopup();
		stProc[i] = new iStrTex * [2];
		imgProcBtn[i] = new iImage();

		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stProc[i][j] = new iStrTex(methodProcBtn);
			stProc[i][j]->setString(str[i]);
			imgProcBtn[i]->addObject(stProc[i][j]->tex);
		}

		imgProcBtn[i]->position = iPointMake(0, i * 50);
		pop->addObject(imgProcBtn[i]);

		float h = devSize.height * 0.3f - 20;
		float w = i % 2 ? -150 : devSize.width;
		pop->style = iPopupMove;
		pop->openPoint = iPointMake(w, h);
		pop->closePoint = iPointMake(devSize.width / 2 - 100, h);
		pop->methodBefore = drawPopProcBefore;
		pop->methodClose = closePopProc;
		popProc[i] = pop;
	}

	iImage* img;
	iPopup* pop = new iPopup();

	stPause = new iStrTex(methodProcTitle);
	stPause->setString(str[5]);
	img = new iImage();
	img->addObject(stPause->tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width / 2 - 150, -70);
	pop->closePoint = iPointMake(devSize.width / 2 - 150, devSize.height * 0.15f - 10);
	popProc[5] = pop;
}

void freePopProc()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 2; j++)
			delete stProc[i][j];
		delete stProc[i];
		delete popProc[i];
	}
	delete stPause;
	delete stProc;
	delete imgProcBtn;
	delete popProc;
}

void drawPopProcBefore(iPopup* pop, float dt, float rate)
{
	const char* str[6] =
	{
		script[language].strBtn.continueGame,
		script[language].strBtn.restartGame,
		script[language].strBtn.info,
		script[language].strBtn.set,
		script[language].strBtn.goMenu,
		script[language].strWords.pause
	};

	for (int i = 0; i < 5; i++)
	{
		imgProcBtn[i]->setTexObject(i == popProc[0]->selected);
		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stProc[i][j]->setString(str[i]);
		}
	}

	stPause->setString(str[5]);
}

void drawPopProc(float dt)
{
	for (int i = 0; i < 6; i++)
	{
		popProc[i]->paint(dt);
	}

	if (tutorial)
		return;

	if (popProc[0]->bShow)
	{
		if (keyDown == keysESC)
		{
			indexCloseProc = -1;
			procPopShow = false;
			showPopProc(false);
		}
	}
	else
	{
		if (!otherPopShow && keyDown == keysESC)
		{
			procPopShow = true;
			showPopProc(true);
		}
	}
}

void showPopProc(bool show)
{
	for (int i = 0; i < 6; i++)
		popProc[i]->show(show);
}

bool keyPopProc(iKeyState state, iPoint mouse)
{
	if (popProc[0]->bShow == false)
		return false;
	if (popProc[0]->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		if (popProc[0]->selected == -1)
			break;

		indexCloseProc = -1;
		if (popProc[0]->selected == 0)
		{
			procPopShow = false;
			popProc[0]->selected = -1;
		}
		else if (popProc[0]->selected == 1)
		{
			indexCloseProc = 0;
			otherPopShow = true;
			popProc[0]->selected = -1;
		}
		else if (popProc[0]->selected == 2)
		{
			indexCloseProc = 1;
			otherPopShow = true;
			popProc[0]->selected = -1;
		}
		else if (popProc[0]->selected == 3)
		{
			indexCloseProc = 2;
			otherPopShow = true;
			popProc[0]->selected = -1;
		}
		else if (popProc[0]->selected == 4)
		{
			indexCloseProc = 3;
			otherPopShow = true;
			popProc[0]->selected = -1;
		}
		showPopProc(false);
		break;
	case iKeyStateMoved:
		for (i = 0; i < 5; i++)
		{
			if (containPoint(mouse, imgProcBtn[i]->touchRect(popProc[i]->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (popProc[0]->selected != j)
		{
			playSound(0);
			popProc[0]->selected = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+              ProcReStart             +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popProcReStart;
iImage** imgProcReStartBtn;
iStrTex* stProcReStart;

void drawProcReStartBefore(iPopup* pop, float dt, float rate);

void createProcReStart()
{
	iImage* img; iSize size;
	iPopup* pop = new iPopup();

	stProcReStart = new iStrTex(methodRestart);
	stProcReStart->setString("%d", language);
	img = new iImage();
	img->addObject(stProcReStart->tex);
	pop->addObject(img);

	const char* strBtn[2] =
	{
		btn.o,
		btn.x
	};

	float w = devSize.width * 0.8f;
	float h = devSize.height * 0.8f;
	iPoint positionBtn[2] = {
		{w / 2 - 50 - 20, h / 2 + 60 - 20},
		{w / 2 + 50 - 20, h / 2 + 60 - 20},
	};

	size = iSizeMake(40, 40);
	imgProcReStartBtn = methodStrBtn(strBtn, positionBtn, size, 2);
	for (int i = 0; i < 2; i++)
		pop->addObject(imgProcReStartBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawProcReStartBefore;
	pop->methodClose = closePopProcCommon;
	popProcReStart = pop;
}

void freeProcReStart()
{
	delete popProcReStart;
	delete imgProcReStartBtn;
	delete stProcReStart;
}

void drawProcReStartBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgProcReStartBtn[i]->setTexObject(i == popProcReStart->selected);

	stProcReStart->setString("%d", language);
}

void drawProcReStart(float dt)
{
	popProcReStart->paint(dt);
}

void showProcReStart(bool show)
{
	popProcReStart->show(show);
}

bool keyProcReStart(iKeyState state, iPoint mouse)
{
	if (popProcReStart->bShow == false)
		return false;
	if (popProcReStart->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		i = popProcReStart->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			setLoading(gs_proc, freeProc, loadProc);
		}
		else// if (i == 1)
		{
			showProcReStart(false);
			indexCloseProc = -1;
			otherPopShow = false;
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(mouse, imgProcReStartBtn[i]->touchRect(popProcReStart->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popProcReStart->selected != j)
		{
			playSound(0);
			popProcReStart->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               GameInfo               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popProcInfo;
iImage** imgProcInfoBtn;
iStrTex* stProcInfo;
Texture** procEnemySample;
Texture** procItemSample;

void drawProcInfoBefore(iPopup* pop, float dt, float rate);

void createProcInfo()
{
	pageInfo = 0;
	iImage* img; iSize size;
	iPopup* pop = new iPopup();

	stProcInfo = new iStrTex(methodInfo);
	stProcInfo->setString("%d", pageInfo);
	img = new iImage();
	img->addObject(stProcInfo->tex);
	pop->addObject(img);

	const char* strBtn[3] = {
		btn.left,
		btn.right,
		btn.x,
	};

	iPoint positionBtn[3] = {
		{0, devSize.height * 0.8 - 45},
		{devSize.width * 0.8 - 40, devSize.height * 0.8 - 45},
		{devSize.width * 0.8 - 40, 0}
	};

	size = iSizeMake(40, 40);
	imgProcInfoBtn = methodStrBtn(strBtn, positionBtn, size, 3);
	for (int i = 0; i < 3; i++)
		pop->addObject(imgProcInfoBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawProcInfoBefore;
	pop->methodClose = closePopProcCommon;
	popProcInfo = pop;

	procEnemySample = new Texture * [6];
	for (int i = 0; i < 6; i++)
		procEnemySample[i] = createBall2(10);
	procItemSample = new Texture * [2];
	for (int i = 0; i < 2; i++)
		procItemSample[i] = createImage("Assets/Potion/Potion%d.png", i);
}

void freeProcInfo()
{
	for (int i = 0; i < 6; i++)
		freeImage(procEnemySample[i]);
	delete procEnemySample;
	for (int i = 0; i < 2; i++)
		freeImage(procItemSample[i]);
	delete procItemSample;
	delete popProcInfo;
	delete stProcInfo;
	delete imgProcInfoBtn;
}

void drawProcInfoBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 3; i++)
		imgProcInfoBtn[i]->setTexObject(popProcInfo->selected == i);
	stProcInfo->setString("%d %d", pageInfo, language);

	float w = devSize.width * 0.8f * 0.1f;
	iPoint closeP = iPointMake(pop->closePoint.x + w, pop->closePoint.y);
	iPoint openP = iPointMake(pop->openPoint.x + w, pop->openPoint.y);
	iPoint p = pop->stat != iPopupClose ? closeP : linear(rate, closeP, openP);
	if (pageInfo == 2)
	{
		for (int i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:setRGBA(RGB_RED, 1.0f); break;
			case 1:setRGBA(RGB_SLOW, 1.0f); break;
			case 2:setRGBA(RGBA_WHITE); break;
			case 3:setRGBA(RGB_POISON, 1.0f); break;
			case 4:setRGBA(RGB_CURSE, 1.0f); break;
			case 5:setRGBA(RGB_EXHAUSTE, 1.0f); break;
			}

			drawImage(procEnemySample[i], p.x, p.y + 90 + 40 * i, VCENTER | LEFT);
		}
	}
	else if (pageInfo == 3)
	{
		drawImage(procItemSample[0], p.x, p.y + 70, TOP | LEFT, 0, 0,
			procItemSample[0]->width, procItemSample[0]->height, 0.5f, 0.5f, 0, 0);
		drawImage(procItemSample[1], p.x, p.y + 190, TOP | LEFT, 0, 0,
			procItemSample[1]->width, procItemSample[1]->height, 0.5f, 0.5f, 0, 0);
	}
}

void drawProcInfo(float dt)
{
	popProcInfo->paint(dt);
}

void showProcInfo(bool show)
{
	popProcInfo->show(show);
	if (show)
		pageInfo = 0;
}

bool keyProcInfo(iKeyState state, iPoint mouse)
{
	if (popProcInfo->bShow == false)
		return false;
	if (popProcInfo->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state) {

	case iKeyStateBegan:
		i = popProcInfo->selected;
		if (i == -1) break;

		if (i == 0)
		{
			playSound(3);
			pageInfo--;
			if (pageInfo < 0)
				pageInfo = _pageInfo - 1;
		}
		else if (i == 1)
		{
			playSound(3);
			pageInfo++;
			if (pageInfo > _pageInfo - 1)
				pageInfo = 0;
		}
		else if (i == 2)
		{
			showProcInfo(false);
			popProcInfo->selected = -1;
			otherPopShow = false;
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 3; i++)
		{
			if (containPoint(mouse, imgProcInfoBtn[i]->touchRect(popProcInfo->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popProcInfo->selected != j)
		{
			playSound(0);
			popProcInfo->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               ProcSet                +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popProcSet;
iImage** imgProcSetBtn;
iStrTex* stProcSet;

extern bool isFullscreen;
extern int filterNum;
void drawProcSetBefore(iPopup* pop, float dt, float rate);

void createProcSet()
{
	iImage* img; iSize size;
	iPopup* pop = new iPopup();

	stProcSet = new iStrTex(methodSet);
	stProcSet->setString("%d %d %d %d", language, onoffBGM, onoffSFX, isFullscreen, filterNum);
	img = new iImage();
	img->addObject(stProcSet->tex);
	pop->addObject(img);

	const char* strBtn[11] = {
		btn.left,
		btn.right,
		btn.left,
		btn.right,
		btn.left,
		btn.right,
		btn.left,
		btn.right,
		btn.left,
		btn.right,
		btn.x
	};

	float w = devSize.width * 0.8f;
	float h = devSize.height * 0.8f * 0.2f;
	size = iSizeMake(30, 50);
	iPoint positionBtn[11] = {
		{w * 0.5f - 15, h - 10},
		{w * 0.75f - 15, h - 10},
		{w * 0.5f - 15, h + 50 - 10},
		{w * 0.75f - 15, h + 50 - 10},
		{w * 0.5f - 15, h + 100 - 10},
		{w * 0.75f - 15, h + 100 - 10},
		{w * 0.5f - 15, h + 150 - 10},
		{w * 0.75f - 15, h + 150 - 10},
		{w * 0.5f - 15, h + 200 - 10},
		{w * 0.75f - 15, h + 200 - 10},
		{w - 40, 0}
	};

	imgProcSetBtn = methodStrBtn(strBtn, positionBtn, size, 11);
	for (int i = 0; i < 11; i++)
		pop->addObject(imgProcSetBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1, devSize.height * 0.1);
	pop->methodBefore = drawProcSetBefore;
	pop->methodClose = closePopProcCommon;
	popProcSet = pop;
}

void freeProcSet()
{
	delete popProcSet;
	delete stProcSet;
	delete imgProcSetBtn;
}

void drawProcSetBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 11; i++)
		imgProcSetBtn[i]->setTexObject(i == popProcSet->selected);

	stProcSet->setString("%d %d %d %d %d",
		language, onoffBGM, onoffSFX, isFullscreen, filterNum);
}

void drawProcSet(float dt)
{
	popProcSet->paint(dt);
}

void showProcSet(bool show)
{
	popProcSet->show(show);
}

extern void goFullscreen();
bool keyProcSet(iKeyState state, iPoint mouse)
{
	if (popProcSet->bShow == false)
		return false;
	if (popProcSet->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		i = popProcSet->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			goFullscreen();
		}
		else if (i == 1)
		{
			goFullscreen();
		}
		else if (i == 2)
		{
			language--;
			if (language < 0)
				language = LanguageMax - 1;
		}
		else if (i == 3)
		{
			language++;
			if (language > LanguageMax - 1)
				language = 0;
		}
		else if (i == 4)
		{
			onoffBGM--;
			if (onoffBGM < 0)
				onoffBGM = 1;

			if (onoffBGM == 1)
				audioPlay(1);
			else if (onoffBGM == 0)
				audioStop(1);
		}
		else if (i == 5)
		{
			onoffBGM++;
			if (onoffBGM > 1)
				onoffBGM = 0;

			if (onoffBGM == 1)
				audioPlay(1);
			else if (onoffBGM == 0)
				audioStop(1);
		}
		else if (i == 6)
		{
			onoffSFX--;
			if (onoffSFX < 0)
				onoffSFX = 1;
		}
		else if (i == 7)
		{
			onoffSFX++;
			if (onoffSFX > 1)
				onoffSFX = 0;
		}
		else if (i == 8)
		{
			filterNum--;
			if (filterNum < 0)
				filterNum = 2;
		}
		else if (i == 9)
		{
			filterNum++;
			if (filterNum > 2)
				filterNum = 0;
		}
		else if (i == 10)
		{
			showProcSet(false);
			popProcSet->selected = -1;
			otherPopShow = false;
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 11; i++)
		{
			if (containPoint(mouse, imgProcSetBtn[i]->touchRect(popProcSet->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popProcSet->selected != j)
		{
			playSound(0);
			popProcSet->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;

	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               ProcGoMenu             +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popProcGoMenu;
iImage** imgProcGoMenuBtn;
iStrTex* stMenuProcGoMenu;

void drawProcGoMenuBefore(iPopup* pop, float dt, float rate);

void createProcGoMenu()
{
	iImage* img; iSize size;
	iPopup* pop = new iPopup();

	stMenuProcGoMenu = new iStrTex(methodGoMenu);
	stMenuProcGoMenu->setString("%d", language);
	img = new iImage();
	img->addObject(stMenuProcGoMenu->tex);
	pop->addObject(img);

	const char* strBtn[2] =
	{
		btn.o,
		btn.x
	};

	float w = devSize.width * 0.8f;
	float h = devSize.height * 0.8f;
	iPoint positionBtn[2] = {
		{w / 2 - 50 - 20, h / 2 + 60 - 20},
		{w / 2 + 50 - 20, h / 2 + 60 - 20},
	};

	size = iSizeMake(40, 40);

	imgProcGoMenuBtn = methodStrBtn(strBtn, positionBtn, size, 2);
	for (int i = 0; i < 2; i++)
		pop->addObject(imgProcGoMenuBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawProcGoMenuBefore;
	pop->methodClose = closePopProcCommon;
	popProcGoMenu = pop;
}

void freeProcGoMenu()
{
	delete popProcGoMenu;
	delete imgProcGoMenuBtn;
	delete stMenuProcGoMenu;
}

void drawProcGoMenuBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgProcGoMenuBtn[i]->setTexObject(i == popProcGoMenu->selected);

	stMenuProcGoMenu->setString("%d", language);
}

void drawProcGoMenu(float dt)
{
	popProcGoMenu->paint(dt);
}

void showProcGoMenu(bool show)
{
	popProcGoMenu->show(show);
}

bool keyProcGoMenu(iKeyState state, iPoint mouse)
{
	if (popProcGoMenu->bShow == false)
		return false;
	if (popProcGoMenu->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		i = popProcGoMenu->selected;
		if (i == -1)
			break;

		if (i == 0)
		{
			setLoading(gs_menu, freeProc, loadMenu);
		}
		else// if (i == 1)
		{
			popProcGoMenu->selected = -1;
			otherPopShow = false;
			showProcGoMenu(false);
		}
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(mouse, imgProcGoMenuBtn[i]->touchRect(popProcGoMenu->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popProcGoMenu->selected != j)
		{
			playSound(0);
			popProcGoMenu->selected = j;
		}
		break;

	case iKeyStateEnded:
		break;

	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               popOver                +
//++++++++++++++++++++++++++++++++++++++++
iPopup** popOver;
iImage** imgOverBtn;
iStrTex*** stOverBtn;
iStrTex* stOverTime;

void drawPopOverBefore(iPopup* pop, float dt, float rate);

void createPopOver()
{
	const char* str[2] =
	{
		script[language].strBtn.restartGame,
		script[language].strBtn.goMenu,
	};

	stOverBtn = new iStrTex * *[2];
	imgOverBtn = new iImage * [2];
	popOver = new iPopup * [3];

	for (int i = 0; i < 2; i++)
	{
		iPopup* pop = new iPopup();
		stOverBtn[i] = new iStrTex * [2];
		imgOverBtn[i] = new iImage();

		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stOverBtn[i][j] = new iStrTex(methodProcBtn);
			stOverBtn[i][j]->setString(str[i]);
			imgOverBtn[i]->addObject(stOverBtn[i][j]->tex);
		}
		imgOverBtn[i]->position = iPointMake(0, i * 50);
		pop->addObject(imgOverBtn[i]);

		float h = devSize.height / 2;
		float w = i % 2 ? -150 : devSize.width;
		pop->style = iPopupMove;
		pop->openPoint = iPointMake(w, h);
		pop->closePoint = iPointMake(devSize.width / 2 - 100, h);
		pop->methodBefore = drawPopOverBefore;
		pop->methodClose = closePopProc;
		popOver[i] = pop;
	}

	iImage* img;
	iPopup* pop = new iPopup();

	stOverTime = new iStrTex(methodOver);
	stOverTime->setString("%d", language);
	img = new iImage();
	img->addObject(stOverTime->tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, - 100);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawPopOverBefore;
	pop->methodClose = closePopProc;
	popOver[2] = pop;
}

void freePopOver()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			delete stOverBtn[i][j];
		delete stOverBtn[i];
		delete popOver[i];
	}
	delete stOverTime;
	delete stOverBtn;
	delete imgOverBtn;
	delete popOver;
}

void drawPopOverBefore(iPopup* pop, float dt, float rate)
{
	const char* str[2] =
	{
		script[language].strBtn.restartGame,
		script[language].strBtn.goMenu,
	};

	for (int i = 0; i < 2; i++)
	{
		imgOverBtn[i]->setTexObject(i == popOver[0]->selected);
		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stOverBtn[i][j]->setString(str[i]);
		}
	}

	stOverTime->setString("%d", language);
}

void drawPopOver(float dt)
{
	for (int i = 0; i < 3; i++)
		popOver[i]->paint(dt);
}

void showPopOver(bool show)
{
	for (int i = 0; i < 3; i++)
	{
		popOver[i]->show(show);
	}
}

bool keyPopOver(iKeyState state, iPoint mouse)
{
	if (popOver[0]->bShow == false)
		return false;
	if (popOver[0]->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		if (popOver[0]->selected == -1)
			break;

		if (popOver[0]->selected == 0)
		{
			indexCloseProc = 0;
			popOver[0]->selected = -1;
		}
		else if (popOver[0]->selected == 1)
		{
			indexCloseProc = 3;
			popOver[0]->selected = -1;
		}
		showPopOver(false);
		break;
	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(mouse, imgOverBtn[i]->touchRect(popOver[i]->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (popOver[0]->selected != j)
		{
			playSound(0);
			popOver[0]->selected = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               popHidden              +
//++++++++++++++++++++++++++++++++++++++++
iPopup** popHidden;
iImage** imgHiddenBtn;
iStrTex*** stHiddenBtn;
iStrTex* stHidden;

void drawPopHiddenBefore(iPopup* pop, float dt, float rate);

void createPopHidden()
{
	const char* str[2] =
	{
		script[language].strBtn.restartGame,
		script[language].strBtn.goMenu,
	};

	stHiddenBtn = new iStrTex * *[2];
	imgHiddenBtn = new iImage * [2];
	popHidden = new iPopup * [3];

	for (int i = 0; i < 2; i++)
	{
		iPopup* pop = new iPopup();
		stHiddenBtn[i] = new iStrTex * [2];
		imgHiddenBtn[i] = new iImage();

		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stHiddenBtn[i][j] = new iStrTex(methodProcBtn);
			stHiddenBtn[i][j]->setString(str[i]);
			imgHiddenBtn[i]->addObject(stHiddenBtn[i][j]->tex);
		}
		imgHiddenBtn[i]->position = iPointMake(0, i * 50);
		pop->addObject(imgHiddenBtn[i]);

		float h = devSize.height * 0.4f;
		float w = i % 2 ? -150 : devSize.width;
		pop->style = iPopupMove;
		pop->openPoint = iPointMake(w, h);
		pop->closePoint = iPointMake(devSize.width / 2 - 100, h);
		pop->methodBefore = drawPopHiddenBefore;
		pop->methodClose = closePopProc;
		popHidden[i] = pop;
	}

	iImage* img;
	iPopup* pop = new iPopup();

	stHidden = new iStrTex(methodHidden);
	stHidden->setString("%d", language);
	img = new iImage();
	img->addObject(stHidden->tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, -100);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawPopHiddenBefore;
	pop->methodClose = closePopProc;
	popHidden[2] = pop;
}

void freePopHidden()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
			delete stHiddenBtn[i][j];
		delete stHiddenBtn[i];
		delete popHidden[i];
	}
	delete stHidden;
	delete stHiddenBtn;
	delete imgHiddenBtn;
	delete popHidden;
}

void drawPopHiddenBefore(iPopup* pop, float dt, float rate)
{
	const char* str[2] =
	{
		script[language].strBtn.restartGame,
		script[language].strBtn.goMenu,
	};

	for (int i = 0; i < 2; i++)
	{
		imgHiddenBtn[i]->setTexObject(i == popHidden[0]->selected);
		for (int j = 0; j < 2; j++)
		{
			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			stHiddenBtn[i][j]->setString(str[i]);
		}
	}

	stHidden->setString("%d", language);
}

void drawPopHidden(float dt)
{
	for (int i = 0; i < 3; i++)
		popHidden[i]->paint(dt);
}

void showPopHidden(bool show)
{
	for (int i = 0; i < 3; i++)
	{
		popHidden[i]->show(show);
	}
}

bool keyPopHidden(iKeyState state, iPoint mouse)
{
	if (popHidden[0]->bShow == false)
		return false;
	if (popHidden[0]->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		if (popHidden[0]->selected == -1)
			break;

		if (popHidden[0]->selected == 0)
		{
			indexCloseProc = 0;
			popHidden[0]->selected = -1;
		}
		else if (popHidden[0]->selected == 1)
		{
			indexCloseProc = 3;
			popHidden[0]->selected = -1;
		}
		showPopHidden(false);
		break;
	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(mouse, imgHiddenBtn[i]->touchRect(popHidden[i]->closePoint)))
			{
				j = i;
				break;
			}
		}

		if (popHidden[0]->selected != j)
		{
			playSound(0);
			popHidden[0]->selected = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}

	return true;
}
