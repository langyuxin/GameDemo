#include "GameManager.h"

GameManager::GameManager(){
	p1x = 0;
	p1z = 0;
	p2x = 0;
	p2z = 0;
}
GameManager::~GameManager(){

}

bool GameManager::HumanCollision(Player player, Enemy enemy){
	p1x = player.getX();
	p1z = player.getZ();
	p2x = enemy.getX();
	p2z = enemy.getZ();

	//�ж�˫���߽��Ƿ����ص��������ؽ��,�����ײ����true
	if((p1x+2) >= p2x && p1x<=p2x+2) return true;
	else return false;
}