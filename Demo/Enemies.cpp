
//#include <windows.h>
#include <math.h>
#include "Enemies.h"


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
}
void Enemy::attack()
{
}

void Enemy::jump()
{
}

void Enemy::die()
{
}

