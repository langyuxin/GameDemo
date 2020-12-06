#ifndef	CLASS_H
#define CLASS_H
//#include "Class.cpp"
// �궨��:
#define	PI					3.1415927	// ����PI��ֵ

#define MAX_AMMUNITION		500			// �ӵ�����Ĵ�С
#define MAX_COMPUTER		15			// ���Եĸ���
#define MAX_AWARD			40			// ��Ʒ�����С
#define MAX_PLAYER			10			// ��ҷɻ�����

#define AMMUNITION_RANGE	500			// �ӵ������
#define BOMB_RANGE			80			// ը�������
#define	BOMB_BLAST_RANGE	150			// ը����ը��Ӱ�췶Χ

#define START_BOMB_NUMBER	3			// ��ʼʱը������Ŀ
#define WIN_SCORE			20000		// �ﵽʤ���ķ���Ҫ��


// ����
class BaseObject{
public:
	BaseObject(float a=0,float b=0,int l=0,float s=0,float d=0,int k=0){
		x=a;
		y=b;
		life=l;
		speed=s;
		direction=d;
		kind=k;
		blastNum=0;
		blastTime=0;
	}

	float	getX(){return x;}
	float	getY(){return y;}
	int		getLife(){return life;}
	float	getSpeed(){return speed;}
	int		getKind(){return kind;}

	void	setX(float a){x=a;}
	void	setY(float b){y=b;}
	void	setLife(int l){life=l;}
	void	setSpeed(float s){speed=s;}

protected:
	float x;				// ��OBJECT��X������	
	float y;				// ��OBJECT��Y������
	float speed;			// ��OBJECT���ƶ��ٶ�
	int life;				// ��OBJECT������ֵ(����Чֵ)

	float direction;		// ��OBJECT�ƶ��ķ����,ȡ��X������н�
	int kind;				// ��OBJECT����������,�ڲ�ͬ�����������в�ͬ����˼

	int blastNum;			// ��ը����,��¼��OBJECT��ըʱ�ֻ���ͼƬ	
	int blastTime;			// ��ը��ʱ,������һ��ʱ����ֻ���ը����һ��ͼƬ
};


// ��ҷɻ���
class PlayerPlane:public BaseObject{
public:
	PlayerPlane(float a=0,float b=0,int l=0,float s=0);		// ���캯��,ͨ����ʼ��������,kind�ڸ�����û������
	void initPlane(float a,float b,int l,float s);			// ��窻�����
	
	int getAmmunitionKind() {return ammunitionKind;}
	int getBombNum() {return bombNum;}
	int getScore() {return score;}

	void setAmmunitionKind(int bk) {ammunitionKind=bk;}
	void setBombNum(int bn) {bombNum=bn;}
	void setScore(int s) {score=s;}

	void moveUp();			// ����
	void moveDown();		// ����
	void moveRight();		// ����
	void moveLeft();		// ����
	void stay();			// ����

	void draw();			// ���Ʒɻ�
	void blast();			// �����б�ը
	void fire();			// �����ӵ�
	void fireBomb();		// ����ը��
	void hitcomPlane();		// ��ײ���Է��ɻ�
	void update();			// ���·ɻ�����

private:
	int fireTime;			// �����������Ƶ�ʵı���,�Ա�֤�ӵ�������һ��
	int bombNum;			// �ɻ�Ŀǰ���ڵ�ը������
	int planeTexture;		// �ɻ�����ͼ,��Ϊ����,�м�,����������ͼ
	int ammunitionKind;		// Ŀǰ�ɻ����ӵ�����
	int score;				// ������˵õ��ķ���
};

// ���Էɻ��࣬��kind=0,1,2���ֵ��Էɻ�
class ComputerPlane:public BaseObject{
public:
	ComputerPlane(float a=0,float b=0,int l=0,float d=0,float s=0,int k=0);	//kind��ʾ��ͬ�ĵл�����

	bool getExplosible(){ return explosible;}
	void setExplosible(bool b) {explosible=b;}
	void setKind(int k);		// ���õл������࣬�������ڸ������µĵл�����

	void draw();				// �л����ƺ���
	void move();				// �л��ƶ�����
	void fire();				// �л��������
	void blast();				// �л���ը����
	void compinit();			// �л���ʼ������
	void update();				// ÿһ֡�жϵл������¼�
	void leftaward();			// �л�����������Ʒ������
	void damaged(int damage);	// ���õл��������˺���

private:
	int fireTime;				// �����������Ƶ�ʵı���,�Ա�֤�ӵ�������һ��
	bool explosible;			// ���Ƹõл��Ƿ�Ҫ��ը
	int rewardScore;			// �л��������������ķ���
};

// �ӵ��࣬��kind=0,1,2,3�����ӵ�
class Ammunition:public BaseObject{
public:
	Ammunition(float a=0,float b=0,float d=0,float s=0,int l=0,int k=0);		// kind��ʾ��ͬ���ӵ�����

	void setOwner(int w) {owner=w;}
	int getOwner() {return owner;}
	void setExplosible(bool bn) {explosible=bn;}
	int getExplodeLevel(void){return explodeLevel;}

	void move();			// �ӵ����ƶ�
	void draw();			// �ӵ�����
	void blast();			// �ӵ���ը
	void hitTheTarget();	// �ӵ��������

private:
	bool explosible;		// �Ƿ�ʼ��ը,ΪFALSE��ʾ���ñ�ը,ΪTRUE��ʾ��ʼ��ը
	float displacement;		// �ӵ��ƶ����ľ���
	int owner;				// �ӵ�����,1Ϊ���,3Ϊһ�����
	int explodeLevel;		// �ӵ���ɱ������
};

// ��Ʒ�࣬��kind=0,1,2������Ʒ
class Award:public BaseObject{
public:
	Award(float a=0,float b=0,float s=0,int l=0,int k=0);						//kind��ʾ��ͬ����Ʒ����

	void draw();			// ��Ʒ�Ļ��ƺ���
	void move();			// ��Ʒ���ƶ�����
	void eat();				// ��Ʒ�Ƿ���ҷɻ��Ե��ļ�⺯��
};

#endif