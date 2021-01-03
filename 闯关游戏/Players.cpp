#include <windows.h>
#include <math.h>
#include "Class.h"


float distance(float x1,float y1,float x2,float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


Player::Player(float x,float y,float z)
{
	HP=100;
	MP=100;
	Speed=WALKSPEED_OF_PLAYER;
	RunSpeed=RUNSPEED_OF_PLAYER;
	TypeOfAttack=0;
	Chances=CHANCES_OF_PLAYER;
	Strength=15;
	Kind=0;
	X=x;
	Y=y;
	Z=z;
	Face=1;
	Act.InitialAct(x,y,z);
	attackstate=0;
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
	Face=1;
	attackstate=0;
}
Player::~Player()
{

}
void Player::Initial(float x,float y,float z)
{
	X=x;
	Y=y;
	Z=z;
	Act.InitialAct(x,y,z);
}

void Player::act()
{
	Act.UpdatePos(X,Y,Z);
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
			if(Face==-1)
			{
				Act.settexture(move[0]);
			}
			if(Face==1)
			{
				Act.settexture(move[1]);
			}
			Z -= ac;
		}
		if(movedirect==1)
		{
			if(Face==-1)
			{
				Act.settexture(move[0]);
			}
			if(Face==1)
			{
				Act.settexture(move[1]);
			}
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
	if(Act.getstate()==4)	
	{
		GLfloat ac=0.1f,ad=0.2f;
		if(Face==-1)
		{
			if(attackstate==0)
			{
				Act.settexture(jump[0]);
			}
			if(attackstate==1)
			{
				Act.settexture(jump[2]);
			}
		}
		if(Face==1)
		{
			if(attackstate==0)
			{
				Act.settexture(jump[1]);
			}
			if(attackstate==1)
			{
				Act.settexture(jump[3]);
			}
		}
		if(movedirect ==2) X-=ac;
		if(movedirect ==3) X+=ac;
		if(Act.getfps()<0.5f) Y +=ad;
		if(Act.getfps()>=0.5f &&Act.getfps()<1.0f) Y -=ad;
		Act.jump(movedirect,attackstate);
		if(Act.getfps()>=0.1f) movedirect=-1;
	}

}


void Bullet::Initial(float x,float y,float z)
{
	X=x;
	Z=z;
	Y=y;

}

void Bullet::draw()
{
	if(direct==-1)
	{
		glBindTexture(GL_TEXTURE_2D, bullet[0]);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+t_height, Z);					// Top Left
			glTexCoord2f(1.0f,1.0f);glVertex3f( X+t_weight, Y+t_height, Z);					// Top Right
			glTexCoord2f(1.0f,0.0f);glVertex3f( X+t_weight, Y, Z);					// Bottom Right
			glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
	}
	if(direct==1)
	{
		glBindTexture(GL_TEXTURE_2D, bullet[1]);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+t_height, Z);					// Top Left
			glTexCoord2f(1.0f,1.0f);glVertex3f( X+t_weight, Y+t_height, Z);					// Top Right
			glTexCoord2f(1.0f,0.0f);glVertex3f( X+t_weight, Y, Z);					// Bottom Right
			glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
	}

}
