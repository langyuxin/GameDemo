/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <afx.h>
#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library
#include "stdafx.h"
#include "FreeImage.h"
#include "GameManager.h"
#include <time.h>

#include "fmod.h"			// 音频库头文件

#define WINDOW_WIDTH = 800
#define WINDOW_HEIGHT = 600

#pragma comment(lib,"FreeImaged.lib")
#pragma comment(lib, "fmodvc.lib")

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default
//HBITMAP		g_hBitmap = NULL;
bool	keyStart = false;	//游戏是否开始
bool	end=false;		//end来判断游戏结束与否;

float	camera=-5;

GLuint	texture[12];			// 保存我方角色纹理
GLuint	attack[2];
GLuint	move[4];
GLuint	die;
GLuint	still[2];
GLuint	jump[4];
GLuint	texturee[12];			//保存敌方角色纹理

GLuint	attacke[2];
GLuint	movee[4];
GLuint	diee;
GLuint	stille[2];

GLuint	base;						// 输出字体的显示列表号
GLuint	textureBg[5];			//背景加载

FSOUND_SAMPLE *sound_2;			//攻击	
FSOUND_STREAM *sound_1;
FSOUND_SAMPLE *sound_3;			//行走
FSOUND_SAMPLE *sound_4;			
FSOUND_SAMPLE *sound_5;



int	enemynum=1,barriesnum=2;



Player p1=Player(-7,-1.8,0.001);	//玩家角色
Enemy e1=Enemy(0,-1.8,0.001);			//
Enemy es[100];					//NPC角色数组
GameManager g1;					//碰撞检测对象

rectangle	barries[5];



LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
void InitGame();	//游戏初始化
int DrawGLScene(GLvoid);//游戏主体函数
void Ending();

GLuint CreateTexture(CString filename )					//创建纹理
{

     GLuint      texture;     
	
	FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename,0);
    FIBITMAP*dib = FreeImage_Load(fifmt,filename, 0);
	BYTE *bits = new BYTE[FreeImage_GetWidth(dib)*FreeImage_GetHeight(dib)*4];
    BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

	int count=0;
    for(int pix=0; pix<(FreeImage_GetWidth(dib)*FreeImage_GetHeight(dib))*3; pix+=3)
     {
		bits[count] = pixels[pix+2];
        bits[count + 1] = pixels[pix+1];
        bits[count + 2] = pixels[pix];
		if(pixels[pix]==255 &&	pixels[pix+1]==255 && pixels[pix+2]==255)
			bits[count + 3] = 0;
		else bits[count + 3] = 255;
		count +=4;
	}
	glGenTextures(1,&texture);                  //generate texture object
    glBindTexture(GL_TEXTURE_2D, texture);       // enable ourtexture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    //generate the texture image
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,FreeImage_GetWidth(dib),FreeImage_GetHeight(dib), 0, GL_RGBA,GL_UNSIGNED_BYTE, bits);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, FreeImage_GetWidth(dib),FreeImage_GetHeight(dib), GL_RGBA, GL_UNSIGNED_BYTE, bits);
	FreeImage_Unload(dib);
	delete bits;

	return texture;


}

void LoadGLTextures()
{

		texture[0] = CreateTexture("Texture\\Court.bmp");
		attack[1]=texture[1] = CreateTexture("Texture1\\attackr.bmp");
		attack[0]=texture[2] = CreateTexture("Texture1\\attackl.bmp");
		move[2]=texture[3] = CreateTexture("Texture1\\walkl.bmp");
		move[3]=texture[4] = CreateTexture("Texture1\\walkr.bmp");
		move[0]=texture[5] = CreateTexture("Texture\\walku.bmp");
		move[1]=texture[6] = CreateTexture("Texture\\walkd.bmp");
		still[1]=texture[7] = CreateTexture("Texture1\\stillr.bmp");
		still[0]=texture[8] = CreateTexture("Texture1\\stilll.bmp");
		die=texture[10] = CreateTexture("Texture\\die.bmp");
		jump[1] = CreateTexture("Texture1\\jumpr.bmp");
		jump[0] = CreateTexture("Texture1\\jumpl.bmp");
		jump[3] = CreateTexture("Texture1\\jumpattackr.bmp");
		jump[2] = CreateTexture("Texture1\\jumpattackl.bmp");


		texturee[0] = CreateTexture("Texture\\enemy_sl.bmp");
		texturee[1] = CreateTexture("Texture\\enemy_wl.bmp");
		texturee[2] = CreateTexture("Texture\\enemy_al.bmp");
		texturee[3] = CreateTexture("Texture\\enemy_die.bmp");

		attacke[1]=texture[1] = CreateTexture("Texture1\\enemyattackr.bmp");
		attacke[0]=texture[2] = CreateTexture("Texture1\\enemyattackl.bmp");
		movee[2]=texture[3] = CreateTexture("Texture1\\enemywalkl.bmp");
		movee[3]=texture[4] = CreateTexture("Texture1\\enemywalkr.bmp");
		stille[1]=texture[7] = CreateTexture("Texture1\\enemystillr.bmp");
		stille[0]=texture[8] = CreateTexture("Texture1\\enemystilll.bmp");

		textureBg[0] = CreateTexture("Texture1\\1.bmp");
		textureBg[1] = CreateTexture("Texture\\fail.bmp");
		textureBg[2] = CreateTexture("Texture\\victory.bmp");
		textureBg[3] = CreateTexture("Texture1\\scene2.bmp");
}
///音乐初始化
GLvoid InitFMOD(GLvoid){
	if (FSOUND_Init(44100, 32, 0))					// 把声音初始化为44khz
		{
			//sound_1=FMUSIC_LoadSong("Music/bg.mod");
			sound_1=FSOUND_Stream_OpenFile("Music/bg.mp3",0,  0);
			sound_2=FSOUND_Sample_Load(FSOUND_FREE, "Music/pingA2.wav", FSOUND_2D, 0);
			sound_3=FSOUND_Sample_Load(FSOUND_FREE, "Music/move.wav", FSOUND_2D, 0);
		}
}
// 输出字体
void BuildFontGL(GLvoid)												// 建立位图字体（Bitmap Fonts）
{
	HFONT	newFont;													// 用以保存新的字体对象
	HFONT	oldFont;													// 用以保存原字体对象

	base = glGenLists(256);												// 存储256个字符

	newFont = CreateFont(	-45,										// 字体的高度
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
							"Georgia");									// 字体名称

	oldFont = (HFONT)SelectObject(wglGetCurrentDC(), newFont); 			// 选进设备描述表 
	wglUseFontBitmaps(wglGetCurrentDC(), 0, 256, base);					// 建立256个字符
	SelectObject(wglGetCurrentDC(), oldFont);								// 恢复设备描述表
	DeleteObject(newFont);												// 删除新字体
}

GLvoid KillFontGL(GLvoid)												// 删除保存字体的显示表
{
	glDeleteLists(base, 256);											// 删除256个字符
}
GLvoid glPrint(const char *pstr)									// 建立Print函数
{
	char		text[256];												// 用以保存格式化后的字符串
	va_list		ap;														// 指向参数列表的指针

	if (pstr == NULL)													// 没有可输出的字符？
		return;															// 返回

	va_start(ap, pstr);													// 遍历字符串，查找变量
		vsprintf(text, pstr, ap);										// 将变量转换为显示的数字
	va_end(ap);														// 结果保存在text内

	glPushAttrib(GL_LIST_BIT);											// 显示表状态入栈
	glListBase(base -0);
	glCallLists(strlen(pstr), GL_UNSIGNED_BYTE, pstr);	// 调用显示列表绘制字符串
	glPopAttrib();				// 弹出属性堆栈
									
}

// 声音释放
GLvoid FreeFMOD(GLvoid)
{
	//if(sound_1) FMUSIC_FreeSong(sound_1);
	if(sound_1) FSOUND_Stream_Close(sound_1);
	if(sound_2) FSOUND_Sample_Free(sound_2);
	if(sound_3) FSOUND_Sample_Free(sound_3);
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{

	LoadGLTextures();								// Jump To Texture Loading Routine
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
//	glEnable(GL_BLEND);
		
	InitGame();
	return TRUE;										// Initialization Went OK
}
void SetWorld()						//加载游戏地图
{			
	if(camera<p1.getX() &&camera<5) camera=p1.getX();
	gluLookAt(camera, 0.0f, 5.0f, camera, 0.0f, 0.0f, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, textureBg[3]);
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0,1.0);glVertex3f(-8.0f, 2.2f, 0.0f);					// Top Left
		glTexCoord2f(0.2,1.0);glVertex3f( -4.0f, 2.2f, 0.0f);					// Top Right
		glTexCoord2f(0.2,0.0);glVertex3f( -4.0f,-2.2f, 0.0f);					// Bottom Right
		glTexCoord2f(0.0,0.0);glVertex3f(-8.0f,-2.2f, 0.0f);					// Bottom Left
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureBg[3]);
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0,1.0);glVertex3f(-4.0f, 2.2f, 0.0f);					// Top Left
		glTexCoord2f(1.0,1.0);glVertex3f( 4.0f, 2.2f, 0.0f);					// Top Right
		glTexCoord2f(1.0,0.0);glVertex3f( 4.0f,-2.2f, 0.0f);					// Bottom Right
		glTexCoord2f(0.0,0.0);glVertex3f(-4.0f,-2.2f, 0.0f);					// Bottom Left
	glEnd();
	glBindTexture(GL_TEXTURE_2D, textureBg[3]);
	glBegin(GL_QUADS);									// Draw A Quad
		glTexCoord2f(0.0,1.0);glVertex3f( 8.0f, 2.2f, 0.0f);					// Top Left
		glTexCoord2f(0.2,1.0);glVertex3f( 4.0f, 2.2f, 0.0f);					// Top Right
		glTexCoord2f(0.2,0.0);glVertex3f( 4.0f,-2.2f, 0.0f);					// Bottom Right
		glTexCoord2f(0.0,0.0);glVertex3f( 8.0f,-2.2f, 0.0f);					// Bottom Left
	glEnd();
	glPrint("Active OpenGL Text With NeHe ");  // 输出文字到屏幕
}
void InitGame()							//游戏初始化
{
	barries[0].InitialRec(-1.2,-1.8,0.001,0.5,0.5);
	barries[1].InitialRec(0.8,-1.8,0.001,0.5,0.5);
//	barries[2].InitialRec(1.2,-1.8,0.001,0.5,0.5);
	p1.setattack(attack);
	p1.setmove(move);
	p1.setstill(still);
	p1.setdie(die);	
	p1.setjump(jump);

	e1.setattack(attacke);
	e1.setmove(movee);
	e1.setstill(stille);
	e1.setdie(diee);

	for(int i=0;i<5;i++)
	{
		es[i].Initial(-5+i*2,-1.8,0.001);
		es[i].setattack(attacke);
		es[i].setmove(movee);
		es[i].setstill(stille);
		es[i].setdie(diee);
		enemynum++;
	}
	es[3].setstate(3);
	es[2].setstate(3);
	InitFMOD();
	BuildFontGL();
	
	end=true;
}
void Start()							//判断是否开始游戏
{
	if(!keyStart)						//开始界面加载
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();	// Reset The Current Modelview Matrixf
		gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);
			//glTranslatef(0.0f, 0.0f, -1.0f);
			//glColor3f(1.0f, 1.0f, 0.0f); // 颜色
			glRasterPos2f(-0.4f, 0.30f); // 输出位置

			glPrint("Active OpenGL Text With NeHe ");  // 输出文字到屏幕
			glRasterPos3f(-200.0f, 100.0f,-20.0f);
			glPrint("Congratulations!!");

		glBindTexture(GL_TEXTURE_2D, textureBg[0]);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0,1.0);glVertex3f(-4.0f, 2.2f, 0.0f);					// Top Left
			glTexCoord2f(1.0,1.0);glVertex3f( 4.0f, 2.2f, 0.0f);					// Top Right
			glTexCoord2f(1.0,0.0);glVertex3f( 4.0f,-2.2f, 0.0f);					// Bottom Right
			glTexCoord2f(0.0,0.0);glVertex3f(-4.0f,-2.2f, 0.0f);					// Bottom Left
		glEnd();
	}
	else
	{
		if(end)
		{
			DrawGLScene();
			FSOUND_Stream_Play (0,sound_1);			//游戏开始并播放背景音乐
		}
		else
		{
			Ending();
		//	FSOUND_Stream_Stop (0,sound_1);			//背景音乐停止
		}
	}

}
void Running()
{
		if(keys['J'])					//攻击
		{
			p1.setattackstate(1);
			if(p1.getstate()!=4)
			{	
				p1.setstate(1);
				if(p1.getfps()!=0 &&p1.getstate()!=1) p1.setfps(0);
				FSOUND_PlaySound(5,sound_2);
			}
		}
		if(keys['W'] )	//向上走
		{
			p1.setmovedirect(0);
			if(p1.getstate()==0)
			{
				p1.setstate(2);
			}
		}
		if(keys['S']  )	//向下走
		{
			p1.setmovedirect(1);	
			if(p1.getstate()==0)
			{
				p1.setstate(2);
			}
		}
		if(keys['D'] )	//向右走
		{
			p1.setmovedirect(3);
			if(p1.getstate()==0)
			{
				p1.setstate(2);
			}
			if(p1.getstate()!=1&&p1.getstate()!=3)
			{
				p1.setFace(1);
			}
		//	FSOUND_PlaySound(5,sound_3);
		}
		if(keys['A'] )	//向左走
		{
			p1.setmovedirect(2);
			if(p1.getstate()==0)
			{
				p1.setstate(2);
			}
			if(p1.getstate()!=1&&p1.getstate()!=3)
			{
				p1.setFace(-1);
			}
	//		FSOUND_PlaySound(5,sound_3);
		}
		if(keys['K'])
		{
			if(p1.getfps()!=0 &&p1.getstate()!=4) p1.setfps(0);
			p1.setstate(4);	
			FSOUND_PlaySound(5,sound_3);
		}
		
		if(keys[' '])
		{
			keyStart = true;	
		}
}

void Ending()					//游戏结束
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();	// Reset The Current Modelview Matrix
	gluLookAt(0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);
	if(p1.getChances()>0)
	{
		glBindTexture(GL_TEXTURE_2D, textureBg[2]);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0,1.0);glVertex3f(-4.0f, 2.2f, 0.0f);					// Top Left
			glTexCoord2f(1.0,1.0);glVertex3f( 4.0f, 2.2f, 0.0f);					// Top Right
			glTexCoord2f(1.0,0.0);glVertex3f( 4.0f,-2.2f, 0.0f);					// Bottom Right
			glTexCoord2f(0.0,0.0);glVertex3f(-4.0f,-2.2f, 0.0f);					// Bottom Left
		glEnd();
	}
	if(p1.getChances()==0)
	{
		glBindTexture(GL_TEXTURE_2D, textureBg[1]);
		glBegin(GL_QUADS);									// Draw A Quad
			glTexCoord2f(0.0,1.0);glVertex3f(-4.0f, 2.2f, 0.0f);					// Top Left
			glTexCoord2f(1.0,1.0);glVertex3f( 4.0f, 2.2f, 0.0f);					// Top Right
			glTexCoord2f(1.0,0.0);glVertex3f( 4.0f,-2.2f, 0.0f);					// Bottom Right
			glTexCoord2f(0.0,0.0);glVertex3f(-4.0f,-2.2f, 0.0f);					// Bottom Left
		glEnd();
	}
		
}

void Update()					//敌我双方角色的动作更新
{
	int a,flag=0;
	for(int i=0;i<barriesnum;i++)							//人物与障碍物的碰撞
	{
		a=g1.BarriesCollision(p1,barries[i]);
		if(a==0) 
		{
			p1.setX(barries[i].getX()-p_weight);
			flag=1;
		}
		if(a==3)
		{
			p1.setX(barries[i].getX()+p_weight);
			flag=1;
		}

		if(a==1)
		{
			p1.setY(barries[i].getY()+barries[i].getheight());
			flag=1;
		}
		if(a==2)   flag=1;

	}
	if(flag==0 && p1.getY()!=-1.8f &&p1.getstate()!=4) 
	{
		if(p1.getX()<-5 || (p1.getX()+p_weight)>-4)
		p1.setY(-1.8);
	}
	if(p1.getX()>=-5 && (p1.getX()+p_weight)<-4 && p1.getstate()!=4)				//河流等无地面的地方
	{
		p1.setY(p1.getY()-0.1f);
		if(p1.getY()<-2.2)	
		{
			p1.setChances(p1.getChances()-1);
			if(p1.getChances()==0)  end=0;
		}
	}
	p1.act();
	if(e1.getstate()!=3) e1.act();
	for( i=0;i<5;i++)									
	{
		if(es[i].getstate()==3) continue;
		es[i].act();
		if( g1.HumanCollision(p1,es[i]) && p1.getattackstate()==1)
		{
			enemynum--;
			es[i].setstate(3);
			if(enemynum ==2)  end=0;		//敌人数为0，游戏结束
		}
	}
	if( g1.HumanCollision(p1,e1) && p1.getattackstate()==1 && e1.getstate()!=3)
	{
		enemynum--;
		e1.setstate(3);
		if(enemynum ==2)  end=0;		//敌人数为0，游戏结束
	}
	if(g1.HumanCollision(p1,e1) && e1.getattackstate()==1) 
	{
		p1.setChances(p1.getChances()-1);
		if(p1.getChances()==0) end=0;     //玩家命数为0，游戏结束
	}
	
}
int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();	// Reset The Current Modelview Matrix
	
	SetWorld();			//加载游戏地图
	Update();			//游戏中变化更新
	return TRUE;										// Keep Going

}

GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window				*
 *	width			- Width Of The GL Window Or Fullscreen Mode				*
 *	height			- Height Of The GL Window Or Fullscreen Mode			*
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"OpenGL",							// Class Name
								title,								// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hInstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}
			break;
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done=FALSE;								// Bool Variable To Exit Loop
	srand((int)time(NULL));

	// Ask The User Which Screen Mode They Prefer
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow("NeHe's First Polygon Tutorial",640,480,16,fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if ( keys[VK_ESCAPE])	// Active?  Was There A Quit Received?
			{
				done=TRUE;							// ESC or DrawGLScene Signalled A Quit
			}
			else									// Not Time To Quit, Update Screen
			{
				Start();
				Running();
				SwapBuffers(hDC);	// Swap Buffers (Double Buffering)
				
			}
//&& !DrawGLScene()
			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1]=FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen=!fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow("NeHe's First Polygon Tutorial",640,480,16,fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
