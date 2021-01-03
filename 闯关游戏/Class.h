#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include <time.h>
#include "State.h"

#define ENEMY_DETECT_RANGE		10 //�з�̽�ⷶΧ
#define CHANCES_OF_PLAYER		1 //��ҳ�ʼ����
#define WALKSPEED_OF_PLAYER		1//��Ҳ����ٶ�
#define RUNSPEED_OF_PLAYER		2//��ұ����ٶ�
#define SPEED_OF_ENEMY			1//�з��ٶ�
#define NUB_OF_ENEMY			3//�з�����
#define HEIGHT_OF_JUMP			2//��Ծ�߶�


#define  p_height 0.6
#define  p_weight 0.6
#define  e_height 0.6
#define  e_weight 0.6
#define	b_weight 0.3
#define b_height 0.3
#define	t_weight 0.6
#define t_height 0.6

class  BaseObject				//�������
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
	float Speed;			//�ٶ�
	int	TypeOfAttack;		//������ʽ
	int	Chances;			//����
	int	Strength;			//������������ɫ������
	int	Kind;				//��ɫ������

	float X,Y,Z;			//��ɫ�ռ�����

	int		Face;				//����ķ���1:�����ң�-1��������
	GLuint	attack[2];			//��������0Ϊ����1Ϊ����
	GLuint	move[4];			//�ƶ�����0,1,2,3��Ӧw,s,a,d
	GLuint	still[2];			//��ֹʱ����0,1�ֱ�Ϊ��������
	GLuint	die;
	Action	Act;
	int		movedirect;			//�ƶ�����
	int		attackstate;		//�Ƿ��ڹ���״̬��0�����ǣ�1����

	
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
	void	settexturetype(int a)		{Act.settexturetype(a);}
	
	void	setjump(GLuint a[4])	{jump[0]=a[0];jump[1]=a[1];jump[2]=a[2];jump[3]=a[3];}

protected:
	int Score;					//��õķ���
	int RunSpeed;
	GLuint	jump[4];			//��Ծ����0,1�ֱ�Ϊ��������
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
	void	settexturetype(int a)		{Act.settexturetype(a);}



	void	setDetectRange(float a)	{DetectRange=a;}
	void	setRewardScore(int b)	{RewardScore=b;}
	void	setTime(float c)		{Time=c;}



protected:
	float	DetectRange;		//̽����ҷ�Χ
	int		RewardScore;		//�����ܽ�����ҵķ���
	float	Time;				//�����ж���޶�ʧ��ʱ��
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
	float	X,Y,Z;				//���������½ǵ�����

	float	weight,height;		//���Ϳ�

};


class Trap						//�����ࣺ������������
{
public:

	void	setx(float a,float b)	{x1=a;x2=b;}
	float	getx1()					{return x1;}
	float	getx2()					{return x2;}
private:
	float	x1,x2;				//�ӵ�ǰ������

};

class	Bullet
{
public:
	void		Initial(float x,float y,float z);	
	void		draw();

	int			getdirect()							{return direct;}
	float		getX()								{return X;}
	float		getY()								{return Y;}
	float		getZ()								{return Z;}
	float		getspeed()							{return speed;}

	void		setspeed(float a)					{speed=a;}	
	void		setX(float a)						{X=a;}
	void		setY(float a)						{Y=a;}
	void		setZ(float a)						{Z=a;}
	void		setbullet(GLuint a[2])				{bullet[0]=a[0];bullet[1]=a[1];}
	void		setdirect(int a)					{direct=a;}

private:
	GLuint		bullet[2];
	int			direct;
	float		X,Y,Z;
	float		speed;
	float		fps;
};

class	Zhaoyun:public Enemy
{
public:
	void	setdistance(float x)		{distance=x;}
	float	getdistance()				{return distance;}
	void	settime(float x)			{time=x;}
	float	gettime()					{return time;}

	void	setbullet(GLuint a[2])		{bullet.setbullet(a);}
	void	setdirect(int a)			{bullet.setdirect(a);}

private:
	float	distance;			//����ҵľ���
	float	time;				//ǰ��ҡʱ��
	Bullet  bullet;				//������ӵ�

};

