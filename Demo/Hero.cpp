
#include <math.h>
#include "Hero.h"
#include <windows.h>

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
	flag=1;
}

Player::~Player()
{

}


void Player::run()
{
}
void Player::walk(GLfloat& roll,GLuint a,int b)			
{
	if(roll<=1)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.25f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.25f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		GLfloat ac=0.1f;
		if(b==3) X -= ac;
		if(b==4) X += ac;
		if(b==5) Z -= ac;
		if(b==6) {Z += ac;}
		roll += 0.25f;
		Sleep(50);
	}
	else flag = 1;
}
void Player::attack(GLfloat& roll,GLuint a)
{
	if(roll<=1)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.25f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.25f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		roll += 0.25f;
		Sleep(50);
	}
	
}
void Player::die()
{
}
void Player::jump()
{
}
void Player::draw(GLuint a)
{
	if(flag==1)
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