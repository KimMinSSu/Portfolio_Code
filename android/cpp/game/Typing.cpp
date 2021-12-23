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
	pageNum = (lineNum + ln - 1) / ln;
	page = new char* [pageNum];
	for (int i = 0; i < pageNum; i++)
	{
		char ss[1024];
		char sss[1024] = {'\0'};
		if (i == pageNum - 1 && lineNum % ln == 1)
			strcpy(sss, line[ln * i + 0]);
		else
		{
			for (int j = 0; j < ln; j++)
			{
				if(j == ln - 1)
					sprintf(ss, "%s", line[ln * i + j]);
				else
					sprintf(ss, "%s\n", line[ln * i + j]);
				strcat(sss, ss); 
			}
		}

		int len = strlen(sss);
		char* s = new char[len + 1];
		strcpy(s, sss);
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
            //tick += (1 + 2 * iString::isUnicode(&s[tick]));
			tickDt = 0.0f;
		}
	}
	memcpy(str, s, tick);
	str[tick] = 0;
	return str;
}

