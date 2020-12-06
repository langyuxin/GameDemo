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
	flagm=0;
	flaga=0;
	flagj=0;
	flagd=0;
	Face=1;
}

Player::~Player()
{

}


void Player::run()
{
}
void Player::walk(GLfloat& roll,GLuint a,int b)			
{
	if(roll<=1 &&flaga==0)
	{
		glBindTexture(GL_TEXTURE_2D, a);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(roll,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(roll+0.25f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(roll+0.25f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
			glTexCoord2f(roll,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		GLfloat ac=0.05f;
		if(b==3) X -= ac;
		if(b==4) X += ac;
		if(b==5) Z -= ac;
		if(b==6) Z += ac;
		roll += 0.25f;
		Sleep(35);
	}
	else if(flagm ) flagm = 0;
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
		Sleep(35);
	}
	else flaga =0;
	
}
void Player::die(GLfloat& roll,GLuint a)
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
		Sleep(250);
	}
	else flagd=0;

}
void Player::jump()
{
}
void Player::draw(GLuint a)
{
	if(flaga==0 && flagm ==0 && flagj==0)
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


