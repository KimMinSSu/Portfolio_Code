#pragma once

#include "iStd.h"

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState state, iPoint p);

extern int indexCloseMenu;
//++++++++++++++++++++++++++++++++++++++++
//+               MenuBtn                +
//++++++++++++++++++++++++++++++++++++++++
extern iPopup** popMenuBtn;
void createMenuBtn();
void freeMenuBtn();
void drawMenuBtn(float dt);
void showMenuBtn(bool show);
bool keyMenuBtn(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuStart              +
//++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++
//+               MenuInfo               +
//++++++++++++++++++++++++++++++++++++++++
extern iPopup* popMenuInfo;
void createMenuInfo();
void freeMenuInfo();
void drawMenuInfo(float dt);
void showMenuInfo(bool show);
bool keyMenuInfo(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuSet                +
//++++++++++++++++++++++++++++++++++++++++
extern iPopup* popMenuSet;
void createMenuSet();
void freeMenuSet();
void drawMenuSet(float dt);
void showMenuSet(bool show);
bool keyMenuSet(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+             MenuGoIntro              +
//++++++++++++++++++++++++++++++++++++++++
extern iPopup* popMenuGoIntro;
void createMenuGoIntro();
void freeMenuGoIntro();
void drawMenuGoIntro(float dt);
void showMenuGoIntro(bool show);
bool keyMenuGoIntro(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+               MenuExit               +
//++++++++++++++++++++++++++++++++++++++++
extern iPopup* popMenuExit;
void createMenuExit();
void freeMenuExit();
void drawMenuExit(float dt);
void showMenuExit(bool show);
bool keyMenuExit(iKeyState state, iPoint p);

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