//������ײ���
#include"Class.h"


class GameManager{
public:
	GameManager();
	~GameManager();
	//�������˵���ײ���
	bool HumanCollision(Player player, Enemy enemy);
	//������ϰ������ײ���
	int BarriesCollision(Player p,rectangle r);
	//������ͼ֮�����ײ���
	//bool WorldCollisionY(Player player, Position thisPos, Position nextPos,int offSetY);
	bool BulletCollision(Player p,Bullet b);

private:
	float px;
	float py;
	float pz;
	float ex;
	float ey;
	float ez;
	float rx;
	float ry;
	float rz;
	float bx;
	float by;
	float bz;

};