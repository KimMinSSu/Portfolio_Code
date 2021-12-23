#include "Effect.h"

#include "Player.h"

void loadEffect()
{
	createBlind();
	createExplosion();
}

void freeEffect()
{
	freeBlind();
	freeExplosion();
}

//+++++++++++++++++++++++++++++
//+          Blind            +
//+++++++++++++++++++++++++++++
Texture* texBlind;

void createBlind()
{
	int ox = 1024;
	int oy = 1024;

	uint8* rgba = new uint8[2048 * 2048 * 4];
	for (int j = 0; j < 2048; j++)
	{
		for (int i = 0; i < 2048; i++)
		{
			uint8* c = &rgba[2048 * 4 * j + 4 * i];
			memset(c, 0xFF, sizeof(uint8) * 3);

			float x = i + 0.5f;
			float y = j + 0.5f;

			float d = sqrtf((ox - x) * (ox - x) + (oy - y) * (oy - y));
			if (d > 150 * SIZE_RATE)
				c[3] = 0xFF;
			else
				c[3] = (d / (150 * SIZE_RATE)) * 0xFF;
		}
	}
	Texture* tex = createImageWithRGBA(rgba, 2048, 2048);
	delete rgba;

	texBlind = tex;
}

void freeBlind()
{
	freeImage(texBlind);
}

void drawBlind(float dt, iPoint off)
{
	setRGBA(0.2f, 0.2f, 0.2f, 1.0f);
	drawImage(texBlind, off.x, off.y, VCENTER | HCENTER);
	setRGBA(RGBA_WHITE);
}

//+++++++++++++++++++++++++++++
//+        Explosion          +
//+++++++++++++++++++++++++++++
Texture* texExplosion;
float explosionDt;

void createExplosion()
{
	int r = 16;

	uint8* rgba = new uint8[32 * 32 * 4];
	for (int j = 0; j < 32; j++)
	{
		for (int i = 0; i < 32; i++)
		{
			uint8* c = &rgba[32 * 4 * j + 4 * i];
			memset(c, 0xFF, sizeof(uint8) * 3);

			float x = i + 0.5f;
			float y = j + 0.5f;
			float d = sqrtf((x - r) * (x - r) + (y - r) * (y - r));
			if (d < 16)
			{
				c[0] = 0xFF;
				c[1] = (1.0f - d / r) * 0xFF;
				c[2] = 0x00;
				c[3] = 0xFF;
			}
			else
				c[3] = 0;
		}
	}

	Texture* tex = createImageWithRGBA(rgba, 32, 32);
	delete rgba;
	texExplosion = tex;
	explosionDt = 0.0f;
}

void freeExplosion()
{
	freeImage(texExplosion);
}

void drawExplosion(float dt, iPoint off)
{
	float rx, ry, a;
	if (player->hit)
	{
		explosionDt += dt;
		if (explosionDt < 0.2f)
		{
			a = 1.0f - (explosionDt / 0.2f);
			rx = ry = linear(explosionDt / 0.2f, 0.5, 2.0f);
		}
		else
		{
			player->hit = false;
			a = 0.0f;
			rx = ry = 2.0f;
		}
		setRGBA(1, 1, 1, a);
		drawImage(texExplosion, off.x, off.y, VCENTER | HCENTER, 0, 0,
				  texExplosion->width, texExplosion->height,
				  rx * SIZE_RATE, ry * SIZE_RATE, 0, 0);
		setRGBA(RGBA_WHITE);
	}
	else
	{
		rx = ry = a = 1.0f;
		explosionDt = 0.0f;
	}
}
