#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "Human.h"

#define ENEMY_DETECT_RANGE		10 //�з�̽�ⷶΧ
#define CHANCES_OF_PLAYER		3 //��ҳ�ʼ����
#define WALKSPEED_OF_PLAYER		1//��Ҳ����ٶ�
#define RUNSPEED_OF_PLAYER		2//��ұ����ٶ�
#define SPEED_OF_ENEMY			1//�з��ٶ�
#define NUB_OF_ENEMY			3//�з�����
#define HEIGHT_OF_JUMP			2//��Ծ�߶�





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
	void	walk(GLfloat& roll,GLuint a,int b);			//�������roll�ж�����Ƿ������ƶ���,b��ȷ�����µ����ĸ��ƶ�����ļ���aΪ���������
	void	attack(GLfloat& roll,GLuint a);			//����roll�ж�����Ƿ����˹�������aΪ����������
	void	die();
	void	jump();
	void	draw(GLuint a);				//����d�ж�����Ƿ�������������������һ֡�Ƿ�������ﾲֹͼ��aΪ���ﾲֹʱ������
protected:
	int Score;					//��õķ���
	int RunSpeed;
	int flag;
};