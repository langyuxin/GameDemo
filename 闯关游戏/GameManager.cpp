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

	//�ж�˫���߽��Ƿ����ص��������ؽ��,�����ײ����true
	return ( p1x < p2x + enemy.getWidth() &&
			 p2x < p1x + player.getWidth() &&
			 p1y < p2y + enemy.getHeight() &&
			 p2y < p1y + player.getHeight()	);
}