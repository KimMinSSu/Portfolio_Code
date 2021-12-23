#include "Item.h"

#include "Player.h"

Potion** potion;
Potion** _potion;

int potionNum;
#define potionMax 10
#define _potionNum potionIndexMax * potionMax

void loadItem()
{
	_potion = new Potion * [potionIndexMax];
	for (int i = 0; i < potionIndexMax; i++)
	{
		_potion[i] = new Potion[potionMax];
		for (int j = 0; j < potionMax; j++)
		{
			Potion* po = &_potion[i][j];
			po->alive = false;
			po->position = iPointZero;
			po->tex = createImage("Assets/Potion/Potion%d.png", i);
			po->index = i;
		}
	}

	potion = new Potion * [_potionNum];
	potionNum = 0;
}

void freeItem()
{
	delete _potion;
	delete potion;
}

void drawItem(float dt)
{
	containItem();

	for (int i = 0; i < potionNum; i++)
	{
		Potion* po = potion[i];
		if (!po->paint(dt))
		{
			potionNum--;
			for (int j = i; j < potionNum; j++)
				potion[j] = potion[j + 1];
			i--;
		}
	}
}

void addPotion(int index)
{
	Potion* po = _potion[index];
	for (int i = 0; i < potionMax; i++)
	{
		Potion* p = &po[i];
		if (p->alive == false)
		{
			p->alive = true;
			p->position = iPointMake(rand() % 500 + 100, rand() % 300 + 100);

			potion[potionNum] = p;
			potionNum++;
			return;
		}
	}
}

void containItem()
{
	Player* p = player;
	iRect rt = iRectMake(p->position.x - playerWidth / 2,
		p->position.y - playerHeight / 2,
		playerWidth,
		playerHeight);

	for (int i = 0; i < potionNum; i++)
	{
		if (potion[i]->contain(rt))
		{
			potion[i]->alive = false;
			if (potion[i]->index == Antidote)
			{
				p->poison = false;
				p->poisonDt = 0.0f;
			}
			else if (potion[i]->index == energyDrink)
			{
				p->exhauste--;
			}
		}
	}
}

//=============================================
Item::Item()
{
	alive = false;
	position = iPointZero;
	tex = NULL;
}

Item::~Item()
{
	
}

//=============================================
Potion::Potion()
{
	index = Antidote;
}

Potion::~Potion()
{
	freeImage(tex);
}

bool Potion::paint(float dt)
{
	drawImage(tex, position.x, position.y, BOTTOM | HCENTER, 0, 0,
		tex->width, tex->height, 0.5f, 0.5f, 0, 0);

	return alive;
}

bool Potion::contain(iRect rect)
{
	iRect rt = iRectMake(position.x - 16, position.y - 32, 32, 32);


	return containRect(rect, rt);
}
