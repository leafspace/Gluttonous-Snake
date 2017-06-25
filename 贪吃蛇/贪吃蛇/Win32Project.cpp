#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "OpenInit.h"
#include "resource.h"
#include "SnakeRun.h"
#include <mmsystem.h>                           //用来支持playsound来播放音乐  
#pragma comment(lib, "winmm.lib")               //定义playsound的库
#define MAX_LOADSTRING 100                      //用来设置串的最大长度

HINSTANCE hInst;                                //入口
HWND hMainWnd;
WCHAR szTitle[MAX_LOADSTRING];                  //标题栏
WCHAR szWindowClass[MAX_LOADSTRING];            //窗口类名
BOOL isCheck = 0;			                    //选中标志

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	                  _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPWSTR    lpCmdLine,
	                  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);                                      //告诉编译器，这些变量已经使用，不用警告
	UNREFERENCED_PARAMETER(lpCmdLine);                                          //告诉编译器，这些变量已经使用，不用警告
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);             //从资源内加载名字到变量szTitle中
	LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);    //从资源内加载名字到变量szWindowClass中
	MyRegisterClass(hInstance);                                                 //注册窗口

	if (!InitInstance(hInstance, nCmdShow))                                     //初始化窗口
	{
		MessageBox(NULL, TEXT("Failed to initialize !"), TEXT("Error"), 0);
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU));//初始化菜单
	MSG Msg;
	PlaySound(TEXT("葫芦小金刚.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (GetMessage(&Msg, nullptr, 0, 0))                                     //获取信息
	{
		if (!TranslateAccelerator(Msg.hwnd, hAccelTable, &Msg))                 //解释信息
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return (int)Msg.wParam;
}

//定义窗口内容
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDR_MENU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT));

	return RegisterClassExW(&wcex);
}

//初始化内容
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int nScreenWidth, nScreenHeight;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);                               //获取电脑屏幕宽度
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);                              //获取电脑屏幕高度

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU|WS_VISIBLE, //创建主窗口
		(nScreenWidth - 590) / 2, (nScreenHeight - 385) / 2, 590, 385, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)                                                                  //如果未创建成功
	{
		MessageBox(NULL, TEXT("Failed to create window !"), TEXT("Error"), 0);
		return FALSE;
	}
	hMainWnd = hWnd;                                                            //保存当前父窗口
	hInst = hInstance;                                                          //保存当前示例

	ShowWindow(hWnd, nCmdShow);                                                 //弹出窗口
	Backgroundpaste(hWnd, IDB_BITMAP_OPENBACKGROUND, 0, 0);                           //为窗口添加背景
	UpdateWindow(hWnd);                                                         //更新窗口信息
	return TRUE;
}

//动作选择菜单
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (Msg)
	{
	case WM_CREATE:
		BackgroundButton(hWnd, lParam);
		return 0;
	case WM_PAINT:
		Backgroundpaste(hWnd, IDB_BITMAP_OPENBACKGROUND, 0, 0);
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_RBUTTONDOWN:                                               //鼠标在窗口中右键被按下时，弹出浮动式菜单
		if (DoDispPopupMenu(hWnd, LOWORD(lParam), HIWORD(lParam)))
			return 0;                                                  //函数调用成功
		else
			break;                                                     //函数调用失败
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MENU1_START:
			DialogBox(hInst, MAKEINTRESOURCE(ROLE_SELECT),             //弹出选择窗口
				hWnd, RoleDlgSlet);
			break;
		case ID_MENU1_OPERAT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP_FORM),           //弹出帮助窗口
				hWnd, HelpDlgProc);
			break;
		case ID_MENU1_EXIT:
			PostQuitMessage(0);                                        //发送退出信息
			break;
		case ID_MENU2_SPINBOX:
			DoCheckMenuItem(hWnd);                                     //在菜单选项前打钩，并按照前提情况做出弹控制框选择
			break;
		case ID_MENU2_ABOUTPRU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MORE_PRODUCER),       //弹出制作者框信息
				hWnd, MoreDlgProd);
			break;
		case ID_MENU2_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(ID_MENU2_ABOUT),          //弹出关于程序框
				hWnd, About); 
			break;
		case OPENBUTTON1:
			DialogBox(hInst, MAKEINTRESOURCE(ROLE_SELECT),             //弹出选择窗口
				hWnd, RoleDlgSlet);
			break;
		case OPENBUTTON2:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP_FORM),           //弹出帮助窗口
				hWnd, HelpDlgProc);
			break;
		case OPENBUTTON3:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MORE_PRODUCER),       //弹出制作者框信息
				hWnd, MoreDlgProd);
			break;
		}
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);                                            //发送退出信息
		return 0;
	}
	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK HelpDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_OK:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK MoreDlgProd(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;
		Backgroundpaste(hDlg, IDB_BITMAP_PRODUBACKGROUND, 0, 0);
		hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		break;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case BLOGBUTTON:
			ShellExecute(NULL, TEXT("open"), TEXT("http://user.qzone.qq.com/1141610886/"), NULL, NULL, SW_SHOW);
		}
			
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK RoleDlgSlet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case SINGLE_PLAY:
			EndDialog(hDlg, LOWORD(wParam));
			SingleGame(hInst);			
			break;
		case TWO_PLAY:
			EndDialog(hDlg, LOWORD(wParam));
			TwoGame(hInst);
			break;
		default:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}