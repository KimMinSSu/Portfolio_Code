#include "Game.h"
#include "bg.h"

#include "Loading.h"
#include "Tutorial.h"
#include "Intro.h"
#include "Menu.h"
#include "Proc.h"
#include "Data.h"
#include "Sound.h"

void loadGame()
{
	setStringName("Assets/Font/DungGeunMo.ttf");
	loadSound();

	gameState = gs_intro;

	loadBg();
	loadData();
	loadTutorial();

	loadIntro();
}

void freeGame()
{
	switch (gameState)
	{
	case gs_intro:	freeIntro(); break;
	case gs_menu:	freeMenu();  break;
	case gs_proc:	freeProc();	 break;
	}
	freeTutorial();
	freeLoading();
}

void drawGame(float dt)
{
	drawBg(dt);
	switch (gameState)
	{
	case gs_intro:	drawIntro(dt); break;
	case gs_menu:	drawMenu(dt);  break;
	case gs_proc:	drawProc(dt);  break;
	}
	drawTutorial(dt);
	drawLoading(dt);
}

void keyGame(iKeyState state, iPoint mouse)
{
	if (keyTutorial(state, mouse))
		return;

	switch (gameState)
	{
	case gs_intro:	keyIntro(state, mouse);	break;
	case gs_menu:	keyMenu(state, mouse);	break;
	case gs_proc:	keyProc(state, mouse);  break;
	}
}
