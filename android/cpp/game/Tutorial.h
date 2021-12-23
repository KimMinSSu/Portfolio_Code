#pragma once

#include "iStd.h"

void loadTutorial();
void enforceRunTutorial();// start Tutorial in menu
void freeTutorial();
void drawTutorial(float dt);
bool keyTutorial(iKeyState state, iPoint p);
bool keyTutorial(iKeyState state, int touchNum, iPoint* p);

class Tutorial
{
public:
	Tutorial();
	virtual ~Tutorial();

	void paint(float dt);
	bool key(iKeyState state, iPoint p);

public:
	int chapter;

	bool flag[4];
	float flagDt;

	bool tryEnd;
};

extern Tutorial* tutorial;

//++++++++++++++++++++++++++++++++++++++++
//+               PopTuto                +
//++++++++++++++++++++++++++++++++++++++++
void createPopTuto();
void freePopTuto();
void drawPopTuto(float dt);
void showPopTuto(bool show);
bool keyPopTuto(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+              TutoScript              +
//++++++++++++++++++++++++++++++++++++++++
void createTutoScript();
void freeTutoScript();
void drawTutoScript(float dt);
void showTutoScript(bool show);

//++++++++++++++++++++++++++++++++++++++++
//+                TutoBar               +
//++++++++++++++++++++++++++++++++++++++++
void createTutoBar();
void freeTutoBar();
void drawTutoBar(float dt);
void showTutoBar(bool show);

//++++++++++++++++++++++++++++++++++++++++
//+             ClearScript              +
//++++++++++++++++++++++++++++++++++++++++
void createClearScript();
void freeClearScript();
void drawClearScript(float dt);
void showClearScript(bool show);

//++++++++++++++++++++++++++++++++++++++++
//+               TutoSkip               +
//++++++++++++++++++++++++++++++++++++++++
void createTutoSkip();
void freeTutoSkip();
void drawTutoSkip(float dt);
void showTutoSkip(bool show);
bool keyTutoSkip(iKeyState state, iPoint p);

//++++++++++++++++++++++++++++++++++++++++
//+                TutoNum               +
//++++++++++++++++++++++++++++++++++++++++
void createTutoNum();
void freeTutoNum();
void drawTutoNum(float dt);
void showTutoNum(bool show);

//++++++++++++++++++++++++++++++++++++++++
//+                TutoEnd               +
//++++++++++++++++++++++++++++++++++++++++
void createTutoEnd();
void freeTutoEnd();
void drawTutoEnd(float dt);
void showTutoEnd(bool show);
bool keyTutoEnd(iKeyState state, iPoint p);

bool popOpenCheck();