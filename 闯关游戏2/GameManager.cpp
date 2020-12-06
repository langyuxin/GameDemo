#include "GameManager.h"

GameManager::GameManager(){
	p1x = 0;
	p1y = 0;
	p2x = 0;
	p2y = 0;
}
GameManager::~GameManager(){

}

bool GameManager::HumanCollision(Player player, Enemy enemy){
	p1x = player.getX();
	p1y = player.getY();
	p2x = enemy.getX();
	p2y = enemy.getY();

	//�ж�˫���߽��Ƿ����ص��������ؽ��,�����ײ����true
	return ( p1x < p2x + 1.0 &&
			 p2x < p1x + 1.0 &&
			 p1y < p2y + 1.0 &&
			 p2y < p1y + 1.0	);
}