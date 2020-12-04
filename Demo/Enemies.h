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


class Enemy:public BaseObject
{
public:
	Enemy();
	float	getDetectRange(){return DetectRange;}
	float	getTime(){return Time;}
	int		getRewardScore(){return RewardScore;}

	void	setDetectRange(float a){DetectRange=a;}
	void	setRewardScore(int b){RewardScore=b;}
	void	setTime(float c){Time=c;}

	void	walk();
	void	attack();
	void	die();
	void	jump();
protected:
	float	DetectRange;		//̽����ҷ�Χ
	int		RewardScore;		//�����ܽ�����ҵķ���
	float	Time;				//�����ж���޶�ʧ��ʱ��
};