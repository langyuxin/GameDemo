////�ڴ˴�����WINDOWͷ�ļ�
#include <windows.h>		// Windows ͷ�ļ�
#include <stdio.h>

//�ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�:
#include <gl\gl.h>			// OpenGL32���ͷ�ļ�
#include <gl\glu.h>			// Glu32���ͷ�ļ�

//�ڴ˴����ñ������ͷ�ļ�
#include "5DG_Window.h"		// ����ļ�
#include "Class.h"			// ��ͷ�ļ�
#include "Fmod.h"			// ��Ƶ��ͷ�ļ�
#include "Texture.h"		// ������ͷ�ļ�
//#include "5DG_Window.cpp"
 
//�ڴ˴��������Ҫ��Ŀ⵽��������:
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")	
#pragma comment(lib, "fmodvc.lib")

//�ڴ˴�����WINDOWSȫ�ֱ���:
GL_Window*	OGL_window;
Keys*		OGL_keys;

//�ڴ˴������û�����:
TextureTga	playertex[3];				// �������飬������������
TextureTga  computertex[3];
TextureTga	ammunitiontex[4];
TextureTga	awardtex[3];
TextureTga	othertex[4];

GLuint	base;						// ����������ʾ�б��

bool start;							// ��ֵΪ��,��ʾ��Ϸ��ʼ
bool startp;						// ��������start,���ⰴסʱ�����ֻ�
bool end;							// ��ֵΪ��,��ʾ��Ϸ����

bool keybombp;						// ����ը�������õĲ���ֵ,�Ա�֤��ס����ʱҲֻ�ܷ���һ��ը��
bool keyfirep;						// �����ӵ����������Ĳ���ֵ,�Ա�֤�����������
int myPlaneNum;						// ���Ŀǰ���µķɻ���Ŀ

DWORD lastTickCount=0;				// ���е���һ֡��ʱ���,������֡���ʱ��
DWORD tickCount=0;					// ���е���ǰ֡��ʱ���,������֡���ʱ��
int	 timer=0;						// ��һ֡������ʱ��
DWORD	endtime;					// ������������ʼʱ��

DWORD starttime;					// ���ڿ���Ƭͷ��������ʾʱ��

FSOUND_SAMPLE *sound_1;				// �����ӵ�������ָ��
FSOUND_SAMPLE *sound_2;				// ����е�����ָ��
FSOUND_SAMPLE *sound_3;				// ������Ʒ������ָ��
FMUSIC_MODULE *sound_4;				// ��Ϸ�ı�Ӱ����ָ��

float groudMove=0;					// ���Ʊ�Ӱ���������ƶ��ı���
float startQuadOffset;				// ����Ƭͷ�����ƶ��ı���

PlayerPlane myPlane;						// ��ҷɻ�
ComputerPlane computers[MAX_COMPUTER];		// ���Էɻ�����
Award	awards[MAX_AWARD];					// ��Ʒ����
Ammunition ammunitions[MAX_AMMUNITION];		// �ӵ�����

// ����ʱ��
void setTimer()
{
	lastTickCount=tickCount;
	tickCount=GetTickCount();
	timer=tickCount-lastTickCount;	// �õ�ÿ֡������ʱ��
}

// ��Ϸ��ʼ��
void GameInit(void)
{
	int i;
	for(i=0; i<MAX_AMMUNITION; i++){				// ���ȫ���ӵ�
		ammunitions[i].setLife(0);					// ��ȫ���ӵ���Ч
		ammunitions[i].setExplosible(false);		// ���Ҳ��ñ�ը
	}
	for(i=0; i<MAX_AWARD; i++){						// ���ȫ����Ʒ
		awards[i].setLife(0);						// ��ȫ����Ʒ��Ч
	}
	for (i=0; i<MAX_COMPUTER; i++)					// �����е��Էɻ����г�ʼ������
		computers[i].compinit();
	myPlane.initPlane(0,-230,100,2);				// ����ҷɻ���ʼ������
	myPlaneNum=MAX_PLAYER;							// ������ҷɻ�����
	starttime=0;									// ����������Ϸ��ʼƬͷ�Ĳ���ʱ��
	startQuadOffset=0;								// ����������Ϸ��ʼƬͷ�ƶ�����
}

// ������ʼ��
GLvoid InitFMOD(GLvoid)
{
	if (FSOUND_Init(44100, 32, 0))					// ��������ʼ��Ϊ44khz
	{
		sound_1=FSOUND_Sample_Load(5, "Data/fire.wav", FSOUND_LOOP_NORMAL, 0);
		sound_2=FSOUND_Sample_Load(FSOUND_FREE, "Data/hitTheTarget.wav", FSOUND_2D, 0);
		sound_3=FSOUND_Sample_Load(FSOUND_FREE, "Data/eat.wav", FSOUND_2D, 0);
		sound_4=FMUSIC_LoadSong("Data/bg.mid");
	}
}

// �����ͷ�
GLvoid FreeFMOD(GLvoid)
{
	if(sound_1) FSOUND_Sample_Free(sound_1);
	if(sound_2) FSOUND_Sample_Free(sound_2);
	if(sound_3) FSOUND_Sample_Free(sound_3);
	if(sound_4) FMUSIC_FreeSong(sound_4);
}

// �������
void BuildFontGL(GLvoid)												// ����λͼ���壨Bitmap Fonts��
{
	HFONT	newFont;													// ���Ա����µ��������
	HFONT	oldFont;													// ���Ա���ԭ�������

	base = glGenLists(256);												// �洢256���ַ�

	newFont = CreateFont(	-18,										// ����ĸ߶�
							0,											// ����Ŀ��
							0,											// ��ת�ĽǶ�
							0,											// ��λ�Ƕ�
							FW_THIN,									// ����Ĵ�ϸ
							FALSE,										// б��?
							FALSE,										// �»���?
							FALSE,										// ɾ����?
							ANSI_CHARSET,								// �ַ���
							OUT_TT_PRECIS,								// �������
							CLIP_DEFAULT_PRECIS,						// �ü�����
							ANTIALIASED_QUALITY,						// �������
							FF_DONTCARE|DEFAULT_PITCH,					// ����������
							"Tahoma");									// ��������

	oldFont = (HFONT)SelectObject(OGL_window->hDC, newFont); 			// ѡ���豸������ 
	wglUseFontBitmaps(OGL_window->hDC, 0, 256, base);					// ����256���ַ�
	SelectObject(OGL_window->hDC, oldFont);								// �ָ��豸������
	DeleteObject(newFont);												// ɾ��������
}

GLvoid KillFontGL(GLvoid)												// ɾ�������������ʾ��
{
	glDeleteLists(base, 256);											// ɾ��256���ַ�
}

GLvoid glPrint(const char *fmt, ...)									// ����Print����
{
	char		text[256];												// ���Ա����ʽ������ַ���
	va_list		ap;														// ָ������б��ָ��

	if (fmt == NULL)													// û�п�������ַ���
		return;															// ����

	va_start(ap, fmt);													// �����ַ��������ұ���
		vsprintf(text, fmt, ap);										// ������ת��Ϊ��ʾ������
	va_end(ap);															// ���������text��

	glPushAttrib(GL_LIST_BIT);											// ��ʾ��״̬��ջ
	glListBase(base);													// ��ʾ��ƫ����
	glCallLists((int)strlen(text), GL_UNSIGNED_BYTE, text);				// �����ַ���
	glPopAttrib();														// ��ʾ��״̬��ջ
}

// ������������
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

// OpenGL��ʼ��
BOOL Initialize(GL_Window* window, Keys* keys)	
{
	// ����ȫ�ֱ���
	OGL_window	= window;
	OGL_keys	= keys;

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);								// ����Ϊ��ɫ
	glClearDepth(1.0f);													// ������Ȼ���
	glDepthFunc(GL_LEQUAL);												// ѡ����Ȳ��Է�ʽ
	glEnable(GL_DEPTH_TEST);											// ������Ȳ���
	glShadeModel(GL_SMOOTH);											// �����������ƽ����ʽ
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);					// �ϸ��͸�Ӽ���
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	LoadTexture();														// ��������������
	BuildFontGL();														// ���ó�ʼ�����庯��
	InitFMOD();															// ���ó�ʼ����������

	return TRUE;														// ��ʼ���ɹ�����TRUE
}

// ��Ϸ��������
void Background(void)
{
	glEnable(GL_TEXTURE_2D);
	// ��������ʼ�մ��ڵķ���
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

	// �����ƶ����ǿձ���
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

// ��Ϸδ��ʼ�ĳ�������
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

// ��ʼ��Ϸ��������
void Running(void)
{
	int i;
	// Ƭͷ����(����ʱ��4000ms)
	if(starttime<3000){
		starttime += timer;
		startp=true;						// ����F5
		if(starttime<2000)
			startQuadOffset += 0.16f*timer;
		else
			startQuadOffset -= 0.16f*timer;
		// ������ƶ�����
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
		// ������ƶ�����
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

	glEnable(GL_BLEND);				//	����͸�������߽�͸����
	// ���Եķɻ�����
	for(i=0; i<MAX_COMPUTER; i++){
		computers[i].move();
		computers[i].fire();
		computers[i].draw();
		computers[i].blast();
		computers[i].update();
	}
	// �ӵ�����
	for(i=0; i<MAX_AMMUNITION; i++){
		ammunitions[i].move ();
		ammunitions[i].draw ();
		ammunitions[i].blast();
	}
	// ��Ʒ����
	for(i=0; i<MAX_AWARD; i++){
		awards[i].move();
		awards[i].draw();
		awards[i].eat();
	}

	// ��ҷɻ�����
	// �ɻ��ƶ�
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
	// �ɻ������ӵ�
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
	// �ɻ�����ը��
	if(OGL_keys->keyDown['K']==TRUE && !keybombp){
		myPlane.fireBomb();
		keybombp=true;
	}
	if(OGL_keys->keyDown['K']==FALSE)
		keybombp=false;

	myPlane.draw();			// �ɻ�����
	myPlane.hitcomPlane();	// �ɻ���л�����ײ���
	myPlane.blast();		// �ɻ��ı�ը����
	myPlane.update();		// �ɻ��ĸ��´���

	glDisable(GL_BLEND);	// ������ϣ��ر�͸��

	// ��ʾ��ҷ���
	glPushMatrix();
	glRasterPos3f(140.0f,-285.0f,10.0);
	glPrint("score:%.2f", (float)myPlane.getScore());
	glPopMatrix();

	// ��ʾ��ҵķɻ���
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

	// ��ʾ��ҳ��е�ը������
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

	// ��ʾ��ҷɻ�����������
	// ��ʾ�����ľ���ǰ�ķɻ�ͼ��
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
	// ���γ��ȱ�ʾ�����ĳ���
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

// ��Ϸ��������
void Ending(void)
{
	FSOUND_StopSound(5);						// ֹͣ����ǹ��
	FMUSIC_StopSong(sound_4);
	keyfirep=false;								
	if(myPlane.getScore()<WIN_SCORE){			// ������������Ϸʧ��
		if(GetTickCount()-endtime<2000){		// ��Ϸ������2s����ʾ
			startp=true;						// ����F5
			glRasterPos3f(-200.0f, 100.0f,-20.0f);
			glPrint("Final Score:  %d!", myPlane.getScore());
			glRasterPos3f(-200.0f,-100.0f,-20.0f);
			glPrint("You Lost!! Try Again!!");
		}else{
			start=false;
			startp=false;
			end=false;
		}
	}else{										// �����ﵽ����Ϸʤ��
		if(GetTickCount()-endtime<2000){		// ��Ϸ������2s����ʾ
			startp=true;						// ����F5
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

// ���Ƶ�������
void DrawSceneGL(void)
{
	setTimer();			// ȡ��ÿ֡������ʱ��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	

	Background();		// ���Ʊ���

	if(!start){			// ��Ϸ��δ��ʼ
		Opening();		// ������Ϸ��δ��ʼ��ʾ�����ݺ���
	}else{				
		if(!end){		// ��Ϸ��ʼ��ĩ����
			Running();	// ������Ϸ���е�������
		}else{			// ��Ϸ���н���
			Ending();	// ������Ϸ�����Ĵ�����
		}
	}

	glFlush();
}

// ���¶���Ϣ�Ķ���
void Update(void)
{
	if (OGL_keys->keyDown[VK_ESCAPE] == TRUE)						// �ж�ESC���Ƿ���
	{
		TerminateApplication (OGL_window);							// ��������
	}
	if (OGL_keys->keyDown[VK_F1] == TRUE)							// �ж�F1�Ƿ���
	{
		PostMessage (OGL_window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);	// ��ȫ��/����ģʽ���л�
	}

	// ��F5�����Ĵ������ڿ�����Ϸ��ʼ
	if(OGL_keys->keyDown[VK_F5] == TRUE&&!startp){

		start=!start;
		startp=true;
		if(start){
			FMUSIC_PlaySong(sound_4);
			GameInit();											// ������Ϸ��ʼ�����ú���
		}else{
			FMUSIC_StopSong(sound_4);
		}
	}
	if(OGL_keys->keyDown[VK_F5] == FALSE)
		startp=false;
}

// �˳�ǰɨβ����
void Deinitialize(void)
{
	KillFontGL();
	FreeFMOD();
}