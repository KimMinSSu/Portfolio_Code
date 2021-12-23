#pragma once

enum Language
{
	Kor = 0,
	En,
	Jp,

	LanguageMax
};

struct StrWords
{
	const char* title;
	const char* hello;
	const char* press;

	const char* pause;

	const char* gameOver;
	const char* record;
	const char* bestTime;
	const char* please;

	const char* askGoIntro;
	const char* askGoIntro1;
	const char* askGoIntro2;

	const char* askGoMenu;
	const char* askGoMenu1;
	const char* askGoMenu2;

	const char* askRestart;
	const char* askRestart1;
	const char* askRestart2;

	const char* askGoExit;

	const char* askTuto;
	const char* askTuto1;
	const char* askTuto2;

	const char* clearTuto;
	const char* clearTuto1;
	const char* clearTuto2;

	const char* ready;
};

struct InfoPage1
{
	const char* text1;
	const char* text2;
	const char* text3;
	const char* text4;
	const char* text5;
};

struct InfoPage2
{
	const char* text1;
	const char* text2;
	const char* text3;
	const char* text4;
	const char* text5;
	const char* text6;
};

struct InfoPage3
{
	const char* text1;
	const char* text2;
	const char* text3;
	const char* text4;
	const char* text5;
	const char* text6;
	const char* text7;
};

struct InfoPage4
{
	const char* text1;
	const char* text2;
	const char* text3;
	const char* text4;
	const char* text5;
};

struct InfoPage5
{
	const char* text1;
	const char* text2;
	const char* text3;
	const char* text4;
	const char* text5;
	const char* text6;
};

struct StrSet
{
	const char* select;
	const char* language;
	const char* bgm;
	const char* sfx;
	const char* on;
	const char* off;
	const char* size;
	const char* filter;
};

struct StrBtn
{
	const char* start;
	const char* tuto;
	const char* info;
	const char* set;
	const char* goIntro;
	const char* exit;
	const char* filter;

	const char* continueGame;
	const char* restartGame;
	const char* goMenu;

	const char* reset;
	const char* skip;
};

struct TutoChapter
{
	const char* tuto1;
	const char* tuto2;
	const char* tuto3;

	const char* clear;
};

struct Script
{
	StrWords strWords;
	InfoPage1 infoP1;
	InfoPage2 infoP2;
	InfoPage3 infoP3;
	InfoPage4 infoP4;
	InfoPage5 infoP5;
	StrSet strSet;
	StrBtn strBtn;
	TutoChapter tutoChapter;
};

struct Btn
{
	const char* o;
	const char* x;
	const char* left;
	const char* right;
};

extern int language;
extern Script script[LanguageMax];
extern Btn btn;
