#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library


class Action
{
public:
	Action();

	void	InitialAct(int x,int y,int z);
	int		getstate()					{return state;}
	void	setstate(int a)				{state=a;}
	float	getfps()					{return fps;}
	void	setfps(float a)				{fps=a;}
	GLuint	gettexture()				{return texture;}
	void	settexture(GLuint a)		{texture=a;}

	void	attack(int& c);
	void	move(int b);
	void	die();
	void	jump(int b,int& c);
	void	still();

private:
	int		state;
	GLuint	texture;
	float	fps;
	float	X,Y,Z;

};
/*Class State
{


private:
	int state;
	
};*/