#ifndef	CLASS_H
#define CLASS_H
//#include "Class.cpp"
// 宏定义:
#define	PI					3.1415927	// 定义PI的值

#define MAX_AMMUNITION		500			// 子弹数组的大小
#define MAX_COMPUTER		15			// 电脑的个数
#define MAX_AWARD			40			// 物品数组大小
#define MAX_PLAYER			10			// 玩家飞机个数

#define AMMUNITION_RANGE	500			// 子弹的射程
#define BOMB_RANGE			80			// 炸弹的射程
#define	BOMB_BLAST_RANGE	150			// 炸弹爆炸的影响范围

#define START_BOMB_NUMBER	3			// 开始时炸弹的数目
#define WIN_SCORE			20000		// 达到胜利的分数要求


// 基类
class BaseObject{
public:
	BaseObject(float a=0,float b=0,int l=0,float s=0,float d=0,int k=0){
		x=a;
		y=b;
		life=l;
		speed=s;
		direction=d;
		kind=k;
		blastNum=0;
		blastTime=0;
	}

	float	getX(){return x;}
	float	getY(){return y;}
	int		getLife(){return life;}
	float	getSpeed(){return speed;}
	int		getKind(){return kind;}

	void	setX(float a){x=a;}
	void	setY(float b){y=b;}
	void	setLife(int l){life=l;}
	void	setSpeed(float s){speed=s;}

protected:
	float x;				// 该OBJECT的X轴坐标	
	float y;				// 该OBJECT的Y轴坐标
	float speed;			// 该OBJECT的移动速度
	int life;				// 该OBJECT的生命值(或有效值)

	float direction;		// 该OBJECT移动的方向角,取与X正方向夹角
	int kind;				// 该OBJECT所属的类型,在不同的派生类中有不同的意思

	int blastNum;			// 爆炸记数,记录该OBJECT爆炸时轮换的图片	
	int blastTime;			// 爆炸记时,当超过一定时间才轮换爆炸的下一张图片
};


// 玩家飞机类
class PlayerPlane:public BaseObject{
public:
	PlayerPlane(float a=0,float b=0,int l=0,float s=0);		// 构造函数,通过初始化函数来,kind在该类中没有意义
	void initPlane(float a,float b,int l,float s);			// 初绐化函数
	
	int getAmmunitionKind() {return ammunitionKind;}
	int getBombNum() {return bombNum;}
	int getScore() {return score;}

	void setAmmunitionKind(int bk) {ammunitionKind=bk;}
	void setBombNum(int bn) {bombNum=bn;}
	void setScore(int s) {score=s;}

	void moveUp();			// 上移
	void moveDown();		// 下移
	void moveRight();		// 右移
	void moveLeft();		// 左移
	void stay();			// 不动

	void draw();			// 绘制飞机
	void blast();			// 被击中爆炸
	void fire();			// 发射子弹
	void fireBomb();		// 发射炸弹
	void hitcomPlane();		// 碰撞到对方飞机
	void update();			// 更新飞机参数

private:
	int fireTime;			// 辅助控制射击频率的变量,以保证子弹的射速一致
	int bombNum;			// 飞机目前存在的炸弹个数
	int planeTexture;		// 飞机的贴图,分为左移,中间,右移三张贴图
	int ammunitionKind;		// 目前飞机的子弹类型
	int score;				// 歼灭敌人得到的分数
};

// 电脑飞机类，有kind=0,1,2三种电脑飞机
class ComputerPlane:public BaseObject{
public:
	ComputerPlane(float a=0,float b=0,int l=0,float d=0,float s=0,int k=0);	//kind表示不同的敌机种类

	bool getExplosible(){ return explosible;}
	void setExplosible(bool b) {explosible=b;}
	void setKind(int k);		// 设置敌机的种类，并设置在该种类下的敌机属性

	void draw();				// 敌机绘制函数
	void move();				// 敌机移动函数
	void fire();				// 敌机射击函数
	void blast();				// 敌机爆炸函数
	void compinit();			// 敌机初始化函数
	void update();				// 每一帧判断敌机处理事件
	void leftaward();			// 敌机死后留下物品处理函数
	void damaged(int damage);	// 设置敌机击中损伤函数

private:
	int fireTime;				// 辅助控制射击频率的变量,以保证子弹的射速一致
	bool explosible;			// 控制该敌机是否要爆炸
	int rewardScore;			// 敌机被击灭所奖励的分数
};

// 子弹类，有kind=0,1,2,3四种子弹
class Ammunition:public BaseObject{
public:
	Ammunition(float a=0,float b=0,float d=0,float s=0,int l=0,int k=0);		// kind表示不同的子弹类型

	void setOwner(int w) {owner=w;}
	int getOwner() {return owner;}
	void setExplosible(bool bn) {explosible=bn;}
	int getExplodeLevel(void){return explodeLevel;}

	void move();			// 子弹的移动
	void draw();			// 子弹绘制
	void blast();			// 子弹爆炸
	void hitTheTarget();	// 子弹射中与否

private:
	bool explosible;		// 是否开始爆炸,为FALSE表示不用爆炸,为TRUE表示开始爆炸
	float displacement;		// 子弹移动过的距离
	int owner;				// 子弹所属,1为玩家,3为一般电脑
	int explodeLevel;		// 子弹的杀伤力力
};

// 物品类，有kind=0,1,2三种物品
class Award:public BaseObject{
public:
	Award(float a=0,float b=0,float s=0,int l=0,int k=0);						//kind表示不同的物品类型

	void draw();			// 物品的绘制函数
	void move();			// 物品的移动函数
	void eat();				// 物品是否被玩家飞机吃掉的检测函数
};

#endif