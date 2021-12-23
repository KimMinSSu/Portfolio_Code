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

    if (player->hp < 1 || procPopShow)
    {
        sizeDt = 0.1f;
        _dt = 0.0f;
    }
    else
        _dt = dt;

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

void keyProc(iKeyState state, iPoint p)
{
    if (!player->alive)
    {
        if (onceOver)
            if (keyPopOver(state, p))
                return;

        if (onceHidden)
            if (keyPopHidden(state, p))
                return;
    }
    else
    {
        keyPauseBtn(state, p);
        keyJoystick(state, p);
    }

    if (keyPopProc(state, p)	||
        keyProcReStart(state, p)||
        keyProcInfo(state, p)	||
        keyProcSet(state, p)	||
        keyProcGoMenu(state, p)) {
        return;
    }
}

void keyProc(iKeyState state, int touchNum, iPoint* p)
{
    if (!player->alive)
    {
        if (onceOver)
            if (keyPopOver(state, p[0]))
                return;
        if (onceHidden)
            if (keyPopHidden(state, p[0]))
                return;
    }
    else
    {
        keyPauseBtn(state, touchNum, p);
        keyJoystick(state, touchNum, p);
    }

    if (keyPopProc(state, p[0])	   ||
        keyProcReStart(state, p[0]) ||
        keyProcInfo(state, p[0])	   ||
        keyProcSet(state, p[0])	   ||
        keyProcGoMenu(state, p[0]))
        return;
}

//============================================
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
iPopup** popProcBtn;
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
    popProcBtn = new iPopup * [6];

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

        imgProcBtn[i]->position = iPointMake(0, i * 50 * SIZE_RATE);
        pop->addObject(imgProcBtn[i]);

        float h = devSize.height * 0.3f - 20 * SIZE_RATE;
        float w = i % 2 ? -150 * SIZE_RATE : devSize.width;
        pop->style = iPopupMove;
        pop->openPoint = iPointMake(w, h);
        pop->closePoint = iPointMake(devSize.width / 2 - 100 * SIZE_RATE, h);
        pop->methodBefore = drawPopProcBefore;
        pop->methodClose = closePopProc;
        popProcBtn[i] = pop;
    }

    iImage* img;
    iPopup* pop = new iPopup();

    stPause = new iStrTex(methodProcTitle);
    stPause->setString(str[5]);
    img = new iImage();
    img->addObject(stPause->tex);
    pop->addObject(img);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width / 2 - 150 * SIZE_RATE, -70 * SIZE_RATE);
    pop->closePoint = iPointMake(devSize.width / 2 - 150 * SIZE_RATE, devSize.height * 0.15f - 35 * SIZE_RATE);
    popProcBtn[5] = pop;
}

void freePopProc()
{
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 2; j++)
            delete stProc[i][j];
        delete stProc[i];
        delete popProcBtn[i];
    }
    delete stPause;
    delete stProc;
    delete imgProcBtn;
    delete popProcBtn;
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
        imgProcBtn[i]->setTexObject(i == popProcBtn[0]->selected);
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
        popProcBtn[i]->paint(dt);
    }
}

void showPopProc(bool show)
{
    for (int i = 0; i < 6; i++)
        popProcBtn[i]->show(show);
}

bool keyPopProc(iKeyState state, iPoint p)
{
    if (popProcBtn[0]->bShow == false)
        return false;
    if (popProcBtn[0]->stat != iPopupProc)
        return true;

    int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 5; i++)
        {
            if (containPoint(p, imgProcBtn[i]->touchRect(popProcBtn[i]->closePoint)))
            {
                j = i;
                break;
            }
        }

            for (i = 0; i < 5; i++)
            {
                if (popProcBtn[i]->selected != j)
                {
                    playSfxSound(0);
                    popProcBtn[i]->selected = j;
                }
            }
		break;

	case iKeyStateEnded:
        if (popProcBtn[0]->selected == -1)
            break;

            indexCloseProc = -1;
            if (popProcBtn[0]->selected == 0)
            {
                procPopShow = false;
                extern bool pressHomeB;
                pressHomeB = false;
                popProcBtn[0]->selected = -1;
            }
            else if (popProcBtn[0]->selected == 1)
            {
                indexCloseProc = 0;
                otherPopShow = true;
                popProcBtn[0]->selected = -1;
            }
            else if (popProcBtn[0]->selected == 2)
            {
                indexCloseProc = 1;
                otherPopShow = true;
                popProcBtn[0]->selected = -1;
            }
            else if (popProcBtn[0]->selected == 3)
            {
                indexCloseProc = 2;
                otherPopShow = true;
                popProcBtn[0]->selected = -1;
            }
            else if (popProcBtn[0]->selected == 4)
            {
                indexCloseProc = 3;
                otherPopShow = true;
                popProcBtn[0]->selected = -1;
            }
            showPopProc(false);
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+             ProcContinue             +
//++++++++++++++++++++++++++++++++++++++++

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
            {w / 2 - 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
            {w / 2 + 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
    };

    size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);
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
#if (OS == OS_WINDOW)
    if (popProcReStart->bShow)
	{
		if (keyDown == keysLeftArrow)
		{

			popProcReStart->selected--;
			if (popProcReStart->selected < 0)
				popProcReStart->selected = 1;
		}
		else if (keyDown == keysRightArrow)
		{

			popProcReStart->selected++;
			if (popProcReStart->selected > 1)
				popProcReStart->selected = 0;
		}

		if (keyDown == keysEnter)
		{
			if (popProcReStart->selected == 0)
			{
				setLoading(gs_proc, freeProc, loadProc);
			}
			else if (popProcReStart->selected == 1)
			{
				showProcReStart(false);
				indexCloseProc = -1;
			}
		}

		if (keyDown == keysESC)
		{
			showProcReStart(false);
			indexCloseProc = -1;
		}
	}
#endif
    popProcReStart->paint(dt);
}

void showProcReStart(bool show)
{
    popProcReStart->show(show);
}

bool keyProcReStart(iKeyState state, iPoint p)
{
    if (popProcReStart->bShow == false)
        return false;
    if (popProcReStart->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgProcReStartBtn[i]->touchRect(popProcReStart->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popProcReStart->selected != j)
        {
            playSfxSound(0);
            popProcReStart->selected = j;
        }
		break;

	case iKeyStateEnded:
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
                otherPopShow = false;
                indexCloseProc = -1;
            }
            popProcReStart->selected = -1;
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               ProcInfo               +
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
            {0, devSize.height * 0.8f - 45 * SIZE_RATE},
            {devSize.width * 0.8f - 40 * SIZE_RATE, devSize.height * 0.8f - 45 * SIZE_RATE},
            {devSize.width * 0.8f - 40 * SIZE_RATE, 0}
    };

    size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);
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
        procEnemySample[i] = createBall2(20);
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

            drawImage(procEnemySample[i], p.x, p.y + 80 * SIZE_RATE + 40 * SIZE_RATE * i, VCENTER | LEFT);
        }
    }
    else if (pageInfo == 3)
    {
        drawImage(procItemSample[0], p.x, p.y + 60 * SIZE_RATE, TOP | LEFT, 0, 0,
                  procItemSample[0]->width, procItemSample[0]->height, 1, 1, 0, 0);
        drawImage(procItemSample[1], p.x, p.y + 180 * SIZE_RATE, TOP | LEFT, 0, 0,
                  procItemSample[1]->width, procItemSample[1]->height, 1, 1, 0, 0);
    }
}

void drawProcInfo(float dt)
{
#if (OS == OS_WINDOW)
    if (popProcInfo->bShow)
	{
		if (keyDown == keysLeftArrow)
		{

			pageInfo--;
			if (pageInfo < 0)
				pageInfo = _pageInfo - 1;
		}
		else if (keyDown == keysRightArrow)
		{

			pageInfo++;
			if (pageInfo > _pageInfo - 1)
				pageInfo = 0;
		}

		else if (keyDown == keysESC)
		{
			showProcInfo(false);
			indexCloseProc = -1;
		}
	}
#endif
    popProcInfo->paint(dt);
}

void showProcInfo(bool show)
{
    popProcInfo->show(show);
    if (show)
        pageInfo = 0;

}

bool keyProcInfo(iKeyState state, iPoint p)
{
    if (popProcInfo->bShow == false)
        return false;
    if (popProcInfo->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(p, imgProcInfoBtn[i]->touchRect(popProcInfo->closePoint)))
            {
                j = i;
                break;
            }
        }
            if (popProcInfo->selected != j)
            {
                playSfxSound(0);
                popProcInfo->selected = j;
            }
		break;

	case iKeyStateEnded:
            i = popProcInfo->selected;
            if (i == -1)
                break;

            if (i == 0)
            {
                playSfxSound(2);
                pageInfo--;
                if (pageInfo < 0)
                    pageInfo = _pageInfo - 1;
            }
            else if (i == 1)
            {
                playSfxSound(2);
                pageInfo++;
                if (pageInfo > _pageInfo - 1)
                    pageInfo = 0;
            }
            else if (i == 2)
            {
                showProcInfo(false);
                otherPopShow = false;
                indexCloseProc = -1;
            }
            popProcInfo->selected = -1;
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

extern int filterNum;
void drawProcSetBefore(iPopup* pop, float dt, float rate);

void createProcSet()
{
    iImage* img; iSize size;
    iPopup* pop = new iPopup();

    stProcSet = new iStrTex(methodSet);
    stProcSet->setString("%d %d %d %d", language, onoffBGM, onoffSFX, filterNum);
    img = new iImage();
    img->addObject(stProcSet->tex);
    pop->addObject(img);

    const char* strBtn[9] = {
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
    size = iSizeMake(30 * SIZE_RATE, 50 * SIZE_RATE);
    iPoint positionBtn[9] = {
            {w * 0.5f - 15 * SIZE_RATE, h - 10 * SIZE_RATE},
            {w * 0.75f - 15 * SIZE_RATE, h - 10 * SIZE_RATE},
            {w * 0.5f - 15 * SIZE_RATE, h + 50 * SIZE_RATE - 10 * SIZE_RATE},
            {w * 0.75f - 15 * SIZE_RATE, h + 50 * SIZE_RATE - 10 * SIZE_RATE},
            {w * 0.5f - 15 * SIZE_RATE, h + 100 * SIZE_RATE - 10 * SIZE_RATE},
            {w * 0.75f - 15 * SIZE_RATE, h + 100 * SIZE_RATE - 10 * SIZE_RATE},
            {w * 0.5f - 15 * SIZE_RATE, h + 150 * SIZE_RATE - 10 * SIZE_RATE},
            {w * 0.75f - 15 * SIZE_RATE, h + 150 * SIZE_RATE - 10 * SIZE_RATE},
            {w - 40 * SIZE_RATE, 0}
    };

    imgProcSetBtn = methodStrBtn(strBtn, positionBtn, size, 9);
    for (int i = 0; i < 9; i++)
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
    delete imgProcSetBtn;
    delete stProcSet;
}

void drawProcSetBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 9; i++)
        imgProcSetBtn[i]->setTexObject(i == popProcSet->selected);

    stProcSet->setString("%d %d %d %d",
                         language, onoffBGM, onoffSFX, filterNum);
}

void drawProcSet(float dt)
{
    popProcSet->paint(dt);
}

void showProcSet(bool show)
{
    popProcSet->show(show);
}

bool keyProcSet(iKeyState state, iPoint p)
{
    if (popProcSet->bShow == false)
        return false;
    if (popProcSet->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
            break;

	case iKeyStateMoved:
        for (i = 0; i < 9; i++)
        {
            if (containPoint(p, imgProcSetBtn[i]->touchRect(popProcSet->closePoint)))
            {
                j = i;
                break;
            }
        }
            if (popProcSet->selected != j)
            {
                playSfxSound(0);
                popProcSet->selected = j;
            }
		break;

	case iKeyStateEnded:
        i = popProcSet->selected;
            if (i == -1)
                break;

            if (i == 0)
            {
                language--;
                if (language < 0)
                    language = LanguageMax - 1;
            }
            else if (i == 1)
            {
                language++;
                if (language > LanguageMax - 1)
                    language = 0;
            }
            else if (i == 2)
            {
                onoffBGM--;
                if (onoffBGM < 0)
                    onoffBGM = 1;

                if (onoffBGM == 0)
                    stopBgmSound(0);
                else if (onoffBGM == 1)
                    playBgmSound(0);
            }
            else if (i == 3)
            {
                onoffBGM++;
                if (onoffBGM > 1)
                    onoffBGM = 0;

                if (onoffBGM == 0)
                    stopBgmSound(0);
                else if (onoffBGM == 1)
                    playBgmSound(0);
            }
            else if (i == 4)
            {
                onoffSFX--;
                if (onoffSFX < 0)
                    onoffSFX = 1;

                volumeSfxSound(onoffSFX);
            }
            else if (i == 5)
            {
                onoffSFX++;
                if (onoffSFX > 1)
                    onoffSFX = 0;

                volumeSfxSound(onoffSFX);
            }
            else if (i == 6)
            {
                filterNum--;
                if (filterNum < 0)
                    filterNum = 2;
            }
            else if (i == 7)
            {
                filterNum++;
                if (filterNum > 2)
                    filterNum = 0;
            }
            else if (i == 8)
            {
                setLanguage(language);
                showProcSet(false);
                otherPopShow = false;
                popProcSet->selected = -1;
            }
            popProcSet->selected = -1;
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
            {w / 2 - 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
            {w / 2 + 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 + 60 * SIZE_RATE - 20 * SIZE_RATE},
    };

    size = iSizeMake(40* SIZE_RATE, 40* SIZE_RATE);

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

bool keyProcGoMenu(iKeyState state, iPoint p)
{
    if (popProcGoMenu->bShow == false)
        return false;
    if (popProcGoMenu->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgProcGoMenuBtn[i]->touchRect(popProcGoMenu->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popProcGoMenu->selected != j)
        {
            playSfxSound(0);
            popProcGoMenu->selected = j;
        }
		break;

	case iKeyStateEnded:
        i = popProcGoMenu->selected;
        if (i == -1)
            break;

        if (i == 0)
        {
            setLoading(gs_menu, freeProc, loadMenu);
        }
        else// if (i == 1)
        {
            showProcGoMenu(false);
            otherPopShow = false;
            indexCloseProc = -1;
        }
            popProcGoMenu->selected = -1;
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
        imgOverBtn[i]->position = iPointMake(0, i * 50 * SIZE_RATE);
        pop->addObject(imgOverBtn[i]);

        float h = devSize.height / 2;
        float w = i % 2 ? -150 * SIZE_RATE : devSize.width;
        pop->style = iPopupMove;
        pop->openPoint = iPointMake(w, h);
        pop->closePoint = iPointMake(devSize.width / 2 - 100 * SIZE_RATE, h);
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
    pop->openPoint = iPointMake(devSize.width * 0.1f, - 100 * SIZE_RATE);
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

bool keyPopOver(iKeyState state, iPoint p)
{
    if (popOver[0]->bShow == false)
        return false;
    if (popOver[0]->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgOverBtn[i]->touchRect(popOver[i]->closePoint)))
            {
                j = i;
                break;
            }
        }

            for (i = 0; i < 2; i++)
            {
                if (popOver[i]->selected != j)
                {
                    playSfxSound(0);
                    popOver[i]->selected = j;
                }
            }
		break;

	case iKeyStateEnded:
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
        imgHiddenBtn[i]->position = iPointMake(0, i * 50 * SIZE_RATE);
        pop->addObject(imgHiddenBtn[i]);

        float h = devSize.height * 0.4f;
        float w = i % 2 ? -150 * SIZE_RATE : devSize.width;
        pop->style = iPopupMove;
        pop->openPoint = iPointMake(w, h);
        pop->closePoint = iPointMake(devSize.width / 2 - 100 * SIZE_RATE, h);
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
    pop->openPoint = iPointMake(devSize.width * 0.1f, -100 * SIZE_RATE);
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

bool keyPopHidden(iKeyState state, iPoint p)
{
    if (popHidden[0]->bShow == false)
        return false;
    if (popHidden[0]->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgHiddenBtn[i]->touchRect(popHidden[i]->closePoint)))
            {
                j = i;
                break;
            }
        }

            for (i = 0; i < 2; i++)
            {
                if (popHidden[i]->selected != j)
                {
                    playSfxSound(0);
                    popHidden[i]->selected = j;
                }
            }
		break;

	case iKeyStateEnded:
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
	}

	return true;
}
