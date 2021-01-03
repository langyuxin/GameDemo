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
	pz = player.getZ();
	ex = enemy.getX();
	ey = enemy.getY();
	ez = enemy.getZ();

	//判定双方边界是否发生重叠，并返回结果,如果碰撞返回true
	if((px+p_weight) >= ex && px-p_weight<=ex+e_height && pz==ez) return true;
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
	/*if((px+p_weight+0.05) >=rx && p.getstate() == 2 &&p.getmovedirect()==3) return 0;
	if((px-0.05) <=rx+r.getweight()&& p.getstate() == 2 &&p.getmovedirect()==2) return 1;
	if((py-0.25) <=ry+r.getheight()&& p.getstate() == 4 &&p.getfps()>0.75) return 2;*/
	else  return -1;
}

bool	GameManager::BulletCollision(Player p,Bullet b)
{
	px = p.getX();
	py = p.getY();
	pz = p.getZ();
	bx = b.getX();
	by = b.getY();
	bz = b.getZ();

	//判定双方边界是否发生重叠，并返回结果,如果碰撞返回true
	if((px+p_weight) >= (bx) && px-p_weight<=bx+b_weight && py<=by+b_height && pz==bz) return true;
	else return false;
}