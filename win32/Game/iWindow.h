#pragma once
#include "App.h"

void startApp(HDC hdc, VOID_METHOD methodLoad);
void endApp(VOID_METHOD methodFree);
void drawApp(FLOAT_METHOD  methodDraw);
void resizeApp(int width, int height);
iPoint coordinate(int x, int y);





