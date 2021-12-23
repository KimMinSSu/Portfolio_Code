#include "PopMethod.h"

#include "Tutorial.h"
#include "Script.h"
#include "Sound.h"
#include "Data.h"
#include "Proc.h"

Texture* methodBar(iSize size, int color)
{
	iGraphics* g = iGraphics::share();
	g->init(size);

	if (color == 0)
		setRGBA(RGB_GRAY, 1.0f);
	else if (color == 1)
		setRGBA(RGB_RED, 1.0f);
	else if (color == 2)
		setRGBA(RGBA_BLACK);
	else if (color == 3)
		setRGBA(RGB_MINT, 1.0f);
	else if (color == 4)
		setRGBA(RGB_YELLOW, 1.0f);
	else if (color == 5)
		setRGBA(RGB_BLUE, 1.0f);

	g->fillRect(0, 0, size.width, size.height, size.height / 2);
	setRGBA(RGBA_WHITE);

	return g->getTexture();
}

iImage** methodStrBtn(const char** btn, iPoint* position, iSize sizeBtn, int num)
{
	Texture* tex; iImage** btnImg; iImage* img; int i, j;
	iGraphics* g = iGraphics::share();
	iSize size = sizeBtn;

	float strSize = sizeBtn.height * 0.75;

	btnImg = new iImage * [num];
	setStringSize(strSize);

	for (i = 0; i < num; i++)
	{
		img = new iImage();
		for (j = 0; j < 2; j++)
		{
			g->init(size);

			if (j == 0) setStringRGBA(RGB_ORANGE, 1.0f);
			else setStringRGBA(RGB_MINT, 1.0f);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, btn[i]);
			tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = position[i];
		btnImg[i] = img;
	}
	setRGBA(RGBA_WHITE);

	return btnImg;
}

iImage* methodStrBtn(const char* btn, iPoint position, iSize sizeBtn)
{
	Texture* tex; iImage* img;
	iGraphics* g = iGraphics::share();
	iSize size = sizeBtn;

	float strSize = sizeBtn.height * 0.75;
	setStringSize(strSize);

	img = new iImage();
	for (int i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0) setStringRGBA(RGB_ORANGE, 1.0f);
		else setStringRGBA(RGB_MINT, 1.0f);
		g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, btn);
		tex = g->getTexture();

		img->addObject(tex);
		freeImage(tex);
	}
	img->position = position;
	setRGBA(RGBA_WHITE);

	return img;
}

//=======================================================================
Texture* methodIntro()
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize;

	g->init(size);

	setStringSize(60);
	setStringRGBA(RGB_BLUE, 1.0f);
	g->drawString(size.width / 2, size.height * 0.2f, VCENTER | HCENTER, script[language].strWords.title);

	setStringSize(30);
	setStringRGBA(RGB_MINT, 1.0f);
	g->drawString(size.width / 2, size.height * 0.8f, VCENTER | HCENTER, script[language].strWords.hello);

	return g->getTexture();
}

Texture* methodMenuTitle(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(300, 70);

	g->init(size);

	setStringSize(60);
	setStringRGBA(RGB_BLUE, 1.0f);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	return g->getTexture();
}

Texture* methodMenuBtn(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(200, 40);

	g->init(size);

	setStringSize(30);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	return g->getTexture();
}

Texture* methodProcTitle(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(300, 70);

	g->init(size);

	setStringSize(60);
	setStringRGBA(RGB_BLUE, 1.0f);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	return g->getTexture();
}

Texture* methodProcBtn(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(200, 40);

	g->init(size);

	setStringSize(30);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	return g->getTexture();
}

Texture* methodTuto(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize * 0.8f;
	g->init(size);

	setStringSize(30);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.askTuto1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		script[language].strWords.askTuto2);

	return g->getTexture();
}

Texture* methodTutoScript(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize;
	g->init(size);

	setStringSize(30);
	setStringRGBA(RGB_ORANGE, 1.0f);

	if (3 <= tutorial->chapter && tutorial->chapter <= 5)
		g->drawString(size.width / 2, 20, VCENTER | HCENTER,
			script[language].tutoChapter.tuto1);
	else if (7 <= tutorial->chapter && tutorial->chapter <= 9)
		g->drawString(size.width / 2, 20, VCENTER | HCENTER,
			script[language].tutoChapter.tuto2);
	else if (11 <= tutorial->chapter && tutorial->chapter <= 14)
		g->drawString(size.width / 2, 20, VCENTER | HCENTER,
			script[language].tutoChapter.tuto3);

	return g->getTexture();
}

Texture* methodClearScript(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize;
	g->init(size);

	setStringSize(30);
	setStringRGBA(RGB_ORANGE, 1.0f);

	g->drawString(size.width / 2, 20, VCENTER | HCENTER,
		script[language].tutoChapter.clear);

	return g->getTexture();
}

Texture* methodTutoEnd(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize * 0.8f;
	g->init(size);

	setStringSize(30);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.clearTuto1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		script[language].strWords.clearTuto2);

	return g->getTexture();
}

//=======================================================================
int pageInfo;

Texture* methodInfo(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(20);
	setStringRGBA(RGB_ORANGE, 1.0f);

	const char* strP1[5] = {
		script[language].infoP1.text1,
		script[language].infoP1.text2,
		script[language].infoP1.text3,
		script[language].infoP1.text4,
		script[language].infoP1.text5,
	};

	const char* strP2[6] = {
		script[language].infoP2.text1,
		script[language].infoP2.text2,
		script[language].infoP2.text3,
		script[language].infoP2.text4,
		script[language].infoP2.text5,
		script[language].infoP2.text6,
	};

	const char* strP3[7] = {
		script[language].infoP3.text1,
		script[language].infoP3.text2,
		script[language].infoP3.text3,
		script[language].infoP3.text4,
		script[language].infoP3.text5,
		script[language].infoP3.text6,
		script[language].infoP3.text7,
	};

	const char* strP4[5] = {
		script[language].infoP4.text1,
		script[language].infoP4.text2,
		script[language].infoP4.text3,
		script[language].infoP4.text4,
		script[language].infoP4.text5,
	};

	const char* strP5[6] = {
		script[language].infoP5.text1,
		script[language].infoP5.text2,
		script[language].infoP5.text3,
		script[language].infoP5.text4,
		script[language].infoP5.text5,
		script[language].infoP5.text6,
	};

	int i;
	int p = atoi(str);
	if (p == 0)
	{
		for (i = 0; i < 5; i++)
			g->drawString(size.width * 0.1f, 30 + 40 * i, TOP | LEFT, strP1[i]);
	}
	else if (p == 1)
	{
		for (i = 0; i < 6; i++)
			g->drawString(size.width * 0.1f, 30 + 40 * i, TOP | LEFT, strP2[i]);
	}
	else if (p == 2)
	{
		for (i = 0; i < 7; i++)
			g->drawString(i == 0 ? size.width * 0.1f : size.width * 0.2f, 30 + 40 * i, TOP | LEFT, strP3[i]);
	}
	else if (p == 3)
	{
		g->drawString(size.width * 0.1f, 30, TOP | LEFT, strP4[0]);
		g->drawString(size.width * 0.1f, 110, TOP | LEFT, strP4[1]);
		g->drawString(size.width * 0.1f, 150, TOP | LEFT, strP4[2]);
		g->drawString(size.width * 0.1f, 230, TOP | LEFT, strP4[3]);
		g->drawString(size.width * 0.1f, 270, TOP | LEFT, strP4[4]);
	}
	else if (p == 4)
	{
		for (i = 0; i < 6; i++)
			g->drawString(size.width * 0.1f, 30 + 40 * i, TOP | LEFT, strP5[i]);
	}
	g->drawString(size.width / 2, size.height - 20, VCENTER | HCENTER, "%d / %d", 1 + p, _pageInfo);

	return g->getTexture();
}

extern bool isFullscreen;
extern int filterNum;
Texture* methodSet(const char* str)
{
	int i; iPoint p;
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(25);
	setStringRGBA(RGB_ORANGE, 1.0f);

	for (i = 0; i < 5; i++)
	{
		p = iPointMake(size.width * 0.25f, size.height * 0.2f + 15 + 50 * i);
		switch (i)
		{
			case 0:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.size);
				break;
			case 1:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.select);
				break;
			case 2:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.bgm);
				break;
			case 3:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.sfx);
				break;
			case 4:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.filter);
				break;
		}
	}

	for (i = 0; i < 5; i++)
	{
		p = iPointMake(size.width * 0.625f, size.height * 0.2f + 15 + 50 * i);
		switch (i)
		{
			case 0:
				g->drawString(p.x, p.y, VCENTER | HCENTER,
					isFullscreen ? script[language].strSet.on : script[language].strSet.off);
				break;
			case 1:
				g->drawString(p.x, p.y, VCENTER | HCENTER, script[language].strSet.language);
				break;
			case 2:
					g->drawString(p.x, p.y, VCENTER | HCENTER, 
						onoffBGM ? script[language].strSet.on : script[language].strSet.off);
				break;
			case 3:
				g->drawString(p.x, p.y, VCENTER | HCENTER,
					onoffSFX ? script[language].strSet.on : script[language].strSet.off);
				break;
			case 4:
				if (filterNum == 0)
					g->drawString(p.x, p.y, VCENTER | HCENTER,"%d", filterNum + 1);
				else if (filterNum == 1)					  
					g->drawString(p.x, p.y, VCENTER | HCENTER,"%d", filterNum + 1);
				else if (filterNum == 2)
					g->drawString(p.x, p.y, VCENTER | HCENTER, "%d", filterNum + 1);
				break;
		}
	}

	return g->getTexture();
}

Texture* methodGoIntro(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(40);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.askGoIntro1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		script[language].strWords.askGoIntro2);

	return g->getTexture();
}

Texture* methodExit(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(40);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.askGoExit);

	return g->getTexture();
}

Texture* methodBestTime(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(350, 40);

	g->init(size);

	setStringSize(20);
	setStringRGBA(RGB_ORANGE, 1.0f);
	bestTime = getBestTime();
	g->drawString(0, size.height / 2, VCENTER | LEFT, script[language].strWords.bestTime, bestTime);

	return g->getTexture();
}

Texture* methodResetBtn(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(150, 40);

	g->init(size);

	setStringSize(20);
	g->drawString(0, size.height / 2, VCENTER | LEFT, script[language].strBtn.reset);

	return g->getTexture();
}

Texture* methodRestart(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(40);
	setStringRGBA(RGB_ORANGE, 1.0f);

	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.askRestart1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		script[language].strWords.askRestart2);

	return g->getTexture();
}

Texture* methodGoMenu(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(devSize.width, devSize.height) * 0.8f;
	g->init(size);

	setStringSize(40);
	setStringRGBA(RGB_ORANGE, 1.0f);
	g->drawString(size.width / 2, size.height / 2 - 60, VCENTER | HCENTER,
		script[language].strWords.askGoMenu1);
	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER,
		script[language].strWords.askGoMenu2);

	return g->getTexture();
}

extern float playTime;
extern float bestTime;
Texture* methodOver(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = devSize * 0.8f;
	g->init(size);

	float h = size.height * 0.1f;
	setStringSize(40);
	setStringRGBA(RGB_BLUE, 1.0f);
	g->drawString(size.width / 2, h, VCENTER | HCENTER, script[language].strWords.gameOver);

	setStringRGBA(RGB_SKY, 1.0f);
	g->drawString(size.width / 2, h + 50, VCENTER | HCENTER, script[language].strWords.bestTime, bestTime);
	g->drawString(size.width / 2, h + 100, VCENTER | HCENTER, script[language].strWords.record, playTime);

	return g->getTexture();
}

Texture* methodHidden(const char* str)
{
	int i; iPoint p;
	iGraphics* g = iGraphics::share();
	iSize size = devSize * 0.8f;
	g->init(size);

	setStringSize(40); 
	setStringRGBA(RGB_BLUE, 1.0f);
	g->drawString(size.width / 2, 75, VCENTER | HCENTER, script[language].strWords.please);

	return g->getTexture();
}
