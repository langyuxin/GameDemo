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
	flagm=0;
	flagd=0;
	flaga=0;
}
Enemy::~Enemy()
{

}

void Enemy::walk(GLfloat& roll,GLuint a,int b)			
{
	if(roll<=1 &&flaga==0 && flagd==0)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.25f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.25f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		GLfloat ac=0.1f;
		if(b==-1) X -= ac;
		if(b==1) X += ac;
		if(b==5) Z -= ac;
		if(b==6) Z += ac;
		roll += 0.25f;
		Sleep(35);
	}
	else flagm = 0;
}
void Enemy::attack(GLfloat& roll,GLuint a)
{
	if(roll<=1 && flagd==0)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.25f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.25f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		roll += 0.25f;
		Sleep(35);
	}
	else flaga=0;
	
}

void Enemy::jump()
{
}

void Enemy::die(GLfloat& roll,GLuint a)
{
	if(roll<=1 && flagd==1)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.33f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.33f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		roll += 0.33f;
		Sleep(50);
	}

}
void Enemy::draw(GLuint a)
{
	if(flaga==0 && flagm ==0 && flagd==0)								//判断有没有在进行其他动作：攻击、移动等；
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(1.0f,1.0f);glVertex3f(X+2, Y+2, Z);					// Top Right
			glTexCoord2f(1.0f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
			glTexCoord2f(0.0,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
	}
}

