//用于碰撞检测


class GameMananger{
pubilc:
	GameMananger();
	//玩家与敌人的碰撞检测
	bool HumanCollision(Player player, Enemies enemy);
	//玩家与地图之间的碰撞检测
	//bool WorldCollisionY(Player player, Position thisPos, Position nextPos,int offSetY);

privite:
	int p1x;
	int p1y;
	int p2x;
	int p2y;

}