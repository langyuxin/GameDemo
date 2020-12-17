////在此处定义WINDOW头文件
#include <windows.h>		// Windows 头文件
#include <stdio.h>

//在此处引用程序要求的附加头文件:
#include <gl\gl.h>			// OpenGL32库的头文件
#include <gl\glu.h>			// Glu32库的头文件

//在此处引用本程序的头文件
#include "5DG_Window.h"		// 框架文件
#include "Class.h"			// 类头文件
#include "Fmod.h"			// 音频库头文件
#include "Texture.h"		// 纹理导入头文件
//#include "5DG_Window.cpp"
 
//在此处加入程序要求的库到链接器中:
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")	
#pragma comment(lib, "fmodvc.lib")

//在此处定义WINDOWS全局变量:
GL_Window*	OGL_window;
Keys*		OGL_keys;

//在此处定义用户变量:
TextureTga	playertex[3];				// 纹理数组，保存纹理名字
TextureTga  computertex[3];
TextureTga	ammunitiontex[4];
TextureTga	awardtex[3];
TextureTga	othertex[4];

GLuint	base;						// 输出字体的显示列表号

bool start;							// 该值为真,表示游戏开始
bool startp;						// 辅助控制start,以免按住时不断轮换
bool end;							// 该值为真,表示游戏结束

bool keybombp;						// 控制炸弹发射用的布尔值,以保证按住不放时也只能发射一个炸弹
bool keyfirep;						// 控制子弹发射声音的布尔值,以保证不会产生叠音
int myPlaneNum;						// 玩家目前余下的飞机数目

DWORD lastTickCount=0;				// 运行到上一帧的时间点,用于求帧间隔时间
DWORD tickCount=0;					// 运行到当前帧的时间点,用于求帧间隔时间
int	 timer=0;						// 这一帧的运行时间
DWORD	endtime;					// 结束动画的起始时间

DWORD starttime;					// 用于控制片头动画的显示时间

FSOUND_SAMPLE *sound_1;				// 发射子弹的声音指针
FSOUND_SAMPLE *sound_2;				// 射击中的声音指针
FSOUND_SAMPLE *sound_3;				// 遇到物品的声音指针
FMUSIC_MODULE *sound_4;				// 游戏的背影音乐指针

float groudMove=0;					// 控制背影不断向下移动的变量
float startQuadOffset;				// 控制片头动画移动的变量

PlayerPlane myPlane;						// 玩家飞机
ComputerPlane computers[MAX_COMPUTER];		// 电脑飞机数组
Award	awards[MAX_AWARD];					// 物品数组
Ammunition ammunitions[MAX_AMMUNITION];		// 子弹数组

// 设置时钟
void setTimer()
{
	lastTickCount=tickCount;
	tickCount=GetTickCount();
	timer=tickCount-lastTickCount;	// 得到每帧的运行时间
}

// 游戏初始化
void GameInit(void)
{
	int i;
	for(i=0; i<MAX_AMMUNITION; i++){				// 清除全部子弹
		ammunitions[i].setLife(0);					// 将全部子弹无效
		ammunitions[i].setExplosible(false);		// 并且不让爆炸
	}
	for(i=0; i<MAX_AWARD; i++){						// 清除全部物品
		awards[i].setLife(0);						// 将全部物品无效
	}
	for (i=0; i<MAX_COMPUTER; i++)					// 对所有电脑飞机进行初始化设置
		computers[i].compinit();
	myPlane.initPlane(0,-230,100,2);				// 对玩家飞机初始化设置
	myPlaneNum=MAX_PLAYER;							// 设置玩家飞机个数
	starttime=0;									// 辅助控制游戏开始片头的播放时间
	startQuadOffset=0;								// 辅助控制游戏开始片头移动距离
}

// 声音初始化
GLvoid InitFMOD(GLvoid)
{
	if (FSOUND_Init(44100, 32, 0))					// 把声音初始化为44khz
	{
		sound_1=FSOUND_Sample_Load(5, "Data/fire.wav", FSOUND_LOOP_NORMAL, 0);
		sound_2=FSOUND_Sample_Load(FSOUND_FREE, "Data/hitTheTarget.wav", FSOUND_2D, 0);
		sound_3=FSOUND_Sample_Load(FSOUND_FREE, "Data/eat.wav", FSOUND_2D, 0);
		sound_4=FMUSIC_LoadSong("Data/bg.mid");
	}
}

// 声音释放
GLvoid FreeFMOD(GLvoid)
{
	if(sound_1) FSOUND_Sample_Free(sound_1);
	if(sound_2) FSOUND_Sample_Free(sound_2);
	if(sound_3) FSOUND_Sample_Free(sound_3);
	if(sound_4) FMUSIC_FreeSong(sound_4);
}

// 输出字体
void BuildFontGL(GLvoid)												// 建立位图字体（Bitmap Fonts）
{
	HFONT	newFont;													// 用以保存新的字体对象
	HFONT	oldFont;													// 用以保存原字体对象

	base = glGenLists(256);												// 存储256个字符

	newFont = CreateFont(	-18,										// 字体的高度
							0,											// 字体的宽度
							0,											// 旋转的角度
							0,											// 定位角度
							FW_THIN,									// 字体的粗细
							FALSE,										// 斜体?
							FALSE,										// 下划线?
							FALSE,										// 删除线?
							ANSI_CHARSET,								// 字符集
							OUT_TT_PRECIS,								// 输出精度
							CLIP_DEFAULT_PRECIS,						// 裁减精度
							ANTIALIASED_QUALITY,						// 输出质量
							FF_DONTCARE|DEFAULT_PITCH,					// 间距和字体族
							"Tahoma");									// 字体名称

	oldFont = (HFONT)SelectObject(OGL_window->hDC, newFont); 			// 选进设备描述表 
	wglUseFontBitmaps(OGL_window->hDC, 0, 256, base);					// 建立256个字符
	SelectObject(OGL_window->hDC, oldFont);								// 恢复设备描述表
	DeleteObject(newFont);												// 删除新字体
}

GLvoid KillFontGL(GLvoid)												// 删除保存字体的显示表
{
	glDeleteLists(base, 256);											// 删除256个字符
}

GLvoid glPrint(const char *fmt, ...)									// 建立Print函数
{
	char		text[256];												// 用以保存格式化后的字符串
	va_list		ap;														// 指向参数列表的指针

	if (fmt == NULL)													// 没有可输出的字符？
		return;															// 返回

	va_start(ap, fmt);													// 遍历字符串，查找变量
		vsprintf(text, fmt, ap);										// 将变量转换为显示的数字
	va_end(ap);															// 结果保存在text内

	glPushAttrib(GL_LIST_BIT);											// 显示表状态入栈
	glListBase(base);													// 显示表偏移量
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);				// 绘制字符串
	glPopAttrib();														// 显示表状态出栈
}

// 载入所需纹理
void LoadTexture(void)
{
	BuildTexture("Data/Playercenter.tga", &playertex[0]);
	BuildTexture("Data/Playerleft.tga", &playertex[1]);
	BuildTexture("Data/Playerright.tga", &playertex[2]);

	BuildTexture("Data/Computer1.tga", &computertex[0]);
	BuildTexture("Data/Computer2.tga", &computertex[1]);
	BuildTexture("Data/Computer3.tga", &computertex[2]);

	BuildTexture("Data/Award1.tga", &awardtex[0]);
	BuildTexture("Data/Award2.tga", &awardtex[1]);
	BuildTexture("Data/Award3.tga", &awardtex[2]);

	BuildTexture("Data/Ball.tga", &ammunitiontex[0]);
	BuildTexture("Data/Heavyball.tga", &ammunitiontex[1]);
	BuildTexture("Data/Missile.tga", &ammunitiontex[3]);

	BuildTexture("Data/Groud1.tga", &othertex[0]);
	BuildTexture("Data/Groud2.tga", &othertex[1]);
	BuildTexture("Data/Planeexplode.tga", &othertex[2]);
	BuildTexture("Data/Ballexplode.tga", &othertex[3]);
}

// OpenGL初始化
BOOL Initialize(GL_Window* window, Keys* keys)	
{
	// 设置全局变量
	OGL_window	= window;
	OGL_keys	= keys;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);								// 清屏为黑色
	glClearDepth(1.0f);													// 设置深度缓存
	glDepthFunc(GL_LEQUAL);												// 选择深度测试方式
	glEnable(GL_DEPTH_TEST);											// 开启深度测试
	glShadeModel(GL_SMOOTH);											// 阴暗处理采用平滑方式
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);					// 最精细的透视计算
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LoadTexture();														// 调用载入纹理函数
	BuildFontGL();														// 调用初始化字体函数
	InitFMOD();															// 调用初始化声音函数

	return TRUE;														// 初始化成功返回TRUE
}

// 游戏背景绘制
void Background(void)
{
	glEnable(GL_TEXTURE_2D);
	// 左右两个始终存在的方框
	glPushMatrix();
	glTranslatef(250.0f, 0.0f, 10.0f);
	glBindTexture(GL_TEXTURE_2D,othertex[0].texID);
	glBegin(GL_QUADS);
		glTexCoord2i(0,  0);glVertex2i(  0, -300);
		glTexCoord2i(4,  0);glVertex2i(150, -300);
		glTexCoord2i(4, 10);glVertex2i(150,  300);
		glTexCoord2i(0, 10);glVertex2i(  0,  300);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-250.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);glVertex2i(-150, -300);
		glTexCoord2i(4, 0);glVertex2i(   0, -300);
		glTexCoord2i(4,10);glVertex2i(   0,  300);
		glTexCoord2i(0,10);glVertex2i(-150,  300);
	glEnd();
	glPopMatrix();

	// 中央移动的星空背景
	glPushMatrix();
	glTranslatef(0.0f, 0.0f,-50.0f);
	glBindTexture(GL_TEXTURE_2D, othertex[1].texID);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f+groudMove);  glVertex2i(-250,-300);
		glTexCoord2f(2.0f, 0.0f+groudMove);  glVertex2i( 250,-300);
		glTexCoord2f(2.0f, 2.0f+groudMove);  glVertex2i( 250, 300);
		glTexCoord2f(0.0f, 2.0f+groudMove);  glVertex2i(-250, 300);
	glEnd();
	glPopMatrix();

	groudMove+=0.0001f*timer;
	glDisable(GL_TEXTURE_2D);
}

// 游戏未开始的场景绘制
void Opening(void)
{
	glRasterPos2f(-200.0f, 100.0f);
	glPrint("Press F5 to start a new game!");
	glRasterPos2f(-200.0f, 0.0f);
	glPrint(" Direction : W,S,A,D ");
	glRasterPos2f(-200.0f, -20.0f);
	glPrint(" Fire : J ; Missile: K");
	glRasterPos2f(-200.0f, -100.0f);
	int winscore=WIN_SCORE;
	glPrint("Mission : Earn %d Score! ", winscore);
}

// 开始游戏的主场景
void Running(void)
{
	int i;
	// 片头播放(播放时间4000ms)
	if(starttime<3000){
		starttime += timer;
		startp=true;						// 禁用F5
		if(starttime<2000)
			startQuadOffset += 0.16f*timer;
		else
			startQuadOffset -= 0.16f*timer;
		// 下面的移动方块
		glPushMatrix();
		glTranslatef(0.0f,-300.0f,-20.0f);
		glTranslatef(0.0f, startQuadOffset, 0.0f);
		glBindTexture(GL_TEXTURE_2D, othertex[0].texID);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);glVertex2i(-250,-320);
			glTexCoord2i(2, 0);glVertex2i( 250,-320);
			glTexCoord2i(2, 2);glVertex2i( 250,   0);
			glTexCoord2i(0, 2);glVertex2i(-250,   0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		// 上面的移动方块
		glPushMatrix();
		glTranslatef(0.0f, 300.0f,-20.0f);
		glTranslatef(0.0f,-startQuadOffset, 0.0f);
		glBindTexture(GL_TEXTURE_2D, othertex[0].texID);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0);glVertex2i(-250,   0);
			glTexCoord2i(2, 0);glVertex2i( 250,   0);
			glTexCoord2i(2, 2);glVertex2i( 250, 320);
			glTexCoord2i(0, 2);glVertex2i(-250, 320);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
		return;
	}
	startp=false;

	glEnable(GL_BLEND);				//	开启透明，将边角透明掉
	// 电脑的飞机处理
	for(i=0; i<MAX_COMPUTER; i++){
		computers[i].move();
		computers[i].fire();
		computers[i].draw();
		computers[i].blast();
		computers[i].update();
	}
	// 子弹处理
	for(i=0; i<MAX_AMMUNITION; i++){
		ammunitions[i].move ();
		ammunitions[i].draw ();
		ammunitions[i].blast();
	}
	// 物品处理
	for(i=0; i<MAX_AWARD; i++){
		awards[i].move();
		awards[i].draw();
		awards[i].eat();
	}

	// 玩家飞机处理
	// 飞机移动
	if(OGL_keys->keyDown['W']==TRUE)
		myPlane.moveUp();
	else if(OGL_keys->keyDown['S']==TRUE)
		myPlane.moveDown ();
	if(OGL_keys->keyDown['A']==TRUE)
		myPlane.moveLeft ();
	else if(OGL_keys->keyDown['D']==TRUE)
		myPlane.moveRight ();
	else
		myPlane.stay();
	// 飞机发射子弹
	if(OGL_keys->keyDown['J']==TRUE){
		myPlane.fire();
		if(!keyfirep){
			FSOUND_PlaySound(5,sound_1);
			keyfirep=true;
		}
	}
	if(OGL_keys->keyDown['J']==FALSE || myPlaneNum<=0){
		keyfirep=false;
		FSOUND_StopSound(5);	
	}
	// 飞机发射炸弹
	if(OGL_keys->keyDown['K']==TRUE && !keybombp){
		myPlane.fireBomb();
		keybombp=true;
	}
	if(OGL_keys->keyDown['K']==FALSE)
		keybombp=false;

	myPlane.draw();			// 飞机绘制
	myPlane.hitcomPlane();	// 飞机与敌机的碰撞检测
	myPlane.blast();		// 飞机的爆炸处理
	myPlane.update();		// 飞机的更新处理

	glDisable(GL_BLEND);	// 绘制完毕，关闭透明

	// 显示玩家分数
	glPushMatrix();
	glRasterPos3f(140.0f,-285.0f,10.0);
	glPrint("score:%.2f", (float)myPlane.getScore());
	glPopMatrix();

	// 显示玩家的飞机数
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(240.0f,-265.0f, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, playertex[0].texID);
	for(i=1; i<=myPlaneNum; i++){
		glTranslatef(-20.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex2i(-10,-10);
			glTexCoord2i(1, 0); glVertex2i( 10,-10);
			glTexCoord2i(1, 1); glVertex2i( 10, 10);
			glTexCoord2i(0, 1); glVertex2i(-10, 10);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	// 显示玩家持有的炸弹个数
	glPushMatrix();
	glTranslatef(-230.0f,-265.0f,0.0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ammunitiontex[3].texID);
	for(i=1; i<=myPlane.getBombNum(); i++){
		glTranslatef(20.0f, 0.0f, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2i(0, 0); glVertex2i(-10,-10);
			glTexCoord2i(1, 0); glVertex2i( 10,-10);
			glTexCoord2i(1, 1); glVertex2i( 10, 10);
			glTexCoord2i(0, 1); glVertex2i(-10, 10);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();

	// 显示玩家飞机的生命长度
	// 表示生命的矩形前的飞机图标
	glPushMatrix();
	glTranslatef(-240.0f,-290.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, playertex[0].texID);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(-10,-10);
		glTexCoord2i(1, 0); glVertex2i( 10,-10);
		glTexCoord2i(1, 1); glVertex2i( 10, 10);
		glTexCoord2i(0, 1); glVertex2i(-10, 10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
	// 矩形长度表示生命的长短
	glPushMatrix();
	glTranslatef(-230.0f,-290.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(0.0f,-5.0f);
		glVertex2f(myPlane.getLife(),-5.0f);
		glVertex2f(myPlane.getLife(), 5.0f);
		glVertex2f(0.0f, 5.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPopMatrix();
}

// 游戏结束函数
void Ending(void)
{
	FSOUND_StopSound(5);						// 停止播放枪声
	FMUSIC_StopSong(sound_4);
	keyfirep=false;								
	if(myPlane.getScore()<WIN_SCORE){			// 分数不够，游戏失败
		if(GetTickCount()-endtime<2000){		// 游戏结束后2s内显示
			startp=true;						// 禁用F5
			glRasterPos3f(-200.0f, 100.0f,-20.0f);
			glPrint("Final Score:  %d!", myPlane.getScore());
			glRasterPos3f(-200.0f,-100.0f,-20.0f);
			glPrint("You Lost!! Try Again!!");
		}else{
			start=false;
			startp=false;
			end=false;
		}
	}else{										// 分数达到，游戏胜利
		if(GetTickCount()-endtime<2000){		// 游戏结束后2s内显示
			startp=true;						// 禁用F5
			glRasterPos3f(-200.0f, 100.0f,-20.0f);
			glPrint("Congratulations!!");
			glRasterPos3f(-200.0f,-100.0f,-20.0f);
			glPrint("YOU WIN!!");
		}else{
			start=false;
			startp=false;
			end=false;
		}
	}
}

// 绘制的主场景
void DrawSceneGL(void)
{
	setTimer();			// 取得每帧的运行时间
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	

	Background();		// 绘制背景

	if(!start){			// 游戏尚未开始
		Opening();		// 调用游戏还未开始显示的内容函数
	}else{				
		if(!end){		// 游戏开始且末结束
			Running();	// 调用游戏运行的主函数
		}else{			// 游戏运行结束
			Ending();	// 调用游戏结束的处理函数
		}
	}

	glFlush();
}

// 更新对消息的动作
void Update(void)
{
	if (OGL_keys->keyDown[VK_ESCAPE] == TRUE)						// 判断ESC键是否按下
	{
		TerminateApplication (OGL_window);							// 结束程序
	}
	if (OGL_keys->keyDown[VK_F1] == TRUE)							// 判断F1是否按下
	{
		PostMessage (OGL_window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);	// 在全屏/窗口模式间切换
	}

	// 对F5按键的处理，用于控制游戏开始
	if(OGL_keys->keyDown[VK_F5] == TRUE&&!startp){

		start=!start;
		startp=true;
		if(start){
			FMUSIC_PlaySong(sound_4);
			GameInit();											// 调用游戏初始化设置函数
		}else{
			FMUSIC_StopSong(sound_4);
		}
	}
	if(OGL_keys->keyDown[VK_F5] == FALSE)
		startp=false;
}

// 退出前扫尾工作
void Deinitialize(void)
{
	KillFontGL();
	FreeFMOD();
}