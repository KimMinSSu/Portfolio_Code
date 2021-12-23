#include "bg.h"

iFilter* bg;

void loadBg()
{
	FilterInfo info
	{
		"shaderfilter/main.vert",
		{
		"shaderfilter/bg.frag",
		{NULL, NULL, NULL, NULL},
		{NEAREST, NEAREST, NEAREST, NEAREST},
		{REPEAT, REPEAT, REPEAT, REPEAT}
		}
	};

	bg = new iFilter(&info);
}

void freeBg()
{
	if(bg)
		delete bg;
}

void drawBg(float dt)
{
	if(bg)
		bg->paint(dt);
}
