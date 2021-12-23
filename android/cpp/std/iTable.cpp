#include "iTable.h"

#include "iStd.h"

static Texture* texTable = NULL;

void iTableBar::paint(	float dt,
						iPoint off,
						iPoint offMin,
						iPoint offMax,
						float alpha)
{
	Texture* t = texTable;

	if (horizontal)
	{
		// rt.size.width x 2
		setRGBA(0.5, 0.5, 0.5, alpha);
		drawImage(t, rt.origin.x, rt.origin.y + rt.size.height / 2, VCENTER | LEFT,
			0, 0, t->width, t->height, rt.size.width / t->width, 2 / t->height, 2, 0);

		// 50 x rt.size.height
		setRGBA(1, 1, 1, alpha);
		float x = rt.origin.x + (rt.size.width - 50) *
								(-off.x - offMax.x) / (-offMin.x - offMax.x);
		drawImage(t, x, rt.origin.y + rt.size.height / 2, VCENTER | LEFT,
			0, 0, t->width, t->height, 50 / t->width, rt.size.height / t->height, 2, 0);
	}
	else
	{
		// 2 x rt.size.height
		setRGBA(0.5, 0.5, 0.5, alpha);
		drawImage(t, rt.origin.x + rt.size.width / 2, rt.origin.y, TOP | HCENTER,
			0, 0, t->width, t->height, 2 / t->width, rt.size.height / t->height, 2, 0);

		// rt.size.width x 50
		setRGBA(1, 1, 1, alpha);
		float y = rt.origin.y + (rt.size.height - 50) *
								(-off.y - offMax.y) / (-offMin.y - offMax.y);
		drawImage(t, rt.origin.x + rt.size.width / 2, y, TOP | HCENTER,
			0, 0, t->width, t->height, rt.size.width / t->width, 50 / t->height, 2, 0);
	}
}


iTable::iTable()
{
	if (texTable == NULL)
	{
		iGraphics* g = iGraphics::share();
		iSize size = iSizeMake(32, 32);
		g->init(size);

		setRGBA(1, 1, 1, 1);
		g->fillRect(0, 0, size.width, size.height, size.width / 2);

		texTable = g->getTexture();
	}

	methodDraw = NULL;
	methodSelect = NULL;
	rt = iRectMake(0, 0, 0, 0);
	off = iPointZero;

#if TABLE_USE_FBO
	texFBO = NULL;
#endif
	imgBtn = NULL;
	numBtn = 0;
	selectedBtn = -1;

	touching = false;
	prevPoint = iPointZero;
	prevTime = iFPS::instance()->now;

	vector = iPointMake(1, 0);
	force = 0.0f;

	tableBar = new iTableBar[4];
	tableBarNum = 0;
	tableBarAlpha = 0.0f;
}

iTable::~iTable()
{
	if (texTable)
	{
		freeImage(texTable);
		texTable = NULL;
	}

#if TABLE_USE_FBO
	freeImage(texFBO);
#endif

	delete tableBar;
}

void iTable::init(	iRect rt,
					iSize sz,
					iTable_Method_Draw mDraw,
					iTable_Method_Select mSelect,
					iImage** imgs, int num)
{
	this->rt = rt;
	methodDraw = mDraw;
	methodSelect = mSelect;
#if TABLE_USE_FBO
	texFBO = createTexture(rt.size.width, rt.size.height);
#endif
	imgBtn = imgs;
	numBtn = num;

	off = iPointZero;
	offMax = iPointZero;
	offMin = iPointMake(rt.size.width - sz.width,
						rt.size.height - sz.height);
}

bool iTable::addBar(bool horizontal, iRect rt)
{
	if (tableBarNum == 4)
		return false;

	iTableBar* tb = &tableBar[tableBarNum];
	tableBarNum++;

	tb->horizontal = horizontal;
	memcpy(&tb->rt, &rt, sizeof(iRect));

	return true;
}

void iTable::paint(float dt, iPoint p)
{
	// draw 
	if (methodDraw)
	{
#if TABLE_USE_FBO
		fbo->bind(texFBO);
		methodDraw(dt, off);
		//for (int i = 0; i < tableBarNum; i++)
		//	tableBar[i].paint(dt, off, offMin, offMax);
		fbo->unbind();

		setGLBlend(GLBlendMultiplied);
		p += rt.origin;
		drawImage(texFBO, p.x, p.y, TOP | LEFT,
			0, 0, texFBO->width, texFBO->height, 1.0F, 1.0F, 2, 0,
			REVERSE_HEIGHT);
		setGLBlend(GLBlendAlpha);
#else
		setClip(rt.origin.x, rt.origin.y, rt.size.width, rt.size.height);
		methodDraw(dt, p + rt.origin + off);
		//for (int i = 0; i < tableBarNum; i++)
		//	tableBar[i].paint(dt);
		setClip(0, 0, 0, 0);
#endif
		for (int i = 0; i < tableBarNum; i++)
			tableBar[i].paint(dt, off, offMin, offMax, tableBarAlpha);
		setRGBA(1, 1, 1, 1);

		tableBarAlpha -= dt / 3.0f;
		if (tableBarAlpha < 0.0f)
			tableBarAlpha = 0.0f;
	}

	// ctrl
	if (force)
	{
		off += vector * (force * dt);

		force -= 500 * dt;
		if (force < 0.0f)
			force = 0.0f;
#if 1
		if (off.x < offMin.x)
			off.x = offMin.x;
		else if (off.x > offMax.x)
			off.x = offMax.x;

		if (off.y < offMin.y)
			off.y = offMin.y;
		else if (off.y > offMax.y)
			off.y = offMax.y;
#else// 스프링 효과 구현할것
#endif

		tableBarAlpha = 1.0f;
	}
	else// if (force == 0.0f)
	{

	}
}

bool iTable::key(iKeyState stat, iPoint point)
{
	if (containPoint(point, rt) == false)
		return false;

	int i, j = -1;

	switch (stat) {

	case iKeyStateBegan:
		touching = true;
		prevPoint = point;
		prevTime = iFPS::instance()->now;

		for (i = 0; i < numBtn; i++)
		{
			if (containPoint(point, imgBtn[i]->touchRect(rt.origin + off)))
			{
				j = i;
				break;
			}
		}
		if (selectedBtn != j)
		{
			printf("audioPlay\n");
			imgBtn[i]->setTexObject(1);
			selectedBtn = j;
		}
		break;

	case iKeyStateMoved:
		if (selectedBtn != -1)
		{
			if (iPointDistance(prevPoint, point) > 3)
			{
				imgBtn[selectedBtn]->setTexObject(0);
				selectedBtn = -1;
			}
		}

		if (touching)
		{
#if (OS == OS_WINDOW)
			unsigned long now = iFPS::instance()->now;
			if (now == prevTime);
#elif (OS == OS_ANDROID)
			timeval now = iFPS::instance()->now;
			if (now.tv_sec == prevTime.tv_sec &&
				now.tv_usec == prevTime.tv_usec)
#endif
				break;

			iPoint mp = point - prevPoint;
#if (OS == OS_WINDOW)
			float delta = (now - prevTime) / 1000.f;
#elif (OS == OS_ANDROID)
			float delta = (now.tv_sec - prevTime.tv_sec) +
					(now.tv_usec - prevTime.tv_usec) / 1000000.f;
#endif
			
			float len = iPointLength(mp);
			vector = mp / len;
			force = len / delta;

			prevPoint = point;
			prevTime = now;
		}
		break;

	case iKeyStateEnded:
		if (selectedBtn != -1)
		{
			printf("selectedBtn = %d\n", selectedBtn);
			if (methodSelect)
				methodSelect(selectedBtn);

			imgBtn[selectedBtn]->setTexObject(0);
			selectedBtn = -1;
		}
		touching = false;
		break;
	}

	return true;
}
