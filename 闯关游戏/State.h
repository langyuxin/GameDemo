#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#define  p_height 0.6
#define  p_weight 0.6
#define  e_height 0.6
#define  e_weight 0.6
#define	b_weight 0.3
#define b_height 0.3
#define	t_weight 0.6
#define t_height 0.6

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
	int		gettexturetype()					{return texturetype;}
	void	settexturetype(int a)				{texturetype=a;}

	void	attack(int& c);
	void	move(int b);
	void	die();
	void	jump(int b,int& c);
	void	still();

private:
	int		state;
	GLuint	texture;
	int		texturetype;
	float	fps;
	float	X,Y,Z;

};
/*Class State
{


private:
	int state;
	
};*/