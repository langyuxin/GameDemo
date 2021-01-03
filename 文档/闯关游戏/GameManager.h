//用于碰撞检测
#include"Class.h"

class GameManager{
public:
	GameManager();
	~GameManager();
	//玩家与敌人的碰撞检测
	bool HumanCollision(Player player, Enemy enemy);
	//玩家与障碍物的碰撞检测
	int BarriesCollision(Player p,rectangle r);
	//玩家与地图之间的碰撞检测
	//bool WorldCollisionY(Player player, Position thisPos, Position nextPos,int offSetY);

private:
	float px;
	float py;
	float ex;
	float ey;
	float rx;
	float ry;

};