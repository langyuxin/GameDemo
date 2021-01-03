#include <windows.h>
#include <math.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <time.h>
#include "State.h"

Action::Action()
{
//	InitialAct(x,y,z);
}

void Action::InitialAct(float x,float y,float z)
{
	fps=0;
	state=0;
	X=x;
	Y=y;
	Z=z;
}
void Action::UpdatePos(float x,float y,float z)
{
	X=x;
	Y=y;
	Z=z;
}
void	Action::attack(int& c)
{
	float a;
	if(texturetype==0) a=0.25f;
	if(texturetype==1) a=0.1f;
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+p_height,Z);					// Top Left
			glTexCoord2f(fps+a,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
			glTexCoord2f(fps+a,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		fps += a;
	//	Sleep(20);
	}
	if(fps>=1) {state=0;fps=0;c=0;}
}
void	Action::move(int b)
{
	float a;
	if(texturetype==0) a=0.25f;
	if(texturetype==1) a=0.1f;
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
			glTexCoord2f(fps+a,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
			glTexCoord2f(fps+a,0.0f);glVertex3f( X+p_weight, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		GLfloat ac=0.05f;
		if(b==2) X -= ac;
		if(b==3) X += ac;
		if(b==1) Z += ac;
		if(b==0) Z -= ac;
		fps += a;
	//	Sleep(10);
	}

	if(fps>=1) {state=0;fps=0;}


}
void	Action::die()
{
	float a;
	if(texturetype==0) a=0.33f;
	if(texturetype==1) a=0.1f;
	if(fps<1)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(fps,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
			glTexCoord2f(fps+a,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
			glTexCoord2f(fps+a,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
			glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
		glEnd();
		fps += a;
	//	Sleep(20);
	}
	if(fps>=1) {fps=0;}

}
void	Action::jump(int b,int& c)
{
	float a=0.1f;
	if(fps<1)
	{
		
		GLfloat ac=0.1f,ad=0.2f;
		if(fps<=0.2f) Y +=ad;
		if(fps>=0.7f && fps<1.0f) Y -=ad;
		glBindTexture(GL_TEXTURE_2D, texture);
		if(c==0)
		{
			if(texturetype==0)
			{
				if(fps<0.5)
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					glBegin(GL_QUADS);									// Draw A Quad
						glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
						glTexCoord2f(0.5f,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
						glTexCoord2f(0.5f,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
						glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
					glEnd();
				}
				if(fps>=0.5)
				{
					glBindTexture(GL_TEXTURE_2D, texture);
					glBegin(GL_QUADS);									// Draw A Quad
						glTexCoord2f(0.5f,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
						glTexCoord2f(1.0f,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
						glTexCoord2f(1.0f,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
						glTexCoord2f(0.5f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
					glEnd();
				}
			}
			if(texturetype==1)
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				glBegin(GL_QUADS);									// Draw A Quad
					glTexCoord2f(fps,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
					glTexCoord2f(fps+a,1.0f);glVertex3f( X+p_weight, Y+p_height, Z);					// Top Right
					glTexCoord2f(fps+a,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
					glTexCoord2f(fps,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
				glEnd();
			}
		}
		if(c==1)
		{
			glBegin(GL_QUADS);									// Draw A Quad
				glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
				glTexCoord2f(1.0f,1.0f);glVertex3f(X+p_weight, Y+p_height, Z);					// Top Right
				glTexCoord2f(1.0f,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
				glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
			glEnd();
		}
		if(b==2) X -= ac;
		if(b==3) X += ac;
		fps += a;
	//	Sleep(10);
	}
	if(fps>=1) {state=0;fps=0;c=0;}
}
void	Action::still()
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0f,1.0f);glVertex3f(X, Y+p_height, Z);					// Top Left
		glTexCoord2f(1.0f,1.0f);glVertex3f(X+p_weight, Y+p_height, Z);					// Top Right
		glTexCoord2f(1.0f,0.0f);glVertex3f(X+p_weight, Y, Z);					// Bottom Right
		glTexCoord2f(0.0f,0.0f);glVertex3f(X, Y, Z);					// Bottom Left
	glEnd();

}