#include "Typing.h"

Typing::Typing()
{
	page = NULL;
	pageIndex = 0;
	pageNum = 0;
	pageCurr = 0;
	_tickDt = 0.0f;

	str = NULL;
	tick = 0;
	tickDt = 0.05f;
}

Typing::~Typing()
{
	if (page == NULL)
		return;

	delete str;
	iString::freeStringLine(page, pageNum);
}

// ---------------------------------------------------------- line 관리
void Typing::divide(const char* s, float w, int ln, float tDt)
{
	int length = 0;
	_tickDt = tDt;

	int lineNum;
	char** line = iStrTex::getStringLineWidth(s, lineNum, w);
	pageNum = (lineNum + 1) / 3;
	page = new char* [pageNum];
	for (int i = 0; i < pageNum; i++)
	{
		char ss[1024];
		if (i == pageNum - 1 && lineNum % 3 == 1)
			strcpy(ss, line[3 * i + 0]);
		else
			sprintf(ss, "%s\n%s\n%s", line[3 * i + 0], line[3 * i + 1], line[3 * i + 2]);

		int len = strlen(ss);
		char* s = new char[len + 1];
		strcpy(s, ss);
		page[i] = s;

		if (length < len)
			length = len;
	}

	pageIndex = 0;
	pageCurr = 0;

	str = new char[length + 1];
}

bool Typing::nextPage()
{
	// skip
	int n = strlen(page[pageCurr]);
	if (tick < n)
		tick = n;

	// next
	if (pageIndex < pageNum - 1)
	{
		pageIndex++;
		return true;
	}
	return false;
}
// ---------------------------------------------------------- str 관리

const char* Typing::get(float dt)
{
	if (pageIndex != pageCurr)
	{
		pageCurr = pageIndex;

		tickDt = 0.0f;
		tick = 0;
	}
	const char* s = page[pageCurr];
	int n = strlen(s);

	if (tick < n)
	{
		tickDt += dt;
		if (tickDt > _tickDt)
		{
			tick += iString::isUnicode(&s[tick]);
			tickDt = 0.0f;
		}
	}

	memcpy(str, s, tick);
	str[tick] = 0;

	return str;
}

