#include "Enemy.h"

#include "Proc.h"
#include "Player.h"
#include "Effect.h"
#include "Item.h"
#include "Ui.h"
#include "Sound.h"

Enemy** enemy;
Enemy** _enemy;

int enemyNum;
float hardDt;
float appearEnemyDt, _appearEnemyDt;
float playerAppearDt;

void loadEnemy()
{
	int i, j;
	_enemy = new Enemy * [enemyTypeMax];
	for (i = 0; i < enemyTypeMax; i++)
	{
		_enemy[i] = new Enemy[enemyMax];
		for (j = 0; j < enemyMax; j++)
		{
			Enemy* e = &_enemy[i][j];
			e->alive = false;
			e->index = i;
			e->damage = 0;
			e->speed = 0;

			e->position = iPointZero;
			e->vector = iPointZero;

			e->texEnemy = createBall2(enemyRadius);

			e->sinDt = 0;
		}
	}

	enemy = new Enemy * [_enemyNum];
	enemyNum = 0;

	_appearEnemyDt = 1.0f;
	appearEnemyDt = 0.0f;

	hardDt = 0.0f;
	playerAppearDt = 0.0f;
}

void freeEnemy()
{
	for (int i = 0; i < enemyTypeMax; i++)
	{
		for (int j = 0; j < enemyMax; j++)
			freeImage(_enemy[i][j].texEnemy);
		delete _enemy[i];
	}
	delete _enemy;
	delete enemy;
}

void drawEnemy(float dt, iPoint off)
{
	playerAppearDt += dt;
	if (playerAppearDt < 6.5f)
	{
		dt = 0.0f;
		playTime = 0.0f;
		sizeDt = 0.1f;
		return;
	}

	for (int i = 0; i < enemyNum; i++)
	{
		Enemy* e = enemy[i];
		if (!e->paint(dt, off))
		{
			enemyNum--;
			for (int j = i; j < enemyNum; j++)
				enemy[j] = enemy[1 + j];
			i--;
		}
	}

	if (enemyNum > _enemyNum)
		return;

	if (_appearEnemyDt > 0.2f)
	{
		hardDt += dt;
		if (hardDt > _hardDt)
		{
			hardDt = 0.0f;
			_appearEnemyDt -= 0.05f;
		}
	}
	else if (_appearEnemyDt < 0.2f)
		_appearEnemyDt = 0.2f;

	appearEnemyDt += dt;
	if (appearEnemyDt > _appearEnemyDt)
	{
		appearEnemyDt = 0;
		int type = randomType(playTime);
		iPoint p = responPosition();
		addEnemy(type, p);
	}
}

void addEnemy(int type, iPoint p)
{
	int i, j;
	Enemy* a = _enemy[type];
	for (i = 0; i < enemyMax; i++)
	{
		Enemy* e = &a[i];
		if (e->alive == false)
		{
			e->alive = true;
			e->index = type;
			e->speed = randomSpeed(playTime);
			e->damage = e->index == nomal ? 1 : 0;

			e->position = p;
			e->vector = randomVector(playTime, e->position);

			e->sinDt = 0;

			enemy[enemyNum] = e;
			enemyNum++;
			return;
		}
	}
}

void dieEnemy()
{
	for (int i = 0; i < enemyNum; i++)
		if (enemy[i]->alive)
			enemy[i]->alive = false;
}

bool Enemy::paint(float dt, iPoint off)
{
	off += position;
	switch (index)
	{
		case nomal:		setRGBA(RGB_RED, 1.0f);			break;
		case slow:		setRGBA(RGB_SLOW, 1.0f);		break;
		case blind:		setRGBA(RGBA_WHITE);			break;
		case poison:	setRGBA(RGB_POISON, 1.0f);		break;
		case curse:		setRGBA(RGB_CURSE, 1.0f);		break;
		case exhauste:	setRGBA(RGB_EXHAUSTE, 1.0f);	break;
	}
	drawImage(texEnemy, position.x + enemyRadius, position.y + enemyRadius, VCENTER | HCENTER);
	setRGBA(RGBA_WHITE);

	sinDt += dt * 300;

	float s;
	if (speed == 200)
		s = ((_sin(sinDt) * 200) + speed);
	else
		s = speed;

	if (vector.x == +1 || vector.x == -1)
	{
		position.x += vector.x * (s * dt);
		position.y += vector.y * (_sin(sinDt) * s * dt);
	}
	else if (vector.y == +1 || vector.y == -1)
	{
		position.x += vector.x * (_sin(sinDt) * s * dt);
		position.y += vector.y * (s * dt);
	}
	else
		position += vector * (s * dt);

	if (-20 > position.x ||
		position.x > devSize.width + 20 ||
		-20 > position.y ||
		position.y > devSize.height + 20)
		alive = false;

	Player* p = player;
	float distance = iPointDistance(p->position, position);
	if (distance < enemyRadius + playerHitRadius)
	{
		p->hit = true;
		alive = false;
		switch (index)
		{
			case nomal:
				p->hp -= damage;
				break;
			case slow:
				p->slow = true;
				p->slowDt = 0;
				break;
			case blind:
				p->blind = true;
				p->blindDt = 0.0f;
				break;
			case poison:
				if(!p->poison)
					addPotion(Antidote);
				p->poison = true;
				break;
			case curse:
				p->curse++;
				break;
			case exhauste:
				p->exhauste++;
				if (p->exhauste < 5)
					addPotion(energyDrink);
				break;
		}
		playSfxSound(1);
	}

	return alive;
}

iPoint responPosition()
{
	iPoint responP[4];
	responP[0] = iPointMake(-20, rand() % (int)devSize.height);
	responP[1] = iPointMake(devSize.width + 20, rand() % (int)devSize.height);
	responP[2] = iPointMake(rand() % (int)devSize.width, -20);
	responP[3] = iPointMake(rand() % (int)devSize.width, devSize.height + 20);
	return responP[rand() % 4];
}

iPoint randomVector(float pt, iPoint position)
{
	iPoint v;
	int weight = rand() % 100, t = pt / 5;

	switch (t)
	{
		case 0:
		case 1:
			v = iPointVector(player->position - position);
			break;
		case 2:
			if (weight < 75)
			{
				if (position.x == devSize.width + 20)
					v = iPointMake(-1, 0.9);
				else if (position.x == -20)
					v = iPointMake(+1, 0.9);

				if (position.y == devSize.height + 20)
					v = iPointMake(0.9, -1);
				else if (position.y == -20)
					v = iPointMake(0.9, +1);
			}
			else
				v = iPointVector(player->position - position);
		default:
			if (weight < 50)
			{
				if (position.x == devSize.width + 20)
					v = iPointMake(-1, 0.9);
				else if (position.x == -20)
					v = iPointMake(+1, 0.9);

				if (position.y == devSize.height + 20)
					v = iPointMake(0.9, -1);
				else if (position.y == -20)
					v = iPointMake(0.9, +1);
			}
			else
				v = iPointVector(player->position - position);
			break;
	}
	return v;
}

float randomSpeed(float pt)
{
	float s;
	int  weight = rand() % 100, t = pt / 5;

	switch (t)
	{
		case 0:
		case 1:
			s = 300;
			break;

		case 2:
			if (weight < 90)
				s = 300;
			else if (weight < 100)
				s = 200;
			break;

		case 3:
			if (weight < 80)
				s = 300;
			else if (weight < 100)
				s = 200;
			break;

		case 4:
			if (weight < 65)
				s = 300;
			else if (weight < 100)
				s = 200;
			break;

		default:
			if (weight < 50)
				s = 300;
			else if (weight < 100)
				s = 200;
			break;
	}
	return s * 2;
}

int randomType(float pt)
{
	int type, weight = rand() % 100, t = pt / 5;
	switch (t)
	{
		case 0:
			type = nomal;
			break;

		case 1:
			if (weight < 50)
				type = nomal;
			else if (weight < 100)
				type = slow;
			break;

		case 2:
			if (weight < 50)
				type = nomal;
			else if (weight < 75)
				type = slow;
			else if (weight < 100)
				type = blind;
			break;

		case 3:
			if (weight < 50)
				type = nomal;
			else if (weight < 75)
				type = slow;
			else if (weight < 90)
				type = blind;
			else if (weight < 100)
				type = poison;
			break;

		case 4:
			if (weight < 50)
				type = nomal;
			else if (weight < 70)
				type = slow;
			else if (weight < 95)
				type = blind;
			else if (weight < 95)
				type = poison;
			else if (weight < 100)
				type = curse;
			break;

		case 5:
			if (weight < 50)
				type = nomal;
			else if (weight < 65)
				type = slow;
			else if (weight < 70)
				type = blind;
			else if (weight < 85)
				type = poison;
			else if (weight < 95)
				type = curse;
			else if (weight < 100)
				type = exhauste;
			break;

		default:
			if (weight < 50)
				type = nomal;
			else if (weight < 65)
				type = slow;
			else if (weight < 70)
				type = blind;
			else if (weight < 85)
				type = poison;
			else if (weight < 95)
				type = curse;
			else if (weight < 100)
				type = exhauste;
			break;
	}

	return type;
}
