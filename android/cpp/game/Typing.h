#pragma once

#include "iStd.h"

class Typing
{
public:
	Typing();
	virtual ~Typing();

	// ---------------------------------------------------------- line 관리
	void divide(const char* s, float w, int ln, float tDt);
	bool nextPage();
	// ---------------------------------------------------------- str 관리

	const char* get(float dt);

public:
	char** page;
	int pageIndex, pageNum;

	int pageCurr;
	char* str;
	int tick;
	float tickDt, _tickDt;
};



