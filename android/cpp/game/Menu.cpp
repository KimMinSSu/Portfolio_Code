#include "Menu.h"

#include "Loading.h"
#include "Tutorial.h"
#include "Intro.h"
#include "Proc.h"

#include "PopMethod.h"
#include "Script.h"
#include "Sound.h"
#include "Data.h"

void loadMenu()
{
    createMenuBtn();
    showMenuBtn(true);

    createMenuInfo();
    createMenuSet();
    createMenuGoIntro();
    createMenuExit();

    createResetBtn();
    showResetBtn(true);
    createBestTime();
    showBestTime(true);
}

void freeMenu()
{
    freeMenuBtn();
    freeMenuInfo();
    freeMenuSet();
    freeMenuGoIntro();
    freeMenuExit();

    freeResetBtn();
    freeBestTime();
}

void drawMenu(float dt)
{
    drawMenuBtn(dt);
    drawMenuInfo(dt);
    drawMenuSet(dt);
    drawMenuGoIntro(dt);
    drawMenuExit(dt);

    drawResetBtn(dt);
    drawBestTime(dt);
}

void keyMenu(iKeyState state, iPoint mouse)
{
    keyResetBtn(state, mouse);
    if (keyMenuBtn(state, mouse)	||
        keyMenuInfo(state, mouse)	||
        keyMenuSet(state, mouse)	||
        keyMenuGoIntro(state, mouse)||
        keyMenuExit(state, mouse))
        return;
}

//========================================
int indexCloseMenu;

void closeMenu(iPopup* pop)
{
    if (indexCloseMenu == -1)
        return;

    void (*MethodShow[4])(bool show) =
            {
                    showMenuInfo,	//0
                    showMenuSet,	//1
                    showMenuGoIntro,//2
                    showMenuExit,	//3
            };
    MethodShow[indexCloseMenu](true);
}

void closeMenuCommon(iPopup* pop)
{
    showMenuBtn(true);
    showResetBtn(true);
    showBestTime(true);
}

//++++++++++++++++++++++++++++++++++++++++
//+               MenuBtn                +
//++++++++++++++++++++++++++++++++++++++++
iPopup** popMenuBtn;
iImage** imgMenuBtn;
iStrTex*** stMenuBtn;
iStrTex* stMenuTitle;

void drawMenuBtnBefore(iPopup* pop, float dt, float rate);

void createMenuBtn()
{
    const char* str[7] =
            {
                    script[language].strBtn.start,
                    script[language].strBtn.tuto,
                    script[language].strBtn.info,
                    script[language].strBtn.set,
                    script[language].strBtn.goIntro,
                    script[language].strBtn.exit,
                    script[language].strWords.title,
            };

    stMenuBtn = new iStrTex * *[6];
    imgMenuBtn = new iImage * [6];
    popMenuBtn = new iPopup * [7];

    for (int i = 0; i < 6; i++)
    {
        iPopup* pop = new iPopup();
        stMenuBtn[i] = new iStrTex * [2];
        imgMenuBtn[i] = new iImage();

        for (int j = 0; j < 2; j++)
        {
            if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
            else setStringRGBA(RGB_MINT, 1.0f);
            stMenuBtn[i][j] = new iStrTex(methodMenuBtn);
            stMenuBtn[i][j]->setString(str[i]);
            imgMenuBtn[i]->addObject(stMenuBtn[i][j]->tex);
        }
        imgMenuBtn[i]->position = iPointMake(0, i * 50 * SIZE_RATE);
        pop->addObject(imgMenuBtn[i]);

        float h = devSize.height * 0.3f - 20 * SIZE_RATE;
        float w = i % 2 ? -150 * SIZE_RATE : devSize.width;
        pop->style = iPopupMove;
        pop->openPoint = iPointMake(w, h);
        pop->closePoint = iPointMake(devSize.width / 2 - 100 * SIZE_RATE, h);
        pop->methodBefore = drawMenuBtnBefore;
        pop->methodClose = closeMenu;
        popMenuBtn[i] = pop;
    }

    iImage* img;
    iPopup* pop = new iPopup();

    stMenuTitle = new iStrTex(methodMenuTitle);
    stMenuTitle->setString(str[6]);
    img = new iImage();
    img->addObject(stMenuTitle->tex);
    pop->addObject(img);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width / 2 - 150 * SIZE_RATE, -70 * SIZE_RATE);
    pop->closePoint = iPointMake(devSize.width / 2 - 150 * SIZE_RATE, devSize.height * 0.15f - 35 * SIZE_RATE);
    popMenuBtn[6] = pop;

    indexCloseMenu = -1;
}

void freeMenuBtn()
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 2; j++)
            delete stMenuBtn[i][j];
        delete stMenuBtn[i];
        delete popMenuBtn[i];
    }
    delete stMenuTitle;
    delete stMenuBtn;
    delete imgMenuBtn;
    delete popMenuBtn;
}

void drawMenuBtnBefore(iPopup* pop, float dt, float rate)
{
    const char* str[7] =
            {
                    script[language].strBtn.start,
                    script[language].strBtn.tuto,
                    script[language].strBtn.info,
                    script[language].strBtn.set,
                    script[language].strBtn.goIntro,
                    script[language].strBtn.exit,
                    script[language].strWords.title,
            };

    for (int i = 0; i < 6; i++)
    {
        imgMenuBtn[i]->setTexObject(i == popMenuBtn[0]->selected);
        for (int j = 0; j < 2; j++)
        {
            if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
            else setStringRGBA(RGB_MINT, 1.0f);
            stMenuBtn[i][j]->setString(str[i]);
        }
    }
    stMenuTitle->setString(str[6]);
}

void drawMenuBtn(float dt)
{
    for (int i = 0; i < 7; i++)
        popMenuBtn[i]->paint(dt);
}

void showMenuBtn(bool show)
{
    for (int i = 0; i < 7; i++)
        popMenuBtn[i]->show(show);
}

bool keyMenuBtn(iKeyState state, iPoint p)
{
    if (popMenuBtn[0]->bShow == false)
        return false;
    if (popMenuBtn[0]->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 6; i++)
        {
            if (containPoint(p, imgMenuBtn[i]->touchRect(popMenuBtn[i]->closePoint)))
            {
                j = i;
                break;
            }
        }

            for (i = 0; i < 6; i++)
            {
                if (popMenuBtn[i]->selected != j)
                {
                    playSfxSound(0);
                    popMenuBtn[i]->selected = j;
                }
            }
		break;

	case iKeyStateEnded:
        indexCloseMenu = -1;

        if (popMenuBtn[0]->selected == -1)
            break;

            indexCloseMenu = -1;
            if (popMenuBtn[0]->selected == 0)
            {
                setLoading(gs_proc, freeMenu, loadProc);
            }
            else if (popMenuBtn[0]->selected == 1)
            {
                enforceRunTutorial();
                setLoading(gs_proc, freeMenu, loadProc);
            }
            else if (popMenuBtn[0]->selected == 2)
            {
                indexCloseMenu = 0;
                popMenuBtn[0]->selected = -1;
            }
            else if (popMenuBtn[0]->selected == 3)
            {
                indexCloseMenu = 1;
                popMenuBtn[0]->selected = -1;
            }
            else if (popMenuBtn[0]->selected == 4)
            {
                indexCloseMenu = 2;
                popMenuBtn[0]->selected = -1;
            }
            else if (popMenuBtn[0]->selected == 5)
            {
                indexCloseMenu = 3;
                popMenuBtn[0]->selected = -1;
            }
            showResetBtn(false);
            showBestTime(false);
            showMenuBtn(false);
		break;
	}

	return true;
}

//======================Popup=======================
//++++++++++++++++++++++++++++++++++++++++
//+               MenuStart              +
//++++++++++++++++++++++++++++++++++++++++
//게임 시작됨

//++++++++++++++++++++++++++++++++++++++++
//+               MenuInfo               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popMenuInfo;
iImage** imgMenuInfoBtn;
iStrTex* stMenuInfo;
Texture** menuEnemySample;
Texture** menuItemSample;

void drawMenuInfoBefore(iPopup* pop, float dt, float rate);

void createMenuInfo()
{
    pageInfo = 0;
    iImage* img; iSize size;
    iPopup* pop = new iPopup();

    iStrTex* st = new iStrTex(methodInfo);
    stMenuInfo = st;
    st->setString("%d", pageInfo);
    img = new iImage();
    img->addObject(st->tex);
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
    imgMenuInfoBtn = methodStrBtn(strBtn, positionBtn, size, 3);
    for (int i = 0; i < 3; i++)
        pop->addObject(imgMenuInfoBtn[i]);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
    pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
    pop->methodBefore = drawMenuInfoBefore;
    pop->methodClose = closeMenuCommon;
    popMenuInfo = pop;

    menuEnemySample = new Texture * [6];
    for (int i = 0; i < 6; i++)
        menuEnemySample[i] = createBall2(20);
    menuItemSample = new Texture * [2];
    for (int i = 0; i < 2; i++)
        menuItemSample[i] = createImage("Assets/Potion/Potion%d.png", i);
}

void freeMenuInfo()
{
    for (int i = 0; i < 6; i++)
        freeImage(menuEnemySample[i]);
    delete menuEnemySample;
    for (int i = 0; i < 2; i++)
        freeImage(menuItemSample[i]);
    delete menuItemSample;
    delete popMenuInfo;
    delete imgMenuInfoBtn;
    delete stMenuInfo;
}

void drawMenuInfoBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 3; i++)
        imgMenuInfoBtn[i]->setTexObject(popMenuInfo->selected == i);
    stMenuInfo->setString("%d %d", pageInfo, language);

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

            drawImage(menuEnemySample[i], p.x, p.y + 80 * SIZE_RATE + 40  * SIZE_RATE * i, VCENTER | LEFT);
        }
    }
    else if (pageInfo == 3)
    {
        drawImage(menuItemSample[0], p.x, p.y + 60 * SIZE_RATE, TOP | LEFT, 0, 0,
                  menuItemSample[0]->width, menuItemSample[0]->height, 1, 1, 0, 0);
        drawImage(menuItemSample[1], p.x, p.y + 180 * SIZE_RATE, TOP | LEFT, 0, 0,
                  menuItemSample[1]->width, menuItemSample[1]->height, 1, 1, 0, 0);
    }
}

void drawMenuInfo(float dt)
{
    popMenuInfo->paint(dt);
}

void showMenuInfo(bool show)
{
    popMenuInfo->show(show);
    if (show)
        pageInfo = 0;
}

bool keyMenuInfo(iKeyState state, iPoint p)
{
    if (popMenuInfo->bShow == false)
        return false;
    if (popMenuInfo->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state) {

	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 3; i++)
        {
            if (containPoint(p, imgMenuInfoBtn[i]->touchRect(popMenuInfo->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popMenuInfo->selected != j)
        {
            playSfxSound(0);
            popMenuInfo->selected = j;
        }
		break;

	case iKeyStateEnded:
        i = popMenuInfo->selected;
        if (i == -1) break;

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
            showMenuInfo(false);
        }
        popMenuInfo->selected = -1;
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               MenuSet                +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popMenuSet;
iImage** imgMenuSetBtn;
iStrTex* stMenuSet;

extern int filterNum;
void drawMenuSetBefore(iPopup* pop, float dt, float rate);

void createMenuSet()
{
    iImage* img; iSize size;
    iPopup* pop = new iPopup();

    stMenuSet = new iStrTex(methodSet);
    stMenuSet->setString("%d %d %d %d", language, onoffBGM, onoffSFX, filterNum);
    img = new iImage();
    img->addObject(stMenuSet->tex);
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

    imgMenuSetBtn = methodStrBtn(strBtn, positionBtn, size, 9);
    for (int i = 0; i < 9; i++)
    {
        pop->addObject(imgMenuSetBtn[i]);
    }

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width * 0.1, devSize.height);
    pop->closePoint = iPointMake(devSize.width * 0.1, devSize.height * 0.1);
    pop->methodBefore = drawMenuSetBefore;
    pop->methodClose = closeMenuCommon;
    popMenuSet = pop;
}

void freeMenuSet()
{
    delete popMenuSet;
    delete imgMenuSetBtn;
    delete stMenuSet;
}

void drawMenuSetBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 9; i++)
        imgMenuSetBtn[i]->setTexObject(i == popMenuSet->selected);

    stMenuSet->setString("%d %d %d %d %d",
                         language, onoffBGM, onoffSFX, filterNum);
}

void drawMenuSet(float dt)
{
    popMenuSet->paint(dt);
}

void showMenuSet(bool show)
{
	popMenuSet->show(show);
}

bool keyMenuSet(iKeyState state, iPoint p)
{
    if (popMenuSet->bShow == false)
        return false;
    if (popMenuSet->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
			break;

	case iKeyStateMoved:
        for (i = 0; i < 9; i++)
        {
            if (containPoint(p, imgMenuSetBtn[i]->touchRect(popMenuSet->closePoint)))
            {
                j = i;
                break;
            }
        }
        if (popMenuSet->selected != j)
        {
            playSfxSound(0);
            popMenuSet->selected = j;
        }
		break;

	case iKeyStateEnded:
        i = popMenuSet->selected;
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
                showMenuSet(false);
            }
        popMenuSet->selected = -1;
        break;
	}
	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+             MenuGoIntro              +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popMenuGoIntro;
iImage** imgMenuGoIntroBtn;
iStrTex* stMenuGoIntro;

void drawMenuGoIntBefore(iPopup* pop, float dt, float rate);

void createMenuGoIntro()
{
    iImage* img; iSize size;
    iPopup* pop = new iPopup();

    stMenuGoIntro = new iStrTex(methodGoIntro);
    stMenuGoIntro->setString("%d", language);
    img = new iImage();
    img->addObject(stMenuGoIntro->tex);
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

    imgMenuGoIntroBtn = methodStrBtn(strBtn, positionBtn, size, 2);
    for (int i = 0; i < 2; i++)
        pop->addObject(imgMenuGoIntroBtn[i]);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
    pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
    pop->methodBefore = drawMenuGoIntBefore;
    pop->methodClose = closeMenuCommon;
    popMenuGoIntro = pop;
}

void freeMenuGoIntro()
{
    delete stMenuGoIntro;
    delete imgMenuGoIntroBtn;
    delete popMenuGoIntro;
}

void drawMenuGoIntBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 2; i++)
        imgMenuGoIntroBtn[i]->setTexObject(i == popMenuGoIntro->selected);

    stMenuGoIntro->setString("%d", language);
}

void drawMenuGoIntro(float dt)
{
    popMenuGoIntro->paint(dt);
}

void showMenuGoIntro(bool show)
{
    popMenuGoIntro->show(show);
}

bool keyMenuGoIntro(iKeyState state, iPoint p)
{
    if (popMenuGoIntro->bShow == false)
        return false;
    if (popMenuGoIntro->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgMenuGoIntroBtn[i]->touchRect(popMenuGoIntro->closePoint)))
            {
                j = i;
                break;
            }
        }
            if (popMenuGoIntro->selected != j)
            {
                playSfxSound(0);
                popMenuGoIntro->selected = j;
            }
		break;

	case iKeyStateEnded:
        i = popMenuGoIntro->selected;
        if (i == -1)
            break;

        if (i == 0)
        {
            setLoading(gs_intro, freeMenu, loadIntro);
        }
        else// if (i == 1)
        {
            showMenuGoIntro(false);
        }
        popMenuGoIntro->selected = -1;
        break;
	}
	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               MenuExit               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popMenuExit;
iImage** imgMenuExitBtn;
iStrTex* stMenuExit;

void drawMenuExitBefore(iPopup* pop, float dt, float rate);

void createMenuExit()
{
    iImage* img; iSize size;
    iPopup* pop = new iPopup();

    stMenuExit = new iStrTex(methodExit);
    stMenuExit->setString("%d", language);
    img = new iImage();
    img->addObject(stMenuExit->tex);
    pop->addObject(img);

    const char* strBtn[2] =
        {
            btn.o,
            btn.x
        };

    float w = devSize.width * 0.8f;
    float h = devSize.height * 0.8f;
    iPoint positionBtn[2] = {
            {w / 2 - 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 - 20 * SIZE_RATE},
            {w / 2 + 50 * SIZE_RATE - 20 * SIZE_RATE, h / 2 - 20 * SIZE_RATE},
    };

    size = iSizeMake(40 * SIZE_RATE, 40 * SIZE_RATE);
    imgMenuExitBtn = methodStrBtn(strBtn, positionBtn, size, 2);
    for (int i = 0; i < 2; i++)
        pop->addObject(imgMenuExitBtn[i]);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(devSize.width * 0.1f, devSize.height);
    pop->closePoint = iPointMake(devSize.width * 0.1f, devSize.height * 0.1f);
    pop->methodBefore = drawMenuExitBefore;
    pop->methodClose = closeMenuCommon;
    popMenuExit = pop;
}

void freeMenuExit()
{
    delete popMenuExit;
    delete imgMenuExitBtn;
    delete stMenuExit;
}

void drawMenuExitBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 2; i++)
        imgMenuExitBtn[i]->setTexObject(i == popMenuExit->selected);

    stMenuExit->setString("%d", language);
}

void drawMenuExit(float dt)
{
    popMenuExit->paint(dt);
}

void showMenuExit(bool show)
{
    popMenuExit->show(show);
}

bool keyMenuExit(iKeyState state, iPoint p)
{
    if (popMenuExit->bShow == false)
        return false;
    if (popMenuExit->stat != iPopupProc)
        return true;

	int i, j = -1;

	switch (state)
	{
	case iKeyStateBegan:
		break;

	case iKeyStateMoved:
        for (i = 0; i < 2; i++)
        {
            if (containPoint(p, imgMenuExitBtn[i]->touchRect(popMenuExit->closePoint)))
            {
                j = i;
                break;
            }
        }
            if (popMenuExit->selected != j)
            {
                playSfxSound(0);
                popMenuExit->selected = j;
            }
		break;

	case iKeyStateEnded:
        i = popMenuExit->selected;
        if (i == -1)
            break;

        if (i == 0)
        {
            finishApp();
            freeLoading();
        }
        else// if (i == 1)
        {
            showMenuExit(false);
        }
        popMenuExit->selected = -1;
		break;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               ResetBtn               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popResetBtn;
iImage* imgResetBtn;
iStrTex** stResetBtn;

void drawResetBtnBefore(iPopup* pop, float dt, float rate);

void createResetBtn()
{
    iPopup* pop = new iPopup();

    imgResetBtn = new iImage();
    stResetBtn = new iStrTex * [2];
    for (int i = 0; i < 2; i++)
    {
        if (i == 0) setStringRGBA(RGB_ORANGE, 1.0f);
        else setStringRGBA(RGB_MINT, 1.0f);
        stResetBtn[i] = new iStrTex(methodResetBtn);
        stResetBtn[i]->setString("%d", language);

        imgResetBtn->addObject(stResetBtn[i]->tex);
    }
    pop->addObject(imgResetBtn);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(-150, devSize.height - 80);
    pop->closePoint = iPointMake(20, devSize.height - 80);
    pop->methodBefore = drawResetBtnBefore;
    popResetBtn = pop;
}

void freeResetBtn()
{
    for(int i = 0; i < 2; i++)
        delete stResetBtn[i];
    delete stResetBtn;
    delete popResetBtn;
}

void drawResetBtnBefore(iPopup* pop, float dt, float rate)
{
    for (int i = 0; i < 2; i++)
    {
        if (i == 0) setStringRGBA(RGB_ORANGE, 1.0f);
        else setStringRGBA(RGB_MINT, 1.0f);
        imgResetBtn->setTexObject(0 == popResetBtn->selected);
        stResetBtn[i]->setString("%d", language);
    }
}

void drawResetBtn(float dt)
{
    popResetBtn->paint(dt);
}

void showResetBtn(bool show)
{
    popResetBtn->show(show);
}

bool keyResetBtn(iKeyState state, iPoint mouse)
{
    if (popResetBtn->bShow == false)
        return false;
    if (popResetBtn->stat != iPopupProc)
        return true;

    int i, j = -1;

    switch (state)
    {
        case iKeyStateBegan:
            break;

        case iKeyStateMoved:
            for (i = 0; i < 2; i++)
            {
                if (containPoint(mouse, imgResetBtn->touchRect(popResetBtn->closePoint)))
                {
                    j = i;
                    break;
                }
            }
            if (popResetBtn->selected != j)
            {
                playSfxSound(0);
                popResetBtn->selected = j;
            }
            break;

        case iKeyStateEnded:
            i = popResetBtn->selected;
            if (i == -1)
                break;

            if (i == 0)
            {
                bestTime = 0.0f;
                setBestTime(bestTime);
            }
            popResetBtn->selected = -1;
            break;
    }

    return true;
}

//++++++++++++++++++++++++++++++++++++++++
//+               BestTime               +
//++++++++++++++++++++++++++++++++++++++++
iPopup* popBestTime;
iImage* imgBestTime;
iStrTex* stBestTime;

void drawBestTimeBefore(iPopup* pop, float dt, float rate);

void createBestTime()
{
    iPopup* pop = new iPopup();

    stBestTime = new iStrTex(methodBestTime);
    stBestTime->setString("%d %.2f", language, bestTime);
    imgBestTime = new iImage();
    imgBestTime->addObject(stBestTime->tex);
    pop->addObject(imgBestTime);

    pop->style = iPopupMove;
    pop->openPoint = iPointMake(-400, devSize.height - 50);
    pop->closePoint = iPointMake(20, devSize.height - 50);
    pop->methodBefore = drawBestTimeBefore;
    popBestTime = pop;
}

void freeBestTime()
{
    delete stBestTime;
    delete popBestTime;
}

void drawBestTimeBefore(iPopup* pop, float dt, float rate)
{
    stBestTime->setString("%d %.2f", language, bestTime);
}

void drawBestTime(float dt)
{
    popBestTime->paint(dt);
}

void showBestTime(bool show)
{
    popBestTime->show(show);
}

