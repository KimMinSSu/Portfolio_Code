#pragma once

#include "iStd.h"

extern float playTime;
extern float bestTime;
extern bool bPlayTime;

void loadProc();
void freeProc();
void drawProc(float dt);
void keyProc(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+                popProc               +
//++++++++++++++++++++++++++++++++++++++++
void createPopProc();
void freePopProc();
void drawPopProc(float dt);
void showPopProc(bool show);
bool keyPopProc(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+              ProcReStart             +
//++++++++++++++++++++++++++++++++++++++++
void createProcReStart();
void freeProcReStart();
void drawProcReStart(float dt);
void showProcReStart(bool show);
bool keyProcReStart(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               GameInfo               +
//++++++++++++++++++++++++++++++++++++++++
void createProcInfo();
void freeProcInfo();
void drawProcInfo(float dt);
void showProcInfo(bool show);
bool keyProcInfo(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               ProcSet                +
//++++++++++++++++++++++++++++++++++++++++
void createProcSet();
void freeProcSet();
void drawProcSet(float dt);
void showProcSet(bool show);
bool keyProcSet(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               ProcGoMenu             +
//++++++++++++++++++++++++++++++++++++++++
void createProcGoMenu();
void freeProcGoMenu();
void drawProcGoMenu(float dt);
void showProcGoMenu(bool show);
bool keyProcGoMenu(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               popOver                +
//++++++++++++++++++++++++++++++++++++++++
void createPopOver();
void freePopOver();
void drawPopOver(float dt);
void showPopOver(bool show);
bool keyPopOver(iKeyState state, iPoint mouse);

//++++++++++++++++++++++++++++++++++++++++
//+               popHidden              +
//++++++++++++++++++++++++++++++++++++++++
void createPopHidden();
void freePopHidden();
void drawPopHidden(float dt);
void showPopHidden(bool show);
bool keyPopHidden(iKeyState state, iPoint mouse);