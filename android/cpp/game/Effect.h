#pragma once

#include "iStd.h"

void loadEffect();
void freeEffect();

//+++++++++++++++++++++++++++++
//+          Blind            +
//+++++++++++++++++++++++++++++
void createBlind();
void freeBlind();
void drawBlind(float dt, iPoint off);

//+++++++++++++++++++++++++++++
//+        Explosion          +
//+++++++++++++++++++++++++++++
void createExplosion();
void freeExplosion();
void drawExplosion(float dt, iPoint off);
