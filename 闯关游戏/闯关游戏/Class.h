#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "State.h"

#define ENEMY_DETECT_RANGE		10 //敌方探测范围
#define CHANCES_OF_PLAYER		1 //玩家初始命数
#define WALKSPEED_OF_PLAYER		1//玩家步行速度
#define RUNSPEED_OF_PLAYER		2//玩家奔跑速度
#define SPEED_OF_ENEMY			1//敌方速度
#define NUB_OF_ENEMY			3//敌方数量
#define HEIGHT_OF_JUMP			2//跳跃高度




class  BaseObject				//人物基类
{
public:
	BaseObject(){};
	float	getX()					{return X;}
	float	getY()					{return Y;}
	float	getZ()					{return Z;}
	float	getHP()					{return HP;}
	float	getMP()					{return MP;}
	float	getSpeed()				{return Speed;}
	int		getTypeOfAttack()		{return TypeOfAttack;}
	int		getChances()			{return Chances;}
	int		getStrength()			{return Strength;}
	int		getKind()				{return Kind;}
	int		getFace()				{return Face;}
	int		getattackstate()		{return attackstate;}
	int		getmovedirect()			{return movedirect;}
	int		getstate()				{return Act.getstate();}
	float	getfps()				{return Act.getfps();}

	void	setmovedirect(int a)	{movedirect = a;}
	void	setattackstate(int a)	{attackstate=a;}
	void	setFace(int a)			{Face=a;}
	void	setX(float x)			{X=x;}
	void	setY(float y)			{Y=y;}
	void	setZ(float z)			{Z=z;}
	void	setHP(float hp)			{HP=hp;}
	void	setMP(float mp)			{MP=mp;}
	void	setSpeed(float s)		{Speed=s;}
	void	setTypeOfAttack(int a)	{TypeOfAttack=a;}
	void	setChances(int b)		{Chances=b;}
	void	setStrength(int c)		{Strength=c;}
	void	setKind(int d)			{Kind=d;}

	void	setattack(GLuint a[2])	{attack[0]=a[0];attack[1]=a[1];}
	void	setstill(GLuint a[2])	{still[0]=a[0];still[1]=a[1];}
	void	setdie(GLuint a)		{die=a;}
	void	setmove(GLuint a[4])	{move[0]=a[0];move[1]=a[1];move[2]=a[2];move[3]=a[3];}
	void	setstate(int a)			{Act.setstate(a);}
	void	setfps(float a)			{Act.setfps(a);}
protected:
	float HP;
	float MP;
	float Speed;			//速度
	int	TypeOfAttack;		//攻击方式
	int	Chances;			//命数
	int	Strength;			//力量，决定角色攻击力
	int	Kind;				//角色的种类

	float X,Y,Z;			//角色空间坐标

	int		Face;				//朝向的方向1:脸朝右，-1：脸朝左；
	GLuint	attack[2];			//攻击纹理，0为向左，1为向右
	GLuint	move[4];			//移动纹理，0,1,2,3对应w,s,a,d
	GLuint	still[2];			//静止时纹理，0,1分别为向左，向右
	GLuint	die;
	Action	Act;
	int		movedirect;			//移动方向
	int		attackstate;		//是否处于攻击状态：0：不是；1：是

	
};

class Player:public BaseObject
{
public:
	Player(float x,float y,float z);
	Player();
	virtual ~Player();
	void	act();
	void	Initial(float x,float y,float z);

	int		getScore()				{return Score;}
	int		getRunSpeed()			{return RunSpeed;}

	void	setScore(int a)			{Score=a;}
	void	setRunSpeed(int b)		{RunSpeed=b;}
	
	void	setjump(GLuint a[4])	{jump[0]=a[0];jump[1]=a[1];jump[2]=a[2];jump[3]=a[3];}

protected:
	int Score;					//获得的分数
	int RunSpeed;
	GLuint	jump[4];			//跳跃纹理，0,1分别为向左，向右
};

class Enemy:public BaseObject
{
public:
	Enemy(float x,float y,float z);
	Enemy() ;
	virtual ~Enemy();
	void	act();
	void	Initial(float x,float y,float z);

	float	getDetectRange()		{return DetectRange;}
	float	getTime()				{return Time;}
	int		getRewardScore()		{return RewardScore;}



	void	setDetectRange(float a)	{DetectRange=a;}
	void	setRewardScore(int b)	{RewardScore=b;}
	void	setTime(float c)		{Time=c;}



protected:
	float	DetectRange;		//探测玩家范围
	int		RewardScore;		//被击败奖励玩家的分数
	float	Time;				//用于判定仇恨丢失的时间
};


class rectangle
{
public:
	rectangle();
	virtual ~rectangle();

	void	InitialRec(float x,float y,float z,float w,float l);

	float	getX()					{return X;}
	void	setX(float x)			{X=x;}
	float	getY()					{return Y;}
	void	setY(float x)			{Y=x;}
	float	getZ()					{return Z;}
	void	setZ(float x)			{Z=x;}
	float	getweight()					{return weight;}
	void	setweight(float x)			{weight=x;}
	float	getheight()					{return height;}
	void	setheight(float x)			{height=x;}

private:
	float	X,Y,Z;				//长方形左下角的坐标

	float	weight,height;		//长和宽

};