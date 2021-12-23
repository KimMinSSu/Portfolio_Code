#pragma once

#include "iStd.h"

struct Player;
extern Player* player;

void loadPlayer();
void freePlayer();
void drawPlayer(float dt, iPoint off);

#define playerWidth 30 * 2
#define playerHeight 30 * 2
#define playerHitRadius 5 * 2

#define playerHp 5
#define playerSpeed 200.0f * 2
#define playerRnu 400.0f * 2

#define maxStamina 200.0f
#define useStamina 100.0f
#define _delaySt 0.5f

#define yDegree 60.0f
#define zDegree yDegree / 6.0f
#define _degreeSpeed 180.0f * 2

#define _blindDt 3.0f
struct Player
{
	bool alive;
	bool move;
	bool walk;
	bool run;

	int hp;
	float speed;
	float degreeSpeed;
	float stamina;
	float delaySt;
	float degree;

	iPoint position;
	Texture* texPlayer;
	Texture* hitBox;

	iPoint vec;

	//enemy
	bool hit;

	float slowRate;
	float slowDt;
	bool slow;

	float blindDt;
	bool blind;

	bool poison;
	float poisonDt;

	int curse;

	int exhauste;
	float exhausteRate;
	bool paint(float dt, iPoint off);
};
Texture* createHitBox(float radius);
