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
//����δ����Ӣ��ʱ����ȡѲ��ģʽ
void Enemy::Patrol(){

	GLfloat ac=0.1f;
	srand((int)time(0)); 
	int num = random(0,2) -1 ;//����������������ж��������߻���������
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


