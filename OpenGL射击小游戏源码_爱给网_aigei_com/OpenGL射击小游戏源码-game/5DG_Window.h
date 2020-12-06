#ifndef _5DG_WINDOW_H
#define _5DG_WINDOW_H

#include <windows.h>													// Windows的头文件
#include <gl\gl.h>														// OpenGL32库的头文件
#include <gl\glu.h>														// Glu32库的头文件

#define WM_TOGGLEFULLSCREEN (WM_USER + 1)								// 定义全屏/窗口模式切换消息
#define TITLE		"5DG's FireGame"									// 定义窗口标题
#define CLASSNAME	"5DG_OPENGL"										// 定义窗口类名
#define	WIDTH		800													// 定义窗口宽度
#define HEIGHT		600													// 定义窗口高度
#define BPP			16													// 定义每象素的位数

// 在此处定义与窗口相关的结构体:
typedef struct {														// 定义处理键盘的结构体
	BOOL keyDown [256];													// 存储键盘按键状态的数组
} Keys;

typedef struct {														// 定义存储应用程序实例的结构体
	HINSTANCE		hInstance;											// 应用程序实例
	const char*		className;											// 应用程序类名
} Application;

typedef struct {														// 定义初始窗口所需信息
	Application*	application;										// 所属的应用程序
	char*			title;												// 窗口标题
	int				width;												// 窗口宽度
	int				height;												// 窗口高度
	int				bitsPerPixel;										// 每像素的位数
	BOOL			isFullScreen;										// 是否全屏
} GL_WindowInit;

typedef struct {														// 定义窗口结构体
	Keys*			keys;												// 窗口的键盘
	HWND			hWnd;												// 窗口句柄
	HDC				hDC;												// 设备描述表
	HGLRC			hRC;												// 绘制描述表
	GL_WindowInit	init;												// Window初始化信息的结构体
	BOOL			isVisible;											// 窗口是否处于活动态
} GL_Window;

static BOOL g_isProgramLooping;
static BOOL g_createFullScreen;

// 此代码模块中包含的函数的前向声明:
void TerminateApplication (GL_Window* window);							// 结束程序
BOOL Initialize(GL_Window* window, Keys* keys);							// 设置你绘制前的初始化值
void DrawSceneGL(void);													// 在这里完成场景的绘制
void Update(void);														// 在此处更新对消息的动作
void Deinitialize(void);												// 在此处做退出前扫尾工作

#endif																	// _5DG_WINDOW_H