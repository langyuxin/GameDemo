#include <afxwin.h>
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#define ENEMY_DETECT_RANGE		10 //敌方探测范围
#define CHANCES_OF_PLAYER		3 //玩家初始命数
#define WALKSPEED_OF_PLAYER		1//玩家步行速度
#define RUNSPEED_OF_PLAYER		2//玩家奔跑速度
#define SPEED_OF_ENEMY			1//敌方速度
#define NUB_OF_ENEMY			3//敌方数量
#define HEIGHT_OF_JUMP			2//跳跃高度



class  BaseObject				//人物基类
{
public:
	BaseObject(){};
	float	getX(){return X;}
	float	getY(){return Y;}
	float	getZ(){return Z;}
	float	getHP(){return HP;}
	float	getMP(){return MP;}
	float	getSpeed(){return Speed;}
	int		getTypeOfAttack(){return TypeOfAttack;}
	int		getChances(){return Chances;}
	int		getStrength(){return Strength;}
	int		getKind(){return Kind;}

	void	setX(float x){X=x;}
	void	setY(float y){Y=y;}
	void	setZ(float z){Z=z;}
	void	setHP(float hp){HP=hp;}
	void	setMP(float mp){MP=mp;}
	void	setSpeed(float s){Speed=s;}
	void	setTypeOfAttack(int a){TypeOfAttack=a;}
	void	setChances(int b){Chances=b;}
	void	setStrength(int c){Strength=c;}
	void	setKind(int d){Kind=d;}
protected:
	float HP;
	float MP;
	float Speed;			//速度
	int	TypeOfAttack;		//攻击方式
	int	Chances;			//命数
	int	Strength;			//力量，决定角色攻击力
	int	Kind;				//角色的种类

	float X,Y,Z;			//角色空间坐标
	
};