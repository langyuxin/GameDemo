#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#define ENEMY_DETECT_RANGE		10 //�з�̽�ⷶΧ
#define CHANCES_OF_PLAYER		3 //��ҳ�ʼ����
#define WALKSPEED_OF_PLAYER		1//��Ҳ����ٶ�
#define RUNSPEED_OF_PLAYER		2//��ұ����ٶ�
#define SPEED_OF_ENEMY			1//�з��ٶ�
#define NUB_OF_ENEMY			3//�з�����
#define HEIGHT_OF_JUMP			2//��Ծ�߶�



class  BaseObject				//�������
{
public:
	BaseObject(){};
	float	getX(){return X;}
	float	getY(){return Y;}
	float	getZ(){return Z;}
	float	getHP(){return HP;}
	float	getMP(){return MP;}
	float	getSpeed(){return Speed;}
	int		getTypeOfAttack(){return TypeOfAttack;}
	int		getChances(){return Chances;}
	int		getStrength(){return Strength;}
	int		getKind(){return Kind;}

	void	setX(float x){X=x;}
	void	setY(float y){Y=y;}
	void	setZ(float z){Z=z;}
	void	setHP(float hp){HP=hp;}
	void	setMP(float mp){MP=mp;}
	void	setSpeed(float s){Speed=s;}
	void	setTypeOfAttack(int a){TypeOfAttack=a;}
	void	setChances(int b){Chances=b;}
	void	setStrength(int c){Strength=c;}
	void	setKind(int d){Kind=d;}
protected:
	float HP;
	float MP;
	float Speed;			//�ٶ�
	int	TypeOfAttack;		//������ʽ
	int	Chances;			//����
	int	Strength;			//������������ɫ������
	int	Kind;				//��ɫ������

	float X,Y,Z;			//��ɫ�ռ�����
	
};

class Player:public BaseObject
{
public:
	Player(float x,float y,float z);
	virtual ~Player();
	int		getScore(){return Score;}
	int		getRunSpeed(){return RunSpeed;}
	int		getflagm(){return flagm;}
	int		getflagj(){return flagj;}
	int		getflaga(){return flaga;}
	int		getFace(){return Face;}
	int     getflagd(){return flagd;}


	void	setFace(int a){Face=a;}
	void	setScore(int a){Score=a;}
	void	setRunSpeed(int b){RunSpeed=b;}
	void	setflagm(int a){flagm = a;}
	void	setflaga(int a){flaga = a;}
	void	setflagj(int a){flagj = a;}
	void	setflagd(int a){flagd = a;}

	void	run();
	void	walk(GLfloat& roll,GLuint a,int b);			//�������roll�ж�����Ƿ������ƶ���,b��ȷ�����µ����ĸ��ƶ�����ļ���aΪ���������
	void	attack(GLfloat& roll,GLuint a);			//����roll�ж�����Ƿ����˹�������aΪ����������
	void	die(GLfloat& roll,GLuint a);
	void	jump();
	void	draw(GLuint a);				//aΪ���ﾲֹʱ������
protected:
	int Score;					//��õķ���
	int RunSpeed;
	int flagm,flaga,flagj,flagd;		//�ж������ʱ��״̬
	int		Face;				//����ķ���1:�����ң�-1��������
};

class Enemy:public BaseObject
{
public:
	Enemy(float x,float y,float z);
	virtual ~Enemy();
	float	getDetectRange(){return DetectRange;}
	float	getTime(){return Time;}
	int		getRewardScore(){return RewardScore;}
	int		getflagm(){return flagm;}
	int		getflagd(){return flagd;}
	int		getflaga(){return flaga;}


	void	setDetectRange(float a){DetectRange=a;}
	void	setRewardScore(int b){RewardScore=b;}
	void	setTime(float c){Time=c;}
	void	setflagm(int a){flagm=a;}
	void	setflagd(int a){flagd=a;}
	void	setflaga(int a){flaga=a;}

	void	walk(GLfloat& roll,GLuint a,int b);			//�������roll�ж�����Ƿ������ƶ���,b��ȷ�����µ����ĸ��ƶ�����ļ���aΪ���������
	void	attack(GLfloat& roll,GLuint a);			//����roll�ж�����Ƿ����˹�������aΪ����������
	void	die(GLfloat& roll,GLuint a);
	void	jump();
	void	draw(GLuint a);
protected:
	float	DetectRange;		//̽����ҷ�Χ
	int		RewardScore;		//�����ܽ�����ҵķ���
	float	Time;				//�����ж���޶�ʧ��ʱ��
	int flagm,flaga,flagd;		//�ж������ʱ��״̬

};
