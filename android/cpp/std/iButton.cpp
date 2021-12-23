#include "iButton.h"

#include "iStd.h"

iButton::iButton(iImage* _img, iButton_Method_Click method, void* parm)
{
	methodClick = method;
	parmClick = parm;

	img = _img->copy();
	if (img->anc != (VCENTER | HCENTER))
	{
		Texture* t = img->tex;
		if (img->anc & LEFT)
			img->position.x += t->width / 2;
		else if (img->anc & RIGHT)
			img->position.x -= t->width / 2;

		if (img->anc & TOP)
			img->position.y += t->height / 2;
		else if (img->anc & BOTTOM)
			img->position.y -= t->height / 2;

		img->anc = VCENTER | HCENTER;
	}
	pushed = false;
	_aniDt = iButton_AniDt;
	aniDt = 0.0f;
}

iButton::~iButton()
{
	delete img;
}

void iButton::paint(float dt, iPoint off)
{
	if (pushed)
	{
		aniDt += dt;
		if (aniDt > _aniDt)
			aniDt = _aniDt;
	}
	else// if (pushed == false)
	{
		aniDt -= dt;
		if (aniDt < 0.0f)
			aniDt = 0.0f;
	}
	float r = linear(aniDt / _aniDt, 1.0f, 0.7f);

	img->paint(dt, off, iPointMake(r, r));
}

bool iButton::key(iKeyState stat, iPoint point)
{
	switch (stat) {

	case iKeyStateBegan:
		if (containPoint(point, img->touchRect()))
		{
			pushed = true;
			img->setTexObject(1);
			//aniDt = 0.0f;
		}
		break;

	case iKeyStateMoved:
#if 0
		pushed = containPoint(point, img->touchRect());
#else
		if (pushed)
		{
			if (containPoint(point, img->touchRect()) == false)
			{
				pushed = false;
				img->setTexObject(0);
			}
		}
		//else
		//{
		//	if (containPoint(point, img->touchRect()))
		//		pushed = true;
		//}
#endif
		break;

	case iKeyStateEnded:
		if (containPoint(point, img->touchRect()))
		{
			pushed = false;
			img->setTexObject(0);

			// notify event
			if (methodClick)
				methodClick(this, parmClick);
		}
		break;
	}

	return false;
}
