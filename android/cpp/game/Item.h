#pragma once

#include "iStd.h"

void loadItem();
void freeItem();
void drawItem(float dt);

void addPotion(int index);
void containItem();

enum PotionIndex
{
	Antidote = 0,
	energyDrink,

	potionIndexMax
};

struct Item
{
	bool alive;
	iPoint position;
	Texture* tex;

	Item();
	virtual ~Item();

	virtual bool paint(float dt) = 0;
};

struct Potion : Item
{
	int index;

	Potion();
	virtual ~Potion();

	virtual bool paint(float dt);
	bool contain(iRect rect);
};
