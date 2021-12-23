#pragma once
#include "iStd.h"

Texture* methodBar(iSize size, int color);
iImage** methodStrBtn(const char** btn, iPoint* position, iSize sizeBtn, int num);
iImage* methodStrBtn(const char* btn, iPoint position, iSize sizeBtn);

//=======================================================================
Texture* methodIntro();
Texture* methodMenuTitle(const char* str);
Texture* methodMenuBtn(const char* str);
Texture* methodProcTitle(const char* str);
Texture* methodProcBtn(const char* str);

Texture* methodTuto(const char* str);
Texture* methodTutoScript(const char* str);
Texture* methodClearScript(const char* str);
Texture* methodTutoEnd(const char* str);

//=======================================================================
#define _pageInfo 5
extern int pageInfo;
Texture* methodInfo(const char* str);
Texture* methodSet(const char* str);
Texture* methodGoIntro(const char* str);
Texture* methodExit(const char* str);

Texture* methodBestTime(const char* str);
Texture* methodResetBtn(const char* str);

Texture* methodRestart(const char* str);
Texture* methodGoMenu(const char* str);
Texture* methodOver(const char* str);
Texture* methodHidden(const char* str);
