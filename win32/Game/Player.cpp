#include "Player.h"

#include "Ui.h"
#include "Item.h"
#include "Effect.h"
#include "Sound.h"

Player* player;
float procPlayerAppearDt;
#define _procPlayerAppearDt 2.0f

float dieExplosionDt;
int dieExplosionNum;

void loadPlayer()
{
	player = new Player;
	Player* p = player;
	p->alive = true;
	p->move = false;
	p->hp = playerHp;
	p->speed = playerSpeed;
	p->stamina = maxStamina;
	p->degreeSpeed = _degreeSpeed;
	p->delaySt = 0.0f;
	p->degree = 0.0f;
	p->position = iPointMake(devSize.width / 2, devSize.height + 50);// iPointCenter;
	p->texPlayer = createTriangle(playerWidth, playerHeight);
	p->hitBox = createHitBox(playerHitRadius);

	p->hit = false;

	p->slowRate = 0;
	p->slowDt = 0;
	p->slow = false;

	p->blindDt = 0;
	p->blind = false;

	p->poison = false;
	p->poisonDt = 0;

	p->curse = 0;

	p->exhauste = 0;
	p->exhausteRate = 0;

	procPlayerAppearDt = 0.0f;

	dieExplosionDt = 0.0f;
	dieExplosionNum = 0;
}

void freePlayer()
{
	freeImage(player->hitBox);
	freeImage(player->texPlayer);
	delete player;
}

void drawPlayer(float dt, iPoint off)
{
	Player* p = player;

	//중독
	if (p->poison)
	{
		p->poisonDt += dt;
		if (p->poisonDt > 3.0f)
		{
			p->hp--;
			p->poisonDt = 0.0f;
		}
	}

	//죽음, 저주
	if (p->curse == 3)
		p->hp = 0;
	

	if (p->paint(dt, off))
	{
		//나올 때 연출
		if (procPlayerAppearDt < 6.5f)
		{
			procPlayerAppearDt += dt;
			if (procPlayerAppearDt < _procPlayerAppearDt)
			{
				p->position = easeIn(procPlayerAppearDt / _procPlayerAppearDt,
					iPointMake(devSize.width / 2, devSize.height + 50), iPointCenter);
			}
			else
			{
				p->position = iPointCenter;
				showPopHpBar(true);
				showPopStamina(true);
			}
			return;
		}

		iPoint v = iPointZero;

		if (keyState & keysLeftArrow ||
			keyState & keysRightArrow ||
			keyState & keysUpArrow ||
			keyState & keysDownArrow)
			p->move = true;

		//slow
		if (p->slow)
		{
			p->slowDt += dt;
			p->slowRate = 100;
			if (p->slowDt > 3)
			{
				p->slowDt = 0;
				p->slow = false;
			}
		}
		else
			p->slowRate = 0;

		//좌우
		if (keyState & keysLeftArrow)
		{
			v.x = -1;
			p->degree += (p->degreeSpeed - p->slowRate) * dt;
			if (p->degree > yDegree)
				p->degree = yDegree;

		}
		else if (keyState & keysRightArrow)
		{
			v.x = 1;
			p->degree -= (p->degreeSpeed - p->slowRate) * dt;
			if (p->degree < -yDegree)
				p->degree = -yDegree;
		}
		else if (p->degree != 0)
		{
			if (p->degree < 0)
			{
				p->degree += (p->degreeSpeed - p->slowRate) * dt;
				if (p->degree > 0)
					p->degree = 0;
			}
			else if (p->degree > 0)
			{
				p->degree -= (p->degreeSpeed - p->slowRate) * dt;
				if (p->degree < 0)
					p->degree = 0;
			}
		}
		//상하
		if (keyState & keysUpArrow)
			v.y = -1;
		else if (keyState & keysDownArrow)
			v.y = 1;

		//exhauste
		if (p->exhauste > 4)
			p->exhauste = 4;
		p->exhausteRate = 50 * p->exhauste;

		//걷기 달리기
		if ((keyState & keysShift) && (v != iPointZero))
		{
			p->delaySt = 0.0f;
			if (p->stamina > 0)
			{
				p->stamina -= useStamina * dt;
				p->speed = playerRnu - p->slowRate;
				p->degreeSpeed = _degreeSpeed + 60.0f;
			}
			else //if (stamina < 0)
			{
				p->stamina = 0;
				p->speed = playerSpeed - p->slowRate;
				p->degreeSpeed = _degreeSpeed;
			}
		}
		else
		{
			p->speed = playerSpeed - p->slowRate;
			p->degreeSpeed = _degreeSpeed;
			if (p->stamina > maxStamina - p->exhausteRate)
				p->stamina = maxStamina - p->exhausteRate;
			else
			{
				p->delaySt += dt;
				if (p->delaySt > _delaySt)
					p->stamina += useStamina * dt;
			}
		}

		//이동범위
		if (v != iPointZero)
		{
			float minX = off.x + playerHitRadius;
			float maxX = (devSize.width - 1) - playerHitRadius;
			float minY = off.y + playerHeight / 2;
			float maxY = (devSize.height - 1) - playerHeight / 2;

			v = iPointVector(v);

			if (p->hp < 1)
				dt = 0.0f;

			v *= (p->speed * dt);
			p->position += v;

			if (p->position.x < minX)
				p->position.x = minX;
			else if (p->position.x > maxX)
				p->position.x = maxX;

			if (p->position.y < minY)
				p->position.y = minY;
			else if (p->position.y > maxY)
				p->position.y = maxY;
		}
	}
}

bool Player::paint(float dt, iPoint off)
{
	off += position;
	if (slow)
		setRGBA(RGB_SLOW, 1.0f);
	else
		setRGBA(RGB_GRAY, 1.0f);

	float scaleW;
	if (degree > 0)
		scaleW = (100 - degree) * 0.01f;
	else
		scaleW = (100 + degree) * 0.01f;

	if (hp < 1)
	{
		scaleW = 1.0f;
		degree = 0.0f;
	}

	drawImage(texPlayer, off.x, off.y, VCENTER | HCENTER, 0, 0, texPlayer->width, texPlayer->height,
		scaleW, 1, 2, -degree / 6);
	setRGBA(RGBA_WHITE);

	drawImage(hitBox, off.x, off.y + 5, VCENTER | HCENTER);

	if (blind)
	{
		blindDt += dt;
		drawBlind(dt, off);
		if (blindDt > _blindDt)
		{
			blindDt = 0.0f;
			blind = false;
		}
	}

	if (hp < 1)
	{
		dieExplosionDt += dt;
		if (dieExplosionDt > 0.3f)
		{
			hit = true;
			dieExplosionDt = 0.0f;
			dieExplosionNum++;
			playSound(2);
		}

		if (dieExplosionNum > 9)
			alive = false;
	}

	drawExplosion(dt, off);

	return alive;
}

Texture* createHitBox(float radius)
{
	float r = radius * 2;
	iGraphics* g = iGraphics::share();
	iSize size = iSizeMake(r, r);
	g->init(size);
	setRGBA(RGB_RED, 0.5f);
	g->fillRect(0, 0, size.width, size.height, radius);
	setRGBA(RGBA_WHITE);
	return g->getTexture();
}
