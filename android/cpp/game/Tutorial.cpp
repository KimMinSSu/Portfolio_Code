#include "Tutorial.h"

#include "Intro.h"
#include "Menu.h"
#include "Loading.h"

#include "Data.h"
#include "Script.h"
#include "PopMethod.h"
#include "Sound.h"

#include "Proc.h"
#include "Player.h"
#include "Enemy.h"
#include "Ui.h"

Tutorial* tutorial = NULL;
bool saveTuto;
bool startTuto;

void loadTutorial()
{
	saveTuto = getTutoclear();
	startTuto = false;

	if (!saveTuto)
		tutorial = new Tutorial();
	else
		tutorial = NULL;
}

void saveTutorial()
{
	setTutoclear(saveTuto);
	//delete saveTuto;
}

void drawTutorial(float dt)
{
	if (tutorial)
		tutorial->paint(dt);
}

void enforceRunTutorial()
{
	tutorial = new Tutorial();
	tutorial->chapter = 2;
}

void freeTutorial()
{
	if (tutorial)
		delete tutorial;
}

bool keyTutorial(iKeyState state, iPoint p)
{
	if (tutorial)
	{
		keyJoystick(state, p);
		return tutorial->key(state, p);
	}

	return false;
}

bool keyTutorial(iKeyState state, int touchNum, iPoint* p)
{
	if (tutorial)
	{
		keyJoystick(state, touchNum, p);
		return tutorial->key(state, p[0]);
	}

    return false;
}

void setPlayTime(bool stop)
{
	if (stop)
	{
		sizeDt = 0.1f;
		playTime = 0.0f;
		bPlayTime = false;
		appearEnemyDt = 0.0f;// 몬스터 안나타나게 하기
	}
	else
	{
		bPlayTime = true;
	}
}

Tutorial::Tutorial()
{
	chapter = 0;

	memset(flag, 0x00, sizeof(bool) * 4);
	flagDt = 0.0f;

	tryEnd = false;
}

Tutorial::~Tutorial()
{
	tutorial = NULL;

	freeTutoEnd();
	freeTutoNum();
	freeTutoSkip();
	freeClearScript();
	freeTutoBar();
	freeTutoScript();
	freePopTuto();
}

void Tutorial::paint(float dt)
{
	if (gameState != gs_proc)
		return;

	switch (chapter)
	{
		case 0:
			startTuto = true;
			setPlayTime(true);
			flagDt += dt;
			if (flagDt > 6.5f)
			{
				createPopTuto();
				showPopTuto(true);
				flagDt = 0.0f;
				chapter = 1;
			}

			break;

		case 1:
			setPlayTime(true);
			drawPopTuto(dt);

			break;

		case 2:
			setPlayTime(true);
			createTutoScript();
			createTutoBar();
			createClearScript();
			createTutoSkip();
			createTutoNum();
			createTutoEnd();
			chapter = 3;

			break;

		case 3:
			setPlayTime(true);
			showTutoScript(true);
			showTutoBar(true);
			showTutoSkip(true);
			showTutoNum(true);
			chapter = 4;

			break;

		case 4:
			setPlayTime(true);

			if (popOpenCheck())
				break;

			if (player->walk)
			{
				{
					flagDt += dt;
					if (flagDt > 2.0f)
					{
						flagDt = 0.0f;
						chapter = 5;
					}
				}
			}

			break;

		case 5:
			setPlayTime(true);

			showTutoScript(false);
			showTutoBar(false);
			showTutoSkip(false);
			showClearScript(true);

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				flagDt = 0.0f;
				chapter = 6;
			}

			break;

		case 6:
			setPlayTime(true);

			if (popOpenCheck())
				break;

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				showClearScript(false);
				if (flagDt > 4.0f)
				{
					flagDt = 0.0f;
					chapter = 7;
				}
			}

			break;

		case 7:
			setPlayTime(true);
			showTutoScript(true);
			showTutoBar(true);
			showTutoSkip(true);
			chapter = 8;

			break;

		case 8:
			setPlayTime(true);

			if (player->run)
			{
				flagDt += dt;
				if (flagDt > 2.0f)
				{
					flagDt = 0.0f;
					chapter = 9;
				}
			}

			break;

		case 9:
			setPlayTime(true);

			showTutoScript(false);
			showTutoBar(false);
			showTutoSkip(false);
			showClearScript(true);

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				flagDt = 0.0f;
				chapter = 10;
			}

			break;

		case 10:
			setPlayTime(true);

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				showClearScript(false);
				if (flagDt > 4.0f)
				{
					flagDt = 0.0f;
					chapter = 11;
				}
			}

			break;

		case 11:
			setPlayTime(true);
			showTutoScript(true);
			showTutoBar(true);
			showTutoSkip(true);
			chapter = 12;

			break;

		case 12:
			sizeDt = 0.1f;
			playTime = 0.0f;
			bPlayTime = false;
			player->hp = playerHp;
			flagDt = 0.0f;
			chapter = 13;

			break;

		case 13:
			sizeDt = 0.1f;
			playTime = 0.0f;
			bPlayTime = false;
			_appearEnemyDt = 0.5f;
			flagDt += dt;

			if (player->hp < playerHp)
				chapter = 12;

			if (flagDt > 10.0f)
			{
				dieEnemy();
				flagDt = 0.0f;
				chapter = 14;
			}

			break;

		case 14:
			setPlayTime(true);
			player->hp = playerHp;

			showTutoScript(false);
			showTutoBar(false);
			showTutoSkip(false);
			showClearScript(true);

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				flagDt = 0.0f;
				chapter = 15;
			}

			break;

		case 15:
			setPlayTime(true);

			flagDt += dt;
			if (flagDt > 2.0f)
			{
				showClearScript(false);
				if (flagDt > 4.0f)
				{
					flagDt = 0.0f;
					chapter = 16;
				}
			}

			break;

		case 16:
			setPlayTime(true);
			showTutoEnd(true);
			chapter = 17;

			break;

		case 17:
			setPlayTime(true);
			drawTutoEnd(dt);

			break;
	}

	if (chapter > 2)
	{
		if (uiDt < 2.0f)
			return;

		drawTutoScript(dt);
		drawTutoBar(dt);
		drawClearScript(dt);
		drawTutoSkip(dt);
		drawTutoNum(dt);
	}

	if (tryEnd)
	{
		startTuto = false;
		saveTuto = true;
		setPlayTime(false);
		saveTutorial();

		delete tutorial;
	}
}

bool Tutorial::key(iKeyState state, iPoint p)
{
	if (chapter == 1)
	{
		if (keyPopTuto(state, p))
			return true;
	}
	else if (chapter == 4 || chapter == 8 || chapter == 13)
	{
		if (keyTutoSkip(state, p))
			return true;
	}
	else if (chapter == 17)
	{
		if (keyTutoEnd(state, p))
			return true;
	}

	return false;
}

//++++++++++++++++++++++++++++++++++++++++
//+               PopTuto                +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popTuto;
iImage** imgTutoBtn;
iStrTex* stTuto;

void drawPopTutoBefore(iPopup* pop, float dt, float rate);
void closePopTuto(iPopup* pop);

void createPopTuto()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	stTuto = new iStrTex(methodTuto);
	stTuto->setString("%d", language);
	img = new iImage();
	img->addObject(stTuto->tex);
	pop->addObject(img);

	const char* strBtn[2] =
			{
					btn.o,
					btn.x
			};

	float w = devSize.width * 0.8f;
	float h = devSize.height * 0.8f;
	iPoint positionBtn[2] = {
			{w / 2 - 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
			{w / 2 + 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
	};

	size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);

	imgTutoBtn = methodStrBtn(strBtn, positionBtn, size, 2);
	for (int i = 0; i < 2; i++)
		pop->addObject(imgTutoBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawPopTutoBefore;
	pop->methodClose = closePopTuto;
	popTuto = pop;
}

int indexClosePopTuto;
void closePopTuto(iPopup* pop)
{
	if (indexClosePopTuto == 0)
	{
		tutorial->chapter = 2;
	}
	else
	{
		tutorial->tryEnd = true;
	}
}

void freePopTuto()
{
	if (popTuto == NULL)
		return;

	delete popTuto;
	popTuto = NULL;
	delete imgTutoBtn;
	delete stTuto;
}

void drawPopTutoBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgTutoBtn[i]->setTexObject(i == popTuto->selected);

	stTuto->setString("%d", language);
}

void drawPopTuto(float dt)
{
	if (popTuto)
		popTuto->paint(dt);
}

void showPopTuto(bool show)
{
	popTuto->show(show);
}

bool keyPopTuto(iKeyState state, iPoint p)
{
	if (popTuto == NULL ||
		popTuto->bShow == false)
		return false;
	if (popTuto->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgTutoBtn[i]->touchRect(popTuto->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popTuto->selected != j)
		{
			playSfxSound(0);
			popTuto->selected = j;
		}
		break;

	case iKeyStateEnded:
		i = popTuto->selected;
			if (i == -1)
				break;

			indexClosePopTuto = i;
			showPopTuto(false);
		break;
	}

	return true;
}


//++++++++++++++++++++++++++++++++++++++++
//+              TutoScript              +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popTutoScript;
iStrTex* stTutoScript;

void drawTutoScriptBefore(iPopup* pop, float dt, float rate);

void createTutoScript()
{
	Texture* tex; iImage* img; iSize size; int i;
	iPopup* pop = new iPopup();

	stTutoScript = new iStrTex(methodTutoScript);
	stTutoScript->setString("%d", tutorial->chapter);

	img = new iImage();
	img->addObject(stTutoScript->tex);
	img->position = iPointMake(0, -20 * SIZE_RATE);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(-devSize.width, devSize.height * 0.25f);
	pop->closePoint = iPointMake(0, devSize.height * 0.25);
	pop->methodBefore = drawTutoScriptBefore;
	popTutoScript = pop;
}

void freeTutoScript()
{
	if (popTutoScript == NULL)
		return;

	delete stTutoScript;
	delete popTutoScript;
}

void drawTutoScriptBefore(iPopup* pop, float dt, float rate)
{
	stTutoScript->setString("%d", tutorial->chapter);
}

void drawTutoScript(float dt)
{
	popTutoScript->paint(dt);
}

void showTutoScript(bool show)
{
	popTutoScript->show(show);
}

//++++++++++++++++++++++++++++++++++++++++
//+                TutoBar               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* TutoBar;

void drawTutoBarAfter(iPopup* pop, float dt, float rate);

void createTutoBar()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();
	iGraphics* g = iGraphics::share();
	size = iSizeMake(220 * SIZE_RATE, 20 * SIZE_RATE);
	g->init(size);

	g->drawRect(0, 0, 0, 0);
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	pop->_aniDt = 2.0f;
	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width, devSize.height * 0.25f + 30 * SIZE_RATE);
	pop->closePoint = iPointMake(devSize.width / 2, devSize.height * 0.25f + 30 * SIZE_RATE);
	pop->methodAfter = drawTutoBarAfter;
	TutoBar = pop;
}

void freeTutoBar()
{
	if (TutoBar == NULL)
		return;

	delete TutoBar;
}

void drawTutoBarAfter(iPopup* pop, float dt, float rate)
{
	iPoint p = pop->closePoint;
	int c = tutorial->chapter;
	float fDt = tutorial->flagDt;

	if (pop->stat == iPopupOpen)
		p = easeIn(pop->aniDt / pop->_aniDt, pop->openPoint, pop->closePoint);
	else if (pop->stat == iPopupClose)
		p = easeOut(pop->aniDt / pop->_aniDt, pop->closePoint, pop->openPoint);

	setRGBA(RGB_GRAY, 1.0f);
	fillRect(p.x, p.y, 208 * SIZE_RATE, 18 * SIZE_RATE, 9 * SIZE_RATE);
	setRGBA(RGBA_WHITE);

	setRGBA(RGBA_BLACK);
	fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE, 200 * SIZE_RATE, 10 * SIZE_RATE, 5 * SIZE_RATE);
	setRGBA(RGBA_WHITE);

	setRGBA(0.4f, 1.0f, 1.0f, 1.0f);
	if (3 == c || c == 4)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
				 100 * SIZE_RATE * fDt, 10 * SIZE_RATE, 5 * SIZE_RATE);
	else if (7 == c || c == 8)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
		   100 * SIZE_RATE * fDt, 10 * SIZE_RATE, 5 * SIZE_RATE);
	else if (11 <= c && c <= 13)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
		   20 * SIZE_RATE * fDt, 10 * SIZE_RATE, 5 * SIZE_RATE);
	else if (c == 5)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
				 200 * SIZE_RATE, 10 * SIZE_RATE, 5 * SIZE_RATE);
	else if (c == 9)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
		   200 * SIZE_RATE, 10 * SIZE_RATE, 5 * SIZE_RATE);
	else if (c == 14)
		fillRect(p.x + 4 * SIZE_RATE, p.y + 4 * SIZE_RATE,
		   200 * SIZE_RATE, 10 * SIZE_RATE, 5 * SIZE_RATE);
	setRGBA(RGBA_WHITE);
}

void drawTutoBar(float dt)
{
	TutoBar->paint(dt);
}

void showTutoBar(bool show)
{
	TutoBar->show(show);
}


//++++++++++++++++++++++++++++++++++++++++
//+             ClearScript              +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popClearScript;
iStrTex* stClearScript;

void drawClearScriptBefore(iPopup* pop, float dt, float rate);

void createClearScript()
{
	Texture* tex; iImage* img; iSize size; int i;
	iPopup* pop = new iPopup();

	stClearScript = new iStrTex(methodClearScript);
	stClearScript->setString("%d", tutorial->chapter);

	img = new iImage();
	img->addObject(stClearScript->tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(-devSize.width, devSize.height * 0.25f);
	pop->closePoint = iPointMake(0, devSize.height * 0.25);
	pop->methodBefore = drawClearScriptBefore;
	popClearScript = pop;
}

void freeClearScript()
{
	if (popClearScript == NULL)
		return;

	delete popClearScript;
	delete stClearScript;
}

void drawClearScriptBefore(iPopup* pop, float dt, float rate)
{
	stClearScript->setString("%d", tutorial->chapter);
}

void drawClearScript(float dt)
{
	popClearScript->paint(dt);
}

void showClearScript(bool show)
{
	popClearScript->show(show);
}


//++++++++++++++++++++++++++++++++++++++++
//+               TutoSkip               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popTutoSkip;
iImage* imgTutoSkipBtn;

void drawTutoSkipBefore(iPopup* pop, float dt, float rate);

void createTutoSkip()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(100 * SIZE_RATE, 50 * SIZE_RATE);
	const char* strBtn = "Skip";
	imgTutoSkipBtn = methodStrBtn(strBtn, iPointZero, size);
	pop->addObject(imgTutoSkipBtn);

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(devSize.width, devSize.height / 2);
	pop->closePoint = iPointMake(devSize.width - 100 * SIZE_RATE,devSize.height / 2);
	pop->methodBefore = drawTutoSkipBefore;
	popTutoSkip = pop;
}

void freeTutoSkip()
{
	if (popTutoSkip == NULL)
		return;

	delete popTutoSkip;
}

void drawTutoSkipBefore(iPopup* pop, float dt, float rate)
{
	imgTutoSkipBtn->setTexObject(0 == popTutoSkip->selected);
}

void drawTutoSkip(float dt)
{
	popTutoSkip->paint(dt);
}

void showTutoSkip(bool show)
{
	popTutoSkip->show(show);
}

bool keyTutoSkip(iKeyState state, iPoint p)
{
	if (popTutoSkip->bShow == false)
		return false;
	if (popTutoSkip->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgTutoSkipBtn->touchRect(popTutoSkip->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popTutoSkip->selected != j)
		{
			playSfxSound(0);
			popTutoSkip->selected = j;
		}
		break;

	case iKeyStateEnded:
		i = popTutoSkip->selected;
			if (i == -1)
				break;

			if (i == 0)
			{
				tutorial->chapter++;
			}
			popTutoSkip->selected = -1;
		break;
	}

	return true;
}


//++++++++++++++++++++++++++++++++++++++++
//+                TutoNum               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popTutoNum;
iStrTex* stTutoNum;

Texture* methodTutoNum(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(180 * SIZE_RATE, 40 * SIZE_RATE);
	g->init(size);

	setStringSize(20 * SIZE_RATE);
	setStringRGBA(RGBA_BLACK);

	size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);

	setRGBA(RGB_GRAY, 1.0f);
	if (tutorial->chapter == 4)		setRGBA(RGB_YELLOW, 1.0f);
	else if (tutorial->chapter > 4)	setRGBA(RGB_MINT, 1.0f);
	g->fillRect(60 * SIZE_RATE * 0, 0, size.width, size.height, 5 * SIZE_RATE);

	setRGBA(RGB_GRAY, 1.0f);
	if (tutorial->chapter == 8)		setRGBA(RGB_YELLOW, 1.0f);
	else if (tutorial->chapter > 8)	setRGBA(RGB_MINT, 1.0f);
	g->fillRect(60 * SIZE_RATE * 1, 0, size.width, size.height, 5 * SIZE_RATE);

	setRGBA(RGB_GRAY, 1.0f);
	if (tutorial->chapter == 12 ||
		tutorial->chapter == 13)		setRGBA(RGB_YELLOW, 1.0f);
	else if (tutorial->chapter > 13)	setRGBA(RGB_MINT, 1.0f);
	g->fillRect(60 * SIZE_RATE * 2, 0, size.width, size.height, 5 * SIZE_RATE);

	setRGBA(RGBA_WHITE);

	g->drawString((size.width / 2) + (60 * SIZE_RATE * 0), size.height / 2, VCENTER | HCENTER, "0");
	g->drawString((size.width / 2) + (60 * SIZE_RATE * 1), size.height / 2, VCENTER | HCENTER, "1");
	g->drawString((size.width / 2) + (60 * SIZE_RATE * 2), size.height / 2, VCENTER | HCENTER, "2");

	return g->getTexture();
}

void drawTutoNumBefore(iPopup* pop, float dt, float rate)
{
	stTutoNum->setString("%d", tutorial->chapter);
}

void createTutoNum()
{
	Texture* tex; iImage* img; iSize size; int i;
	iPopup* pop = new iPopup();

	stTutoNum = new iStrTex(methodTutoNum);
	stTutoNum->setString("%d", tutorial->chapter);

	img = new iImage();
	img->addObject(stTutoNum->tex);
	pop->addObject(img);

	pop->style = iPopupMove;
	pop->_aniDt = 2.0f;
	pop->openPoint = iPointMake(devSize.width / 2, devSize.height + 50 * SIZE_RATE);
	pop->closePoint = iPointMake(devSize.width / 2, devSize.height - 50 * SIZE_RATE);
	pop->methodBefore = drawTutoNumBefore;
	popTutoNum = pop;
}

void freeTutoNum()
{
	if (popTutoNum == NULL)
		return;

	delete popTutoNum;
	delete stTutoNum;
}

void drawTutoNum(float dt)
{
	popTutoNum->paint(dt);
}

void showTutoNum(bool show)
{
	popTutoNum->show(show);
}


//++++++++++++++++++++++++++++++++++++++++
//+                TutoEnd               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popTutoEnd;
iImage** imgTutoEndBtn;
iStrTex* stTutoEnd;

void closePopTutoEnd(iPopup* pop);
void drawPopTutoEndBefore(iPopup* pop, float dt, float rate);

void createTutoEnd()
{
	Texture* tex; iImage* img; iSize size; int i;
	iPopup* pop = new iPopup();

	stTutoEnd = new iStrTex(methodTutoEnd);
	stTutoEnd->setString("%d", language);
	img = new iImage();
	img->addObject(stTutoEnd->tex);
	pop->addObject(img);

	const char* strBtn[2] =
			{
					btn.o,
					btn.x
			};

	float w = devSize.width * 0.8f;
	float h = devSize.height * 0.8f;
	iPoint positionBtn[2] = {
			{w / 2 - 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
			{w / 2 + 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
	};

	size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);

	imgTutoEndBtn = methodStrBtn(strBtn, positionBtn, size, 2);
	for (i = 0; i < 2; i++)
		pop->addObject(imgTutoEndBtn[i]);

	pop->style = iPopupMove;
	pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
	pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
	pop->methodBefore = drawPopTutoEndBefore;
	pop->methodClose = closePopTutoEnd;
	popTutoEnd = pop;
}

int indexClosePopTutoEnd;
void closePopTutoEnd(iPopup* pop)
{
	if (indexClosePopTutoEnd == 0)
		setLoading(gs_proc, freeProc, loadProc);
	else
		setLoading(gs_menu, freeProc, loadMenu);

	tutorial->tryEnd = true;
}

void freeTutoEnd()
{
	if (popTutoEnd == NULL)
		return;

	delete popTutoEnd;
	delete imgTutoEndBtn;
	delete stTutoEnd;
}

void drawPopTutoEndBefore(iPopup* pop, float dt, float rate)
{
	for (int i = 0; i < 2; i++)
		imgTutoEndBtn[i]->setTexObject(i == popTutoEnd->selected);

	stTutoEnd->setString("%d", language);
}

void drawTutoEnd(float dt)
{
	if (popTutoEnd)
		popTutoEnd->paint(dt);
}

void showTutoEnd(bool show)
{
	popTutoEnd->show(show);
}

bool keyTutoEnd(iKeyState state, iPoint p)
{
	if (popTutoEnd == NULL ||
		popTutoEnd->bShow == false)
		return false;
	if (popTutoEnd->stat != iPopupProc)
		return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;
	case iKeyStateMoved:
		for (i = 0; i < 2; i++)
		{
			if (containPoint(p, imgTutoEndBtn[i]->touchRect(popTutoEnd->closePoint)))
			{
				j = i;
				break;
			}
		}
		if (popTutoEnd->selected != j)
		{
			playSfxSound(0);
			popTutoEnd->selected = j;
		}
		break;

	case iKeyStateEnded:
		i = popTutoEnd->selected;
			if (i == -1)
				break;

			indexClosePopTutoEnd = i;
			showTutoEnd(false);
		break;
	}

	return true;
}

bool popOpenCheck()
{
	if (TutoBar->stat == iPopupOpen)
		return true;
	return false;
}
