#pragma once
#include "iStd.h"

void loadGame();
void freeGame();
void drawGame(float dt);
void keyGame(iKeyState state, iPoint p);
void keyGame(iKeyState state, int touchNum, iPoint* touchP);
