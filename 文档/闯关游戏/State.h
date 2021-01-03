#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#define  p_height 0.5
#define  p_weight 0.5
#define  e_height 0.5
#define  e_weight 0.5
class Action
{
public:
	Action();

	void	InitialAct(float x,float y,float z);
	void	UpdatePos(float x,float y,float z);

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