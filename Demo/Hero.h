#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "Human.h"

#define ENEMY_DETECT_RANGE		10 //敌方探测范围
#define CHANCES_OF_PLAYER		3 //玩家初始命数
#define WALKSPEED_OF_PLAYER		1//玩家步行速度
#define RUNSPEED_OF_PLAYER		2//玩家奔跑速度
#define SPEED_OF_ENEMY			1//敌方速度
#define NUB_OF_ENEMY			3//敌方数量
#define HEIGHT_OF_JUMP			2//跳跃高度





class Player:public BaseObject
{
public:
	Player(float x,float y,float z);
	Player();
	virtual ~Player();
	int		getScore(){return Score;}
	int		getRunSpeed(){return RunSpeed;}
	int		getflag(){return flag;}

	void	setScore(int a){Score=a;}
	void	setRunSpeed(int b){RunSpeed=b;}
	void	setflag(int a){flag = a;}

	void	run();
	void	walk(GLfloat& roll,GLuint a,int b);			//传入参数roll判断玩家是否按下了移动建,b来确定按下的是哪个移动方向的键，a为传入的纹理
	void	attack(GLfloat& roll,GLuint a);			//参数roll判断玩家是否按下了攻击键，a为攻击的纹理
	void	die();
	void	jump();
	void	draw(GLuint a);				//参数d判断玩家是否按下了其他键来决定这一帧是否绘制人物静止图，a为人物静止时的纹理
protected:
	int Score;					//获得的分数
	int RunSpeed;
	int flag;
};