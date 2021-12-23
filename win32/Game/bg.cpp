#include "bg.h"

iFilter* bg;

void loadBg()
{
	if (bg)
		return;

	FilterInfo info
	{
		"Assets/shaderfilter/main.vert",
		{
			"Assets/shaderfilter/bg.frag",
			{NULL, NULL, NULL, NULL},
			{NEAREST, NEAREST, NEAREST, NEAREST},
			{REPEAT, REPEAT, REPEAT, REPEAT}
		}
	};

	bg = new iFilter(&info);
}

void freeBg()
{
	delete bg;
}

void drawBg(float dt)
{
	bg->paint(dt);
}
