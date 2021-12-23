#include "Loading.h"
#include "PopMethod.h"

int gameState = gs_intro, toGameState;
MethodLoad methodFree, methodLoad;
float loadingDt;
#define _loadingDt 0.5f

void setLoading(int gameState, MethodLoad free, MethodLoad load)
{
	if (loadingDt)
		return;

	toGameState = gameState;
	methodFree = free;
	methodLoad = load;
	loadingDt = 0.000001f;

	loadLodingBar();
}

void freeLoading()
{
	freeLodingBar();
}

void drawLoading(float dt)
{
	if (loadingDt == 0.0f)
		return;

	float a = 1.0f;

	if (loadingDt < _loadingDt)
	{
		loadingDt += dt;
		if (loadingDt > _loadingDt)
			loadingDt = _loadingDt;

		a = loadingDt / _loadingDt;// 점점 어두워진다.
	}
	else if (loadingDt == _loadingDt)
	{
		showLodingBar(true);

		loadingDt = _loadingDt + 0.000001f;

		if (methodFree)
			methodFree();

		gameState = 10000000;

		a = 1.0f;// 완전 까맣게
	}
	else if (loadingDt < _loadingDt + 1.0f)
	{
		loadingDt += dt;

		if (loadingDt >= _loadingDt + 1.0f)
		{
			if (methodLoad)
				methodLoad();

			gameState = toGameState;

			loadingDt = _loadingDt + 1.0f;
		}
		a = 1.0f;// 완전 까맣게
	}
	else if (loadingDt < _loadingDt * 2 + 1.0f)
	{
		showLodingBar(false);

		loadingDt += dt;
		if (loadingDt > _loadingDt * 2 + 1.0f)
		{
			loadingDt = 0.0f;
			printf("loadingDt zero\n");
			return;
		}

		a = 1.0f - (loadingDt - _loadingDt - 1.0f) / _loadingDt;// 점점 밝아진다.
	}

	printf("%f\n", loadingDt);
	setRGBA(0, 0, 0, a);
	fillRect(-1, -1, devSize.width + 2, devSize.height + 2);
	setRGBA(1, 1, 1, 1);

	drawLodingBar(dt);
}

iPopup* loadingBar;
Texture* loadingGauge;
iStrTex* gPercent;

float loadingbarDt;
#define _loadingbarDt 0.8f

void drawLodingBarAfter(iPopup* pop, float dt, float rate);

Texture* methodPercent(const char* str)
{
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(100, 40);
	g->init(size);

	setStringSize(25);
	setStringRGBA(RGBA_BLACK);
	setStringBorder(5);
	setStringBorderRGBA(RGBA_WHITE);

	g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, str);

	return g->getTexture();
}

void loadLodingBar()
{
	Texture* tex; iImage* img; iSize size;
	iPopup* pop = new iPopup();

	size = iSizeMake(devSize.width * 0.7f + 8, 30 + 8);
	tex = methodBar(size, 0);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	size = iSizeMake(devSize.width * 0.7f, 30);
	tex = methodBar(size, 2);
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(4, 4);
	pop->addObject(img);

	pop->closePoint = iPointMake(devSize.width * 0.15f - 4, devSize.height * 0.8f);
	pop->methodAfter = drawLodingBarAfter;
	loadingBar = pop;

	loadingbarDt = 0.0f;
	loadingGauge = methodBar(size, 3);

	gPercent = new iStrTex(methodPercent);
	gPercent->setString("0");
}

void freeLodingBar()
{
	if (loadingGauge)
		freeImage(loadingGauge);
	
	if (gPercent)
		delete gPercent;
	
	if (loadingBar)
		delete loadingBar;
}

void drawLodingBar(float dt)
{
	loadingBar->paint(dt);
}


void drawLodingBarAfter(iPopup* pop, float dt, float rate)
{
	if (pop->stat == iPopupOpen)
		return;

	loadingbarDt += dt;

	iPoint p = pop->closePoint + 4;

	float w = linear(loadingbarDt / _loadingbarDt, 0, loadingGauge->width);
	if (loadingbarDt > _loadingbarDt)
	{
		loadingbarDt = _loadingbarDt;
		w = loadingGauge->width;
	}

	drawImage(loadingGauge, p.x, p.y, TOP | LEFT, 0, 0,
		w, loadingGauge->height,
		1, 1, 0, 0);
	gPercent->paint(p.x + loadingGauge->width / 2, p.y + loadingGauge->height / 2, VCENTER | HCENTER, 
		"％%.f", (loadingbarDt / _loadingbarDt) * 100);
}

void showLodingBar(bool show)
{
	loadingBar->show(show);
}
