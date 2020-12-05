#include "GameManager.h"

GameManager::GameManager(){
	p1x = 0;
	p1y = 0;
	p2x = 0;
	p2y = 0;
}
GameManager::~GameManager(){

}

bool GameManager::HumanCollision(Player player, Enemies enemy){
	p1x = player.getX();
	p1y = player.getY();
	p2x = enemy.getX();
	p2y = enemy.getY();

	//判定双方边界是否发生重叠，并返回结果,如果碰撞返回true
	return ( p1x < p2x + enemy.getWidth() &&
			 p2x < p1x + player.getWidth() &&
			 p1y < p2y + enemy.getHeight() &&
			 p2y < p1y + player.getHeight()	);
}