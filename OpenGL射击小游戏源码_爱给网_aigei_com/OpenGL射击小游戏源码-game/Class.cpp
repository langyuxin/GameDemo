#include <windows.h>
#include <math.h>
#include <gl\gl.h>

#include "Class.h"
#include "fmod.h"
#include "texture.h"

//各变量定义:
extern TextureTga	playertex[3];				// 纹理数组，保存纹理名字
extern TextureTga  computertex[3];
extern TextureTga	ammunitiontex[4];
extern TextureTga	awardtex[3];
extern TextureTga	othertex[4];

extern PlayerPlane myPlane;					// 我方飞机
extern ComputerPlane computers[MAX_COMPUTER];	// 电脑飞机数组
extern Ammunition ammunitions[MAX_AMMUNITION];		// 子弹数组
extern Award awards[MAX_AWARD];			// 物品数组
int ammunitionIndex=0;
int awardIndex=0;

extern FSOUND_SAMPLE *sound_1;			// 发射子弹的声音指针
extern FSOUND_SAMPLE *sound_2;			// 射击中的声音指针
extern FSOUND_SAMPLE *sound_3;			// 遇到物品的声音指针
extern FSOUND_SAMPLE *sound_4;			// 游戏的背影音乐指针

extern int myPlaneNum;					// 我方目前余下的飞机数目
extern bool end;						// 控制游戏结束的布尔值
extern DWORD endtime;					// 结束动画的起始时间
extern int timer;						// 这一帧的运行时间

// 计算点(x1,y1)与点(x2,y2)的距离
float distance(float x1,float y1,float x2,float y2)
{
	return (float)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

// 玩家飞机类定义
// 玩家飞机初始化
void PlayerPlane::initPlane(float a,float b,int l,float s)
{
	x=a;
	y=b;
	life=l;
	speed=s;

	blastNum=0;
	blastTime=0;

	bombNum=START_BOMB_NUMBER;
	planeTexture=0;
	fireTime=0;
	ammunitionKind=0;
	score=0;
}
PlayerPlane::PlayerPlane(float a,float b,int l,float s)
{
	initPlane(a,b,l,s);
}

// 更新飞机
void PlayerPlane::update()
{
	if (life<=0&&blastNum==8){		// 玩家飞机爆炸完毕
		myPlaneNum--;
		if (myPlaneNum!=0){			// 玩家还有剩余飞机
			int temp=score;
			initPlane(0,-230,100,2);
			score=temp;
		}else{
			// 游戏结束(失败)
			endtime=GetTickCount();
			end=true;
		}
	}else if(score>=WIN_SCORE){		// 游戏结束(胜利)
		endtime=GetTickCount();
		end=true;
	}
}

// 发射子弹
void PlayerPlane::fire()
{
	if(life>0){
		fireTime+=timer;
		if(fireTime>100){
			Ammunition t1(x-12,y+1,90,4,100.0f,ammunitionKind);	
			Ammunition t2(x+12,y+1,90,4,100.0f,ammunitionKind);
			t1.setOwner(1);
			t2.setOwner(1);
			
			ammunitions[ammunitionIndex++]=t1;
			if(ammunitionIndex==MAX_AMMUNITION)
				ammunitionIndex=0;

			ammunitions[ammunitionIndex++]=t2;
			if(ammunitionIndex==MAX_AMMUNITION)
				ammunitionIndex=0;

			fireTime-=100;
		}
	}
}

// 发射炸弹
void PlayerPlane::fireBomb()
{
	if(life>0 && bombNum>0){
		Ammunition t1(x,y+30,90,4,100.0f,3);
		t1.setOwner(1);
		ammunitions[ammunitionIndex++]=t1;
		if(ammunitionIndex==MAX_AMMUNITION)
			ammunitionIndex=0;
		bombNum--;
	}
}

// 检测并绘制飞机爆炸
void PlayerPlane::blast()
{
	if(life<=0){
		blastTime+=timer;
		if(blastNum<8){
			// 绘制爆炸图像
			glPushMatrix();
			glTranslatef(x, y, 0.0f);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, othertex[2].texID);
			glBegin(GL_QUADS);
				glTexCoord2d(blastNum*0.125f,0);	glVertex2f(-50.0f,-50.0f);
				glTexCoord2d((blastNum+1)*0.125f,0);glVertex2f( 50.0f,-50.0f);
				glTexCoord2d((blastNum+1)*0.125f,1);glVertex2f( 50.0f, 50.0f);
				glTexCoord2d(blastNum*0.125f,1);	glVertex2f(-50.0f, 50.0f);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			// 更新爆炸相关参数
			if(blastTime>30){
				if(blastNum==0)
					FSOUND_PlaySound(FSOUND_FREE,sound_2);
				blastNum++;
				blastTime=0;
			}
		}
	}
}

// 检测飞机是否相撞
void PlayerPlane::hitcomPlane()
{
	if(life>0){
		for(int i=0; i<MAX_COMPUTER; i++){
			if(distance(x,y,computers[i].getX(),computers[i].getY())<30 && computers[i].getLife()>0){
				life-=100;							// 飞机相撞,生命值减少100
				computers[i].setLife(0);			// 对方飞机爆炸
				computers[i].setExplosible(true);
				return ;
			}
		}
	}
}

// 飞机向上移动
void PlayerPlane::moveUp()
{
	if(y<280 && life>0){
		y+=speed*timer/10;
		planeTexture=0;
	}
}

// 飞机向下移动
void PlayerPlane::moveDown()
{
	if(y>-230 && life>0){
		y-=speed*timer/10;
		planeTexture=0;
	}
}

// 飞机向右移动
void PlayerPlane::moveRight()
{
	if(life>0){
		planeTexture=2;
		if(x<230.0){
			x+=speed*timer/10;
		}
	}
}

// 飞机向左移动
void PlayerPlane::moveLeft()
{
	if(life>0){
		planeTexture=1;
		if(x>-230.0){
			x-=speed*timer/10;
		}
	}
}

// 飞机原地不动
void PlayerPlane::stay()
{
	if(life>0)
		planeTexture=0;
}

// 绘制飞机
void PlayerPlane::draw()
{
	if(life>0){
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, playertex[planeTexture].texID);
		// 设置飞机颜色
		if(life>50)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_QUADS);
			glTexCoord2i(0.0f, 0.0f);glVertex2i(-20,-20);
			glTexCoord2i(1.0f, 0.0f);glVertex2i( 20,-20);
			glTexCoord2i(1.0f, 1.0f);glVertex2i( 20, 20);
			glTexCoord2i(0.0f, 1.0f);glVertex2i(-20, 20);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

// 电脑飞机类成员定义
ComputerPlane::ComputerPlane(float a,float b,int l,float d,float s,int k):BaseObject(a,b,l,s,d,k)
{
	fireTime=0;
}


// 电脑飞机的种类分为0，1，2三种，对于不同的种类，将会有不同的生命值和子弹属性等。
void ComputerPlane::setKind(int k)
{
	if(k==0){
		kind=0;
		speed=2;
		life=150;
		rewardScore=100;
	}else if(k==1){
		kind=1;
		speed=3;
		life=100;
		rewardScore=200;
	}else if(k==2){
		kind=2;
		speed=1;
		life=200;
		rewardScore=500;
	}
}

// 电脑飞机初始化
void ComputerPlane::compinit()
{
	x=(float)(rand()%500-250);
	y=(float)(rand()%100+300);
	direction=(float)(rand()%90-135);
	blastNum=0;
	explosible=false;
	// 初始化电脑飞机种类kind
	int temp=rand()%8;
	if(temp<=4)
		setKind(0);
	else if(temp==5 || temp==6)
		setKind(1);
	else if(temp==7)
		setKind(2);
}

// 更新电脑飞机
void ComputerPlane::update()
{
	if(life<=0 && blastNum==8 && explosible || life<=0 && !explosible){
		compinit();
		blastNum=0;
	}
}

// 绘制电脑飞机
void ComputerPlane::draw()
{
	if(life>0){
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, computertex[kind].texID);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);glVertex2i(-25,-25);
			glTexCoord2i(1, 0);glVertex2i( 25,-25);
			glTexCoord2i(1, 1);glVertex2i( 25, 25);
			glTexCoord2i(0, 1);glVertex2i(-25, 25);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

// 电脑飞机移动
void ComputerPlane::move()
{
	if(x>-275 && x<275 && y>-325){
		x=x+speed*cos(direction/180*PI)*timer/20;
		y=y+speed*sin(direction/180*PI)*timer/20;
	}else{
		life=0;
		explosible=false;
	}
}

// 电脑飞机移动以及子弹发射
void ComputerPlane::fire()
{
	if(life>0){
		fireTime+=timer;
		// 射击速度随机
		if((fireTime>1000+rand()%5000) && y>-100){
			if(kind==0){
				Ammunition t1(x,y-25,-90,3,100.0f,0);
				t1.setOwner(3);
				ammunitions[ammunitionIndex++]=t1;
				if(ammunitionIndex==MAX_AMMUNITION)
					ammunitionIndex=0;
				fireTime=0;
			}else if(kind==1){
				int temp=atan((myPlane.getY()-(y-25))/(myPlane.getX()-x))/PI*180;
				if(myPlane.getX()<x)		// 若飞机在子弹的左边，则加180度
					temp+=180;
				Ammunition t1(x,y-25,temp,3,100.0f,0);
				t1.setOwner(3);
				ammunitions[ammunitionIndex++]=t1;
				if(ammunitionIndex==MAX_AMMUNITION)
					ammunitionIndex=0;
				fireTime=0;
			}else if(kind==2){
				Ammunition t1(x,y-25,-90,3,100.0f,2);
				t1.setOwner(3);
				ammunitions[ammunitionIndex++]=t1;
				if(ammunitionIndex==MAX_AMMUNITION)
					ammunitionIndex=0;
				fireTime=0;
			}
		}
	}
}

// 电脑飞机爆炸
void ComputerPlane::blast()
{
	if(life<=0 && explosible && blastNum<8){
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,othertex[2].texID);
		glBegin(GL_QUADS);
			glTexCoord2f(blastNum*0.125f, 0.0f);		glVertex2f(-50.0f,-50.0f);
			glTexCoord2f(blastNum*0.125f+0.125f,0.0f);	glVertex2f( 50.0f,-50.0f);
			glTexCoord2f(blastNum*0.125f+0.125f,1.0f);	glVertex2f( 50.0f, 50.0f);
			glTexCoord2f(blastNum*0.125f, 1.0f);		glVertex2f(-50.0f, 50.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// 爆炸参数更新
		blastTime+=timer;
		if(blastTime>30){
			if(blastNum==0)
				FSOUND_PlaySound(FSOUND_FREE,sound_2);
			blastNum++;
			blastTime=0;
		}

		if(blastNum==8){	//爆炸完毕
			// 按照爆炸了的电脑飞机种类加分
			myPlane.setScore(myPlane.getScore()+rewardScore);
			
			// 类型2的高级飞机打完后会自爆
			if(kind==2){
				for(int i=0;i<20;i++){
					Ammunition t1(x,y-25,18*i,3,100.0f,0);
					t1.setOwner(3);
					ammunitions[ammunitionIndex++]=t1;
					if(ammunitionIndex==MAX_AMMUNITION)
						ammunitionIndex=0;
				}
			}
			// 飞机爆炸后留下物品
			leftaward();
		}
	}
}

// 电脑飞机爆炸后留下物品
void ComputerPlane::leftaward()
{
	int temp=rand()%10;

	if(temp==0){			// 高级子弹补给
		Award t1(x,y,1,100,0);
		awards[awardIndex++]=t1;
		if(awardIndex==MAX_AWARD)
			awardIndex=0;
	}else if(temp==1){		// 生命补给
		Award t1(x,y,1,100,1);
		awards[awardIndex++]=t1;
		if(awardIndex==MAX_AWARD)
			awardIndex=0;
	}else if(temp==2){		// 炸弹补给
		Award t1(x,y,1,100,2);
		awards[awardIndex++]=t1;
		if(awardIndex==MAX_AWARD)
			awardIndex=0;
	}
}

void ComputerPlane::damaged(int damage)
{
	life-=damage;
	if(life<=0)
		explosible=true;
}


// Ammunition类的成员定义
// 其种类有四种，分别由kind决定，有0，1，2，3四种。其中：
//	0为普通子弹，1为我方高级子弹，2为敌方高级子弹，3为我方的炸弹

// 子弹初始化
Ammunition::Ammunition(float a,float b,float d,float s,int l,int k):BaseObject(a,b,l,s,d,k)
{
	explosible=false;
	displacement=0;
	owner=0;
	if(kind==0)				//设置子弹的威力
		explodeLevel=20;	
	else if(kind==1)
		explodeLevel=50;
	else if(kind==2)
		explodeLevel=50;
	else if(kind==3)
		explodeLevel=100;
}

// 判断子弹是否打中飞机
void Ammunition::hitTheTarget()
{
	if(owner==1){		// 玩家发出的子弹
		for(int i=0; i<MAX_COMPUTER; i++){
			if(distance(computers[i].getX(),computers[i].getY(),x,y)<=25 && computers[i].getLife()>0){			//子弹在敌机范围内
				life=0;
				explosible=true;
				computers[i].damaged(explodeLevel);
				return;
			}
		}	
	}else if(owner==3){		// 电脑发出的子弹
		if(distance(myPlane.getX(),myPlane.getY(),x,y)<=20 && myPlane.getLife()>0){			//子弹在敌机范围内
			life=0;
			explosible=true;			
			myPlane.setLife(myPlane.getLife()-explodeLevel);
			
		}
	}
}

// 子弹移动
void Ammunition::move()
{
	if(life>0){			// 子弹有效
		if(kind!=3){		// 不是炸弹
			if(displacement<AMMUNITION_RANGE){	// 在射程内
				x=x+speed*cos(direction/180*PI)*timer/20;	
				y=y+speed*sin(direction/180*PI)*timer/20;
				displacement+=speed*timer/20;
				hitTheTarget();				// 检查是否射中
			}else{
				life=0;
				explosible=false;
			}
		}else{							// 炸弹
			if(displacement<BOMB_RANGE){
				x=x+speed*cos(direction/180*PI)*timer/20;	
				y=y+speed*sin(direction/180*PI)*timer/20;
				displacement+=speed*timer/20;
			}else{
				life=0;
				explosible=true;
			}
		}
	}
}

// 子弹爆炸
void Ammunition::blast()
{
	if(life<=0 && explosible && blastNum<8){		// 子弹无效且允许爆炸且未爆炸完
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		if(kind!=3){
			glBindTexture(GL_TEXTURE_2D, othertex[3].texID);
			glBegin(GL_QUADS);
				glTexCoord2d(blastNum*0.125f, 0.0f);		glVertex2f(-10.0f,-10.0f);
				glTexCoord2d(blastNum*0.125f+0.125f, 0.0f);	glVertex2f( 10.0f,-10.0f);
				glTexCoord2d(blastNum*0.125f+0.125f, 1.0f);	glVertex2f( 10.0f, 10.0f);
				glTexCoord2d(blastNum*0.125f, 1.0f);		glVertex2f(-10.0f, 10.0f);
			glEnd();
		}else{
			float temp=0.667*BOMB_BLAST_RANGE;
			glBindTexture(GL_TEXTURE_2D, othertex[2].texID);
			glBegin(GL_QUADS);
				glTexCoord2d(blastNum*0.125f, 0.0f);		glVertex2f(-temp,-temp);
				glTexCoord2d(blastNum*0.125f+0.125f, 0.0f);	glVertex2f( temp,-temp);
				glTexCoord2d(blastNum*0.125f+0.125f, 1.0f);	glVertex2f( temp, temp);
				glTexCoord2d(blastNum*0.125f, 1.0f);		glVertex2f(-temp, temp);
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// 爆炸参数更新
 		blastTime+=timer;
		if(blastTime>20){
			blastNum++;
			blastTime-=20;
		}
		
		// 炸弹爆炸清除范围内目标
		if(kind==3 && blastNum==4){
			// 清除范围内飞机
			for(int i=0; i<MAX_COMPUTER; i++){
				if(distance(computers[i].getX(),computers[i].getY(),x,y)<=BOMB_BLAST_RANGE && computers[i].getLife()>0){	
					computers[i].damaged(explodeLevel);
				}
			}
			// 清除范围内子弹
			for(i=0;i<MAX_AMMUNITION;i++){
				if(distance(ammunitions[i].getX(),ammunitions[i].getY(),x,y)<=BOMB_BLAST_RANGE
					&& ammunitions[i].getLife()>0
					&& ammunitions[i].getOwner()!=owner)
				{
					ammunitions[i].setLife(0);
					ammunitions[i].setExplosible(true);
				}
			}
		}
	}
}

// 绘制子弹
void Ammunition::draw()
{
	if(life>0)
	{
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		switch(kind)
		{
		case 0:			// 绘制普通的子弹
			{
				glBindTexture(GL_TEXTURE_2D, ammunitiontex[0].texID);
				glBegin(GL_QUADS);
					glTexCoord2i(0,0);glVertex2i(-3,-3);
					glTexCoord2i(1,0);glVertex2i( 3,-3);
					glTexCoord2i(1,1);glVertex2i( 3, 3);
					glTexCoord2i(0,1);glVertex2i(-3, 3);
				glEnd();
				break;
			}
		case 1:			// 绘制玩家的高级子弹
			{
				glBindTexture(GL_TEXTURE_2D, ammunitiontex[1].texID);
				glBegin(GL_QUADS);
					glTexCoord2i(0, 0);glVertex2i(-3,-3);
					glTexCoord2i(1, 0);glVertex2i( 3,-3);
					glTexCoord2i(1, 1);glVertex2i( 3, 3);
					glTexCoord2i(0, 1);glVertex2i(-3, 3);
				glEnd();
				break;
			}
		case 2:			// 绘制电脑的高级子弹
			{
				glBindTexture(GL_TEXTURE_2D, ammunitiontex[0].texID);
				glColor3f(0.0f, 1.0f, 1.0f);
				glBegin(GL_QUADS);
					glTexCoord2i(0, 0);glVertex2i(-5,-5);
					glTexCoord2i(1, 0);glVertex2i( 5,-5);
					glTexCoord2i(1, 1);glVertex2i( 5, 5);
					glTexCoord2i(0, 1);glVertex2i(-5, 5);
				glEnd();
				glColor3f(1.0f, 1.0f, 1.0f);
				break;
			}
		case 3:			// 绘制炸弹
			{
				glBindTexture(GL_TEXTURE_2D, ammunitiontex[3].texID);
				glBegin(GL_QUADS);
					glTexCoord2i(0, 0);glVertex2i(-15,-15);
					glTexCoord2i(1, 0);glVertex2i( 15,-15);
					glTexCoord2i(1, 1);glVertex2i( 15, 15);
					glTexCoord2i(0, 1);glVertex2i(-15, 15);
				glEnd();
				break;
			}
		default:
			break;
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

// 物品成员定义
// 由kind决定种类，分为0 高级子弹补给，1 炸弹补给，2 生命补给

// 初始化
Award::Award(float a,float b,float s,int l,int k):BaseObject(a,b,l,s,-1,k)
{
}


// 物品绘制
void Award::draw()
{
	if(life>0){
		glPushMatrix();
		glTranslatef(x, y, 0.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, awardtex[kind].texID);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);glVertex2f(-10.0f,-10.0f);
			glTexCoord2f(1.0f,0.0f);glVertex2f( 10.0f,-10.0f);
			glTexCoord2f(1.0f,1.0f);glVertex2f( 10.0f, 10.0f);
			glTexCoord2f(0.0f,1.0f);glVertex2f(-10.0f, 10.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

// 物品移动
void Award::move()
{
	if(life>0){
		if(x>-240 && x<240 && y>-310 && y<300)
			y-=speed*timer/20;
		else
			life=0;
	}
}

// 判断物体是否被玩家飞机捡到
void Award::eat()
{
	if(life>0 && distance(x,y, myPlane.getX(),myPlane.getY())<30)
	{
		switch(kind)
		{
		case 0:			// 高级子弹补给
			{
				if(myPlane.getAmmunitionKind()==1){
					for(int i=0;i<40;i++){
						Ammunition t1(myPlane.getX(),myPlane.getY(),9*i,3,100.0f,1);
						t1.setOwner(1);
						ammunitions[ammunitionIndex++]=t1;
						if(ammunitionIndex==MAX_AMMUNITION)
							ammunitionIndex=0;
					}
				}
				myPlane.setAmmunitionKind(1);
				life=0;
				break;
			}
		case 1:			// 生命补给
			{
				if(myPlane.getLife()<50)
					myPlane.setLife(myPlane.getLife()+50);
				else if(myPlane.getLife()<100)
					myPlane.setLife(100);
				life=0;
				break;
			}
		case 2:			// 炸弹补给
			{
				myPlane.setBombNum(myPlane.getBombNum()+1);
				life=0;
				break;
			}
		default:
			break;
		}
		FSOUND_PlaySound(FSOUND_FREE,sound_3);
	}
}