#pragma once

#include "iStd.h"

#define gs_intro	0
#define gs_menu		1
#define gs_proc		2
#define gs_ending	3

#define gs_loading 0xff

extern int gameState;

typedef void (*MethodLoad)();

void setLoading(int gameState, MethodLoad free, MethodLoad load);
void freeLoading();
void drawLoading(float dt);

void loadLodingBar();
void freeLodingBar();
void drawLodingBar(float dt);
void showLodingBar(bool show);




