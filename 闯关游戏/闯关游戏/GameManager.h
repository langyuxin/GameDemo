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

private:
	float px;
	float py;
	float ex;
	float ey;
	float rx;
	float ry;

};