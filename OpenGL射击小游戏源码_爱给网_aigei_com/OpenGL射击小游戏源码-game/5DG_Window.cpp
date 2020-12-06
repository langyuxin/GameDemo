#include "5DG_Window.h"


void TerminateApplication(GL_Window* window)							// 结束程序
{
	PostMessage(window->hWnd, WM_QUIT, 0, 0);							// 发送WM_QUIT消息
	g_isProgramLooping = FALSE;											// 停止程序
} 

void ResizeWindowGL(int width, int height)								// 重新设置窗口大小
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));				// 重置当前视口大小
	glMatrixMode(GL_PROJECTION);										// 切换到投影矩阵模式
	glLoadIdentity();													// 重置投影矩阵
	glOrtho(-(float)width/2,(float)width/2,-(float)height/2,(float)height/2,-100,100);			// 设置透视投影
	glMatrixMode(GL_MODELVIEW);											// 切换到模型视图矩阵
	glLoadIdentity();													// 重置模型视图矩阵
}

BOOL ChangeScreenResolution(int width, int height, int bitsPerPixel)	// 修改屏幕分辨率
{
	DEVMODE dmScreenSettings;											// 设备设置模式 
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						// 清空
	dmScreenSettings.dmSize			= sizeof(DEVMODE);					// Devmode结构的大小
	dmScreenSettings.dmPelsWidth	= width;							// 设置为屏幕宽度
	dmScreenSettings.dmPelsHeight	= height;							// 设置为屏幕高度
	dmScreenSettings.dmBitsPerPel	= bitsPerPixel;						// 设为指定位长
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态栏
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return FALSE;													// 分辨率修改失败,返回 FALSE
	}
	return TRUE;														// 分辨率修改成功,返回 TRUE
}

BOOL CreateWindowGL(GL_Window* window)									// 建立OpenGL窗口
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// 设置窗口样式
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// 设置窗口扩展样式

	PIXELFORMATDESCRIPTOR pfd =											// 像素格式描述符(pfd)的设置
	{
		sizeof(PIXELFORMATDESCRIPTOR),									// 像素的尺寸
		1,																// 版本号
		PFD_DRAW_TO_WINDOW |											// pfd必须支持窗口绘制
		PFD_SUPPORT_OPENGL |											// pfd必须支持OpenGL
		PFD_DOUBLEBUFFER,												// pfd必须支持双缓存
		PFD_TYPE_RGBA,													// 像素格式为RGBA
		window->init.bitsPerPixel,										// 设置颜色深度
		0, 0, 0, 0, 0, 0,												// 忽略颜色位数
		0,																// 无Alpha缓存
		0,																// 忽略偏移位
		0,																// 无累积缓存
		0, 0, 0, 0,														// 忽略累积缓存位 
		16,																// 深度缓存为16位 
		0,																// 无模板缓存
		0,																// 无辅助缓存
		PFD_MAIN_PLANE,													// 主要绘制层
		0,																// 保留位
		0, 0, 0															// 忽略层遮罩
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// 定义窗口大小

	GLuint PixelFormat;													// 保存像素格式

	if (window->init.isFullScreen == TRUE)								// 切换全屏
	{
		if (ChangeScreenResolution(window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			// 全屏切换失败
			MessageBox(HWND_DESKTOP, "无法切换到全屏模式,在窗口模式下运行.\nMode Switch Failed,Running In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// 设置 isFullscreen 为FALSE
		}
		else															// 全屏切换成功
		{
			ShowCursor (FALSE);											// 隐藏鼠标
			windowStyle = WS_POPUP;										// 设置窗口样式
			windowExtendedStyle |= WS_EX_TOPMOST;						// 设置窗口扩展样式
		}
	}
	else
	{
		// 调整窗口大小,包括窗口边界
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// 开始创建 OpenGL 窗口
	window->hWnd = CreateWindowEx(windowExtendedStyle,					// 窗口扩展样式
									window->init.application->className,// 应用程序类名
									window->init.title,					// 窗口标题
									windowStyle,						// 窗口样式
									0, 0,								// 窗口的 X,Y 坐标位置
									windowRect.right - windowRect.left,	// 窗口宽度
									windowRect.bottom - windowRect.top,	// 窗口高度
									HWND_DESKTOP,						// 父窗口为桌面
									0,									// 无菜单
									window->init.application->hInstance,// 传入应用程序实例 
									window);

	if (window->hWnd == 0)												// 窗口是否成功创建
	{
		return FALSE;													// 若失败,则返回FALSE
	}

	window->hDC = GetDC(window->hWnd);									// 取得当前窗口的设备描述表
	if (window->hDC == 0)												// 若未得到设备描述表
	{
		DestroyWindow(window->hWnd);									// 销毁该窗口
		window->hWnd = 0;												// 窗口句柄清零
		return FALSE;													// 返回FALSE
	}

	PixelFormat = ChoosePixelFormat(window->hDC, &pfd);					// 选择兼容的像素格式
	if (PixelFormat == 0)												// 若选择失败
	{
		ReleaseDC(window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 将设备描述表清零
		DestroyWindow(window->hWnd);									// 销毁窗口
		window->hWnd = 0;												// 窗口句柄清零
		return FALSE;													// 返回FALSE
	}

	if (SetPixelFormat(window->hDC, PixelFormat, &pfd) == FALSE)		// 设置像素格式并判断是否失败
	{
		ReleaseDC(window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 将设备描述表清零
		DestroyWindow(window->hWnd);									// 销毁窗口
		window->hWnd = 0;												// 窗口句柄清零
		return FALSE;													// 返回FALSE
	}

	window->hRC = wglCreateContext(window->hDC);						// 取得绘制描述表
	if (window->hRC == 0)												// 若未得到绘制描述表
	{
		ReleaseDC(window->hWnd, window->hDC);							// 释放设备描述表 
		window->hDC = 0;												// 将设备描述表清零
		DestroyWindow(window->hWnd);									// 销毁窗口
		window->hWnd = 0;												// 窗口句柄清零
		return FALSE;													// 返回FALSE
	}

	if (wglMakeCurrent(window->hDC, window->hRC) == FALSE)				// 设置绘制描述表并判断是否失败
	{
		wglDeleteContext(window->hRC);									// 删除绘制描述表
		window->hRC = 0;												// 将绘制描述表清零
		ReleaseDC(window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 将设备描述表清零
		DestroyWindow(window->hWnd);									// 销毁窗口
		window->hWnd = 0;												// 窗口句柄清零
		return FALSE;													// 返回FALSE
	}

	ShowWindow(window->hWnd, SW_NORMAL);								// 显示窗口
	ResizeWindowGL(window->init.width, window->init.height);			// 重设窗口
	ZeroMemory(window->keys, sizeof(Keys));								// 清空键盘缓冲区

	return TRUE;														// 窗口创建成功
}

BOOL DestroyWindowGL(GL_Window* window)									// 销毁窗口并释放程序所用的资源
{
	if (window->hWnd != 0)												// 窗口有句柄?
	{	
		if (window->hDC != 0)											// 窗口是否有得到绘制描述表?
		{
			wglMakeCurrent(window->hDC, 0);								// 将当前描述表指针置为0
			if (window->hRC != 0)										// 该窗口是否有绘制描述表
			{
				wglDeleteContext(window->hRC);							// 释放绘制描述表
				window->hRC = 0;										// 将绘制描述表清零
			}
			ReleaseDC(window->hWnd, window->hDC);						// 释放设备描述表
			window->hDC = 0;											// 将设备描述表清零
		}
		DestroyWindow(window->hWnd);									// 销毁窗口
		window->hWnd = 0;												// 将窗口句柄清零
	}

	if (window->init.isFullScreen)										// 若窗口在全屏模式下
	{
		ChangeDisplaySettings(NULL, 0);									// 切换为桌面分辨率
		ShowCursor(TRUE);												// 显示鼠标
	}	
	return TRUE;														// 返回TRUE
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	GL_Window* window = (GL_Window*)(GetWindowLong(hWnd, GWL_USERDATA));

	switch (uMsg)														// 取得Window的消息值
	{
	case WM_SYSCOMMAND:													// 截取系统命令
		{
			switch (wParam)												// 监听系统调用
			{
			case SC_SCREENSAVE:											// 要运行屏保(Screensaver)?
			case SC_MONITORPOWER:										// 显示器进入节电模式(Powersave)?
				return 0;												// 提前返回0,防止系统调用执行
			}
			break;														// 退出
		}

	case WM_CREATE:														// 创建Window
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// 保存窗口结构指针
			window = (GL_Window*)(creation->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, (LONG)(window));			// 改变窗口属性
			return 0;													// 返回
		}
	
	case WM_CLOSE:														// 关闭窗口
		{
			PostMessage(window->hWnd, WM_QUIT, 0, 0);					// 结束程序
			g_isProgramLooping = FALSE;
			return 0;
		}

	case WM_SIZE:														// WM_SIZE消息处理
		switch (wParam)													// 判断消息句柄
		{
		case SIZE_MINIMIZED:											// 窗口最小化?
			window->isVisible = FALSE;									// 设置isVisible 为 FALSE
			return 0;

		case SIZE_MAXIMIZED:											// 窗口最大化?
			window->isVisible = TRUE;									// 设置isVisible 为 TRUE
			ResizeWindowGL (LOWORD (lParam), HIWORD (lParam));			// 修改窗口大小为 Width=LoWord, Height=HiWord
			return 0;													// 返回

		case SIZE_RESTORED:												// 窗口还原?
			window->isVisible = TRUE;									// 设置isVisible 为 TRUE
			ResizeWindowGL (LOWORD (lParam), HIWORD (lParam));			// 修改窗口大小为 Width=LoWord, Height=HiWord
			return 0;													// 返回
		}
		break;															// 退出

	case WM_KEYDOWN:													// 更新键盘缓冲区
		if ((wParam >= 0) && (wParam <= 255))							// 按键是否合法?
		{
			window->keys->keyDown[wParam] = TRUE;						// 设相应键为 TRUE (被按下的键)
			return 0;													// 返回
		}
		break;															// 退出

	case WM_KEYUP:														// 松开按键的时候更新键盘缓冲
		if ((wParam >= 0) && (wParam <= 255))							// 按键是否合法?
		{
			window->keys->keyDown[wParam] = FALSE;						// 设相应键为 FALSE (被释放的键)
			return 0;													// 返回
		}
		break;															// 退出

	case WM_TOGGLEFULLSCREEN:											// 切换全屏/窗口模式
		{
			g_createFullScreen = !g_createFullScreen;
			PostMessage(hWnd, WM_QUIT, 0, 0);
			break;														// 退出
		}

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					// 将本程序不处理的消息传给 DefWindowProc 
}

BOOL RegisterWindowClass(Application* application)						// 为本应用程序注册一个类.
{	
	WNDCLASSEX windowClass;												// 窗口类
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// 清空内存
	windowClass.cbSize		= sizeof(WNDCLASSEX);						// 窗口类的大小
	windowClass.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// 在移动,改变大小的时候重绘 
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// 用WindowProc函数处理消息
	windowClass.hInstance		= application->hInstance;				// 设置实例
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// 类背景的画刷颜色
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 载入箭头光标
	windowClass.lpszClassName	= application->className;				// 设置应用程序的类名
	if (RegisterClassEx(&windowClass) == 0)								// 注册类失败?
	{
		MessageBox(HWND_DESKTOP, "应用程序类注册失败!\nRegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;													// 返回FALSE (注册失败)
	}
	return TRUE;														// 返回TRUE	(注册成功)
}

// 应用程序的入口 (WinMain)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application			application;									// 应用程序
	GL_Window			window;											// 窗口
	Keys				keys;											// 键盘按键
	MSG					msg;											// Window消息
	BOOL				isMessagePumpActive;

	// 给应用程序赋值
	application.className = CLASSNAME;									// 程序类名字
	application.hInstance = hInstance;									// 程序入口

	// 窗口相关信息设置
	ZeroMemory(&window, sizeof(GL_Window));								// 清零窗口变量的内存空间
	window.keys					= &keys;								// 设置按键
	window.init.application		= &application;							// 设置窗口程序
	window.init.title			= TITLE;								// 设置标题
	window.init.width			= WIDTH;								// 设置窗口宽度
	window.init.height			= HEIGHT;								// 设置窗口高度
	window.init.bitsPerPixel	= BPP;									// 设置每像素的位数
	window.init.isFullScreen	= FALSE;								// 设置初始窗口是否全屏否(FALSE)
	ZeroMemory(&keys, sizeof(Keys));									// 键盘缓冲清零

	if (RegisterWindowClass(&application) == FALSE)						// 注册类是否失败
	{
		MessageBox(HWND_DESKTOP, "窗口类注册失败!\nError Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// 结束程序
	}

	g_isProgramLooping = TRUE;											// 将g_isProgramLooping设TRUE
	g_createFullScreen = window.init.isFullScreen;						// g_createFullScreen 设为默认值
	while (g_isProgramLooping)											// 循环直到程序停止
	{
		// 建立窗口
		window.init.isFullScreen = g_createFullScreen;					// 传递是否全屏信息
		if (CreateWindowGL(&window) == TRUE)							// 创建GL窗口成功否?
		{
			if (Initialize(&window, &keys) == FALSE)					// 初始化
			{
				// 失败
				PostMessage(window.hWnd, WM_QUIT, 0, 0);				// 抛出消息WM_QUIT
				g_isProgramLooping = FALSE;
			}
			else
			{	// 初始化成功
				isMessagePumpActive = TRUE;								// 设isMessagePumpActive为TRUE
				while (isMessagePumpActive == TRUE)						// 当isMessagePumpActive为TRUE时
				{
					// 成功建立窗口，监测Window消息
					if (PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// 检测WM_QUIT消息
						if (msg.message != WM_QUIT)						// WM_QUIT消息?
						{
							DispatchMessage(&msg);						// 如果不是,分派消息
						}
						else											// 否则(消息是WM_QUIT)
						{
							isMessagePumpActive = FALSE;				// 结束 Message Pump
						}
					}
					else												// 如果没有消息
					{
						if (window.isVisible == FALSE)					// 如果窗口不可见
						{
							WaitMessage();								// 等待消息
						}
						else											// 如果窗口可见
						{
							Update();									// 更新处理消息事件
							DrawSceneGL();								// 绘制场景
							SwapBuffers(window.hDC);					// 交换缓存
						}
					}
				}														// 当isMessagePumpActive 为TRUE时循环执行
			}

			// 程序结束
			Deinitialize();												// 做扫尾工作
			DestroyWindowGL(&window);									// 销毁窗口
		}
		else															// 如果窗口创建失败
		{
			MessageBox(HWND_DESKTOP, "创建OpenGL窗口失败\nError Creating OpenGL Window", "Error", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									// 结束循环
		}
	}																	
	UnregisterClass(application.className, application.hInstance);		// 注销窗口类
	return 0;
}