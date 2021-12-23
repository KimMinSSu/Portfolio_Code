#pragma once

struct iSize
{
	float width, height;

	iSize operator +(const iSize& p);
	iSize operator -(const iSize& p);
	iSize operator *(const iSize& p);
	iSize operator /(const iSize& p);

	iSize& operator +=(const iSize& p);
	iSize& operator -=(const iSize& p);
	iSize& operator *=(const iSize& p);
	iSize& operator /=(const iSize& p);

	iSize operator +(const float s);
	iSize operator -(const float s);
	iSize operator *(const float s);
	iSize operator /(const float s);

	iSize& operator +=(const float s);
	iSize& operator -=(const float s);
	iSize& operator *=(const float s);
	iSize& operator /=(const float s);
};

#define iSizeZero iSizeMake(0, 0)
#define iSizeOne iSizeMake(1, 1)
#define iSizeCenter iSizeMake(devSize.width / 2, devSize.height / 2)
#define iSizeDev iSizeMake(devSize.width, devSize.height)

iSize iSizeMake(float width, float height);












