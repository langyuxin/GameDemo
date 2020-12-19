#include <windows.h>
#include <math.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "State.h"

Action::Action()
{
//	InitialAct(x,y,z);
}

void Action::InitialAct(int x,int y,int z)		//动作类初始化
{
	fps=0;
	state=0;
	X=x;
	Y=y;
	Z=z;
}

void	Action::attack(int& c)					//人物攻击
{
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(fps+0.1f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(fps+0.1f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		fps += 0.1f;
		Sleep(30);
	}
	if(fps>=1) {state=0;fps=0;c=0;}
}
void	Action::move(int b)						//人物移动
{
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(fps+0.1f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(fps+0.1f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		GLfloat ac=0.05f;
		if(b==2) X -= ac;
		if(b==3) X += ac;
		if(b==1) Z += ac;
		if(b==0) Z -= ac;
		fps += 0.1f;
		Sleep(30);
	}
	if(fps>=1) {state=0;fps=0;}


}
void	Action::die()							//人物被击败
{
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
			glTexCoord2f(fps+0.1f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
			glTexCoord2f(fps+0.1f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		fps += 0.1f;
		Sleep(30);
	}
	if(fps>=1) {state=0;fps=0;}

}
void	Action::jump(int b,int& c)				//人物跳跃
{
	if(fps<1)
	{
		GLfloat ac=0.05f,ad=0.5f;
		if(fps<0.5f) Y +=ad;
		if(fps>=0.5f || fps==0.75f) Y -=ad;
		glBindTexture(GL_TEXTURE_2D, texture);
		if(c==0)
		{
			glBegin(GL_QUADS);									// Draw A Quad
				glTexCoord2f(fps,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
				glTexCoord2f(fps+0.1f,1.0f);glVertex3f( X+2, Y+2, Z);					// Top Right
				glTexCoord2f(fps+0.1f,0.0f);glVertex3f( X+2, Y, Z);					// Bottom Right
				glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
			glEnd();
		}
		if(c==1)
		{
			glBegin(GL_QUADS);									// Draw A Quad
				glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
				glTexCoord2f(1.0f,1.0f);glVertex3f(X+2, Y+2, Z);					// Top Right
				glTexCoord2f(1.0f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
				glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
			glEnd();
		}
		if(b==2) X -= ac;
		if(b==3) X += ac;
		fps += 0.1f;
		Sleep(100);
	}
	if(fps>=1) {state=0;fps=0;c=0;}
}
void	Action::still()									//人物处于静止状态
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+2, Z);					// Top Left
		glTexCoord2f(1.0f,1.0f);glVertex3f(X+2, Y+2, Z);					// Top Right
		glTexCoord2f(1.0f,0.0f);glVertex3f(X+2, Y, Z);					// Bottom Right
		glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
	glEnd();

}