#include "iRect.h"

iRect iRectMake(float x, float y, float width, float height)
{
	iRect r;
	r.origin.x = x;
	r.origin.y = y;
	r.size.width = width;
	r.size.height = height;
	return r;
}

iRect iRectMake(iPoint position, iSize size)
{
	iRect r;
	r.origin.x = position.x;
	r.origin.y = position.y;
	r.size.width = size.width;
	r.size.height = size.height;
	return r;
}

bool containPoint(iPoint point, iRect rect)
{
#if 0
	return (
		point.x > rect.origin.x &&
		point.x < rect.origin.x + rect.size.width &&
		point.y > rect.origin.y &&
		point.y < rect.origin.y + rect.size.height);
#else
	return !(
		point.x < rect.origin.x ||
		point.x > rect.origin.x + rect.size.width ||
		point.y < rect.origin.y ||
		point.y > rect.origin.y + rect.size.height);
#endif
}

bool containRect(iRect rt0, iRect rt1)
{
	return !(
		rt0.origin.x + rt0.size.width < rt1.origin.x ||
		rt0.origin.x > rt1.origin.x + rt1.size.width ||
		rt0.origin.y + rt0.size.height < rt1.origin.y ||
		rt0.origin.y > rt1.origin.y + rt1.size.height);
}
