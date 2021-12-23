#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuBtn                +
//++++++++++++++++++++++++++++++++++++++++
void createMenuBtn();
void freeMenuBtn();
void drawMenuBtn(float dt);
void showMenuBtn(bool show);
bool keyMenuBtn(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuInfo               +
//++++++++++++++++++++++++++++++++++++++++
void createMenuInfo();
void freeMenuInfo();
void drawMenuInfo(float dt);
void showMenuInfo(bool show);
bool keyMenuInfo(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuSet                +
//++++++++++++++++++++++++++++++++++++++++
void createMenuSet();
void freeMenuSet();
void drawMenuSet(float dt);
void showMenuSet(bool show);
bool keyMenuSet(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+             MenuGoIntro              +
//++++++++++++++++++++++++++++++++++++++++
void createMenuGoIntro();
void freeMenuGoIntro();
void drawMenuGoIntro(float dt);
void showMenuGoIntro(bool show);
bool keyMenuGoIntro(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuExit               +
//++++++++++++++++++++++++++++++++++++++++
void createMenuExit();
void freeMenuExit();
void drawMenuExit(float dt);
void showMenuExit(bool show);
bool keyMenuExit(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               ResetBtn               +
//++++++++++++++++++++++++++++++++++++++++
void createResetBtn();
void freeResetBtn();
void drawResetBtn(float dt);
void showResetBtn(bool show);
bool keyResetBtn(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               BestTime               +
//++++++++++++++++++++++++++++++++++++++++
void createBestTime();
void freeBestTime();
void drawBestTime(float dt);
void showBestTime(bool show);