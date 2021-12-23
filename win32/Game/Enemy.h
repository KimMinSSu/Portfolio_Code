#pragma once

#include "iStd.h"

#define enemyMax 100
#define _enemyNum enemyMax * enemyTypeMax
#define enemyRadius 10
#define _hardDt 2.0f

extern int enemyNum;
extern float hardDt;
extern float appearEnemyDt, _appearEnemyDt;

void loadEnemy();
void freeEnemy();
void drawEnemy(float dt, iPoint off);

void addEnemy(int type, iPoint p);
void dieEnemy();

iPoint responPosition();
iPoint randomVector(float pt, iPoint position);
float randomSpeed(float pt);
int randomType(float pt);

enum EnemyType
{
	nomal = 0,	//50
	slow,		//15
	blind,		//15
	poison,		//10
	curse,		//5
	exhauste,	//5

	enemyTypeMax
};

struct Enemy
{
	bool alive;
	int index;
	int damage;
	float speed;

	iPoint position;
	iPoint vector;

	//tex
	Texture* texEnemy;

	bool paint(float dt, iPoint off);

	float sinDt;
};
