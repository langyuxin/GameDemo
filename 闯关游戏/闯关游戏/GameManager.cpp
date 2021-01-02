#include "GameManager.h"

GameManager::GameManager()
{

}
GameManager::~GameManager()
{

}

bool GameManager::HumanCollision(Player player, Enemy enemy)
{
	px = player.getX();
	py = player.getY();
	ex = enemy.getX();
	ey = enemy.getY();

	//判定双方边界是否发生重叠，并返回结果,如果碰撞返回true
	if((px+p_weight) >= ex && px<=ex+e_height) return true;
	else return false;
}
int GameManager::BarriesCollision(Player p, rectangle r)
{
	px = p.getX();
	py = p.getY();
	rx = r.getX();
	ry = r.getY();

	//判定双方边界是否发生重叠，并返回结果,如果碰撞返回true
	if((px+p_weight)>=rx && py <(ry+r.getheight()) &&px<rx &&p.getstate()!=4) return 0;
	if((px+p_weight)>=(rx+r.getweight()) && py <(ry+r.getheight()) &&(px<=rx+r.getweight()) &&p.getstate()!=4) return 3;
	if((px+p_weight)>=rx && py <= (ry+r.getheight()) &&px<=(rx+r.getweight()) && p.getstate()==4)  return 1;
	if((px+p_weight)>=rx && py == (ry+r.getheight()) &&px<=(rx+r.getweight()) && p.getstate()!=4)  return 2;
	else  return -1;
}