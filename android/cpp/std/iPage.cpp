#include "iPage.h"

#include "iStd.h"

iPage::iPage()
{
	horizontal = true;
	page = _page = 0;
	rt = iRectMake(0, 0, 0, 0);
	methodChanged = NULL;
	methodDrawBefore = NULL;
	methodDraw = NULL;
	methodDrawAfter = NULL;

	touching = false;
	prevPoint = iPointZero;
	prevTime = iFPS::instance()->now;
	off = iPointZero;
	force = 0.0f;
}

void iPage::init(	bool horizontal,				
					int page,
					iRect rt,
					Method_Page_Changed mChanged,
					Method_Page_Draw mDrawBefore,
					Method_Page_Draw mDraw,
					Method_Page_Draw mDrawAfter)
{
	this->horizontal = horizontal;
	_page = page;
	this->rt = rt;
	methodChanged = mChanged;
	methodDrawBefore = mDrawBefore;
	methodDraw = mDraw;
	methodDrawAfter = mDrawAfter;
}

iPage::~iPage()
{
}

void iPage::paint(float dt, iPoint p)
{
	if (methodDrawBefore)
		methodDrawBefore(dt);
	if (methodDraw)
		methodDraw(dt);
	if (methodDrawAfter)
		methodDrawAfter(dt);
#if (OS == OS_WINDOW)
	if (horizontal)
	{
		if (keyDown & keysLeft)
			force = -800;
		else if (keyDown & keysRight)
			force = 800;
	}
	else
	{
		if (keyDown & keysUp)
			force = -800;
		else if (keyDown & keysDown)
			force = 800;
	}
#endif

	if (touching)
		return;
	
	int newPage = page;
	if (horizontal)
	{
		off.x += force * dt;

		int x = off.x * -1 ;
		int dx = rt.size.width;
		newPage = (x + dx / 2) / dx;
	}
	else
	{
		off.y += force * dt;

		int y = off.y * -1;
		int dy = rt.size.height;
		newPage = (y + dy / 2) / dy;
	}

	if (newPage != page)
	{
		page = newPage;
		if (methodChanged)
			methodChanged(page);
	}

	if (force > 0)
	{
		force -= 500 * dt;
		if (force < 0)
			force = 0.0f;
	}
	else if (force < 0)
	{
		force += 500 * dt;
		if (force > 0)
			force = 0.0f;
	}
	else
	{
		//printf("force==0\n");
		iPoint tOff = iPointZero;
		if (horizontal)
		{
			tOff.x = -rt.size.width * page;
			if (off.x < tOff.x)
			{
				off.x += 500 * dt;
				if (off.x > tOff.x)
					off.x = tOff.x;
			}
			else if(off.x > tOff.x)
			{
				off.x -= 500 * dt;
				if (off.x < tOff.x)
					off.x = tOff.x;
			}
		}
		else
		{
			tOff.y = -rt.size.height * page;
			if (off.y < tOff.y)
			{
				off.y += 500 * dt;
				if (off.y > tOff.y)
					off.y = tOff.y;
			}
			else if (off.y > tOff.y)
			{
				off.y -= 500 * dt;
				if (off.y < tOff.y)
					off.y = tOff.y;
			}
		}
	}
}

bool iPage::key(iKeyState stat, iPoint point)
{
	if (containPoint(point, rt) == false)
		return false;

	switch (stat) {

	case iKeyStateBegan:
		touching = true;
		moved = false;
		prevPoint = point;
		prevTime = iFPS::instance()->now;
		break;

	case iKeyStateMoved:
		if (touching)
		{
			iPoint mp = point - prevPoint;
			if (iPointLength(mp) > 3)
			{
				float move = horizontal ? mp.x : mp.y;
#if (OS == OS_WINDOW)
				unsigned long now = iFPS::instance()->now;
				float dt = (iFPS::instance()->now - prevTime) / 1000.f;
#elif (OS == OS_ANDROID)
				timeval now = iFPS::instance()->now;
				float delta = (now.tv_sec - prevTime.tv_sec) +
							  (now.tv_usec - prevTime.tv_usec) / 1000000.f;
#endif
				force = move / delta * 0.5f;

				prevPoint = point;
				prevTime = iFPS::instance()->now;

				if (horizontal)
					off.x += move;
				else
					off.y += move;
				moved = true;
			}
		}
		break;

	case iKeyStateEnded:
		touching = false;
		if (moved == false)
		{
			printf("click\n");
		}
		moved = false;
		break;
	}

	return true;
}

