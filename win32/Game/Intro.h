#pragma once
#include "iStd.h"

void loadIntro();
void freeIntro();
void drawIntro(float dt);
void keyIntro(iKeyState state, iPoint p);

//+++++++++++++++++++++++++++++
//+          PopIntro         +
//+++++++++++++++++++++++++++++

void createPopIntro();
void freePopIntro();
void showPopIntro(bool show);
void drawPopIntro(float dt);