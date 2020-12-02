#include <windows.h>
#include <math.h>
#include <gl\gl.h>
#include "Class.h"


float distance(float x1,float y1,float x2,float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

Player::Player()
{
	HP=100;
	MP=100;
	Speed=WALKSPEED_OF_PLAYER;
	RunSpeed=RUNSPEED_OF_PLAYER;
	TypeOfAttack=0;
	Chances=CHANCES_OF_PLAYER;
	Strength=15;
	Kind=0;
	X=-5;
	Y=0;
	Z=0;
}

void Player::run()
{
};
void Player::walk()
{
};
void Player::attack()
{
};
void Player::die()
{
};
void Player::jump()
{
};




Enemy::Enemy()
{
	HP=50;
	MP=0;
	Speed=SPEED_OF_ENEMY;
	TypeOfAttack=0;
	Chances=1;
	Strength=5;
	Kind=0;
	X=10;
	Y=0;
	Z=0;
	DetectRange=ENEMY_DETECT_RANGE;
	Time=5;
	RewardScore=20;
}

void Enemy::walk()
{
};
void Enemy::attack()
{
};

void Enemy::jump()
{
};

void Enemy::die()
{
};


