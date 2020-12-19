#include <windows.h>
#include <math.h>
#include "Class.h"
Enemy::Enemy(float x,float y,float z)
{
	HP=50;
	MP=0;
	Speed=SPEED_OF_ENEMY;
	TypeOfAttack=0;
	Chances=1;
	Strength=5;
	Kind=0;
	X=x;
	Y=y;
	Z=z;
	DetectRange=ENEMY_DETECT_RANGE;
	Time=5;
	RewardScore=20;
	Act.InitialAct(x,y,z);
	attackstate=0;
	Face=-1;
}
Enemy::~Enemy()
{

}

void Enemy::act()
{
	if(Act.getstate()==0)	
	{
		if(Face==1)
		{
			Act.settexture(still[1]);
		}
		if(Face==-1)
		{
			Act.settexture(still[0]);
		}
		Act.still();
	}
	if(Act.getstate()==1)	
	{
		if(Face==1)
		{
			Act.settexture(attack[1]);

		}
		if(Face==-1)
		{
			Act.settexture(attack[0]);
		}
		Act.attack(attackstate);
		if(Act.getfps()>=1) attackstate=0;
	}
	if(Act.getstate()==2)	
	{
		GLfloat ac=0.05f;
		if(movedirect==0)
		{
			Act.settexture(move[0]);
			Z -= ac;
		}
		if(movedirect==1)
		{
			Act.settexture(move[1]);
			Z += ac;
		}
		if(movedirect==2)
		{
			Act.settexture(move[2]);
			X -= ac;
		}
		if(movedirect==3)
		{
			Act.settexture(move[3]);
			X += ac;
		}

		Act.move(movedirect);
		if(Act.getfps()>=1) movedirect=-1;
	}
	if(Act.getstate()==3)	Act.die();
	
}