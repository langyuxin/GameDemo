//������ײ���
#include"Class.h"

class GameManager{
public:
	GameManager();
	~GameManager();
	//�������˵���ײ���
	bool HumanCollision(Player player, Enemy enemy);
	//������ͼ֮�����ײ���
	//bool WorldCollisionY(Player player, Position thisPos, Position nextPos,int offSetY);

private:
	float p1x;
	float p1z;
	float p2x;
	float p2z;

};