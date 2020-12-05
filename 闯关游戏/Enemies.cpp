//#include <windows.h>
#include <math.h>
#include "Class.h"
#include <cstdlib>
#include <ctime>
#define random(a,b) (rand()%(b-a)+a)

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
//敌人未发现英雄时，采取巡逻模式
void Enemy::Patrol(){

	GLfloat ac=0.1f;
	srand((int)time(0)); 
	int num = random(0,2) -1 ;//随机生成正负数来判断是向左走还是向右走
	if(num){
		X += ac;
	}else X -= ac;

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


