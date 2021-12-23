#pragma once

#include "iStd.h"

extern float uiDt;

void loadUI();
void freeUI();
void drawUI(float dt, float _dt);

//+++++++++++++++++++++++++++++
//+          HpBar            +
//+++++++++++++++++++++++++++++
void createHpBar();
void freeHpBar();
void drawHpBar(float dt);
void showPopHpBar(bool show);

//+++++++++++++++++++++++++++++
//+          Stamina          +
//+++++++++++++++++++++++++++++
void createStamina();
void freeStamina();
void drawStamina(float dt);
void showPopStamina(bool show);

//+++++++++++++++++++++++++++++
//+           Clock           +
//+++++++++++++++++++++++++++++
extern float sizeDt;

void createClock();
void freeClock();
void drawClock(float dt);

//+++++++++++++++++++++++++++++
//+           Curse           +
//+++++++++++++++++++++++++++++
void createCurse();
void freeCurse();
void drawCurse(float dt);
