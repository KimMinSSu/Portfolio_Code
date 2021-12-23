#include "Game.h"
#include "Bg.h"

#include "Loading.h"
#include "Tutorial.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Ui.h"
#include "Player.h"
#include "Data.h"
#include "Sound.h"

void loadGame()
{
	loadSound();
	loadData();

	setStringName("Assets/Font/DungGeunMo.ttf");
	gameState = gs_intro;

	loadBg();
	loadTutorial();
	loadIntro();
	loadLodingBar();
	playBgmSound(0);
}

void freeGame()
{
	freeBg();
	switch (gameState)
	{
	case gs_intro:	freeIntro();	break;
	case gs_menu:	freeMenu();		break;
	case gs_proc:	freeProc();		break;
	}
	freeTutorial();
	freeLoading();
}

#if (OS == OS_ANDROID)
bool showTerminate = false;
bool pressHomeB = false;
#endif

static float backDelay;
#define _backDelay 1.0f
void drawGame(float dt)
{
	backDelay += dt;
	drawBg(dt);
	switch (gameState)
	{
	case gs_intro:	drawIntro(dt);	break;
	case gs_menu:	drawMenu(dt);	break;
	case gs_proc:	drawProc(dt);	break;
	}

	drawTutorial(dt);
	drawLoading(dt);

#if (OS == OS_ANDROID)
	if (pressHomeB && gs_proc)
	{
		extern bool startTuto;
		if(startTuto == false)
		{
			procPopShow = true;
			showPopProc(true);
		}
	}

	if( showTerminate )
	{
		if(backDelay < _backDelay)
			return;
		else
			backDelay = 0.0f;

		showTerminate = false;

		if(gameState == gs_menu)
		{
			if (popMenuBtn[0]->bShow)
			{
				indexCloseMenu = 3;
				showMenuBtn(false);
			}
			else if (popMenuInfo->bShow)
			{
				indexCloseMenu = -1;
				showMenuInfo(false);
			}
			else if (popMenuSet->bShow)
			{
				indexCloseMenu = -1;
				showMenuSet(false);
			}
			else if (popMenuGoIntro->bShow)
			{
				indexCloseMenu = -1;
				showMenuGoIntro(false);
			}
			else if (popMenuExit->bShow)
			{
				indexCloseMenu = -1;
				showMenuExit(false);
			}
		}
		else if(gameState == gs_proc)
		{
			if (popProcReStart->bShow)
			{
				indexCloseProc = -1;
				showProcReStart(false);
			}
			else if (popProcInfo->bShow)
			{
				indexCloseProc = -1;
				showProcInfo(false);
			}
			else if (popProcSet->bShow)
			{
				indexCloseProc = -1;
				showProcSet(false);
			}
			else if (popProcGoMenu->bShow)
			{
				indexCloseProc = -1;
				showProcGoMenu(false);
			}

			if(!player->alive)
				return;

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
	}
#endif
}

void keyGame(iKeyState state, iPoint touchP)
{
    if (touchP.x < 0 ||
        touchP.y < 0 ||
        touchP.x > devSize.width ||
        touchP.y > devSize.height)
        return;

    if (keyTutorial(state, touchP))
		return;

	switch (gameState)
	{
	case gs_intro:	keyIntro(state, touchP);	break;
	case gs_menu:	keyMenu(state, touchP);	break;
	case gs_proc:	keyProc(state, touchP);	break;
	}
}

void keyGame(iKeyState state, int touchNum, iPoint* touchP)
{
    for(int i = 0; i < touchNum; i++)
    {
        if (touchP[i].x < 0 ||
            touchP[i].y < 0 ||
            touchP[i].x > devSize.width ||
            touchP[i].y > devSize.height)
            return;
    }

	if (keyTutorial(state, touchNum, touchP))
		return;

	switch (gameState)
	{
		case gs_intro:	keyIntro(state, touchP[0]);	break;
		case gs_menu:	keyMenu(state, touchP[0]);	break;
		case gs_proc:	keyProc(state, touchNum, touchP);	break;
	}
}