#pragma once

#include "../resource.h"
#include <windows.h>
#include "iStd.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

void goFullscreen();
void enforceResolution(int edge, RECT& rect, int win_border_width, int win_border_height);

extern iPoint cursor;
void loadCursor();
void freeCursor();
void drawCursor(float dt);
bool updateCursor(bool inClient);


