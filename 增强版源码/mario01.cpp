// mario01.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#include "bitmaptool.h"
#include "texttool.h"
#include "gamemap.h"
#include "myclock.h"

#define MAX_LOADSTRING 100

char *pPreText[]={
	"",
	"操作:   Z:子弹   X:跳   方向键移动  W:默认窗口大小",
	"x",
	"地图文件错误，请修正错误后重新启动程序。",
	"(上下键选择菜单,回车键确认)",
	"开始游戏",
	"操作说明",
	"(回车键返回主菜单)",
	""
};

int mapani[2][10]={
{32,32,64,32,32,52,64,32,64,32},
{32,32,64,32,32,25,64,32,64,32},
};

int mapsolid[2][13]={
{32,32,32,32,32,32,32,32,32,64,64,20,100},
{32,32,32,32,32,32,32,32,32,64,64,10,12}
};

int mapanibk[2][4]={
{96,96,96,96},
{64,64,64,64},
};

int mapanimagic[2][1]={
{192},
{128}
};

//所有地图信息
struct MAPINFO allmapinfo[]={
{1,3,66,7,0,5},
{2,4,25,4,1,5},
{MAX_MATCH,-1,-1,-1,2,5},
{-1,0,3,8,3,1},
{-1,1,3,8,3,2}
};

//普通蘑菇兵
struct ROLE gl_enemy_normal=
{
	0,
	0,
	32,
	32,
	ID_ANI_ENEMY_NORMAL,
	0,
	2,
	0,
	0,
	-ENEMY_STEP_X,//speed
	1,
	1
};

//跟踪打印
//FILEREPORT f1;

MYCLOCK c1;

GAMEMAP gamemap;

MYBITMAP bmPre;
MYBKSKY bmSky;

MYANIOBJ bmMap;
MYANIOBJ bmMapBkObj;
MYANIOBJ bmAniObj;
MYROLE rmain;

MYANIMAGIC bmMagic;

MYFONT myfont;

HDC hwindow,hscreen,hmem,hmem2;
HBITMAP hmapnull;

int wwin,hwin;
int wwingame,hwingame;

HWND hWndMain;

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MARIO01, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MARIO01);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MARIO01);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
//	wcex.lpszMenuName	= (LPCSTR)IDC_MARIO01;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	
	hInst = hInstance; // Store instance handle in our global variable

	wwingame=GAMEW*32;
	hwingame=GAMEH*32;

	wwin=GetSystemMetrics(SM_CXSCREEN);
	hwin=GetSystemMetrics(SM_CYSCREEN);
	
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		(wwin-wwingame)/2, (hwin-hwingame)/2, 
		wwingame, hwingame+32, NULL, NULL, hInstance, NULL);
	
	if (!hWnd)
	{
		return FALSE;
	}
	
	hWndMain=hWnd;

	//dc
	hwindow=GetDC(hWnd);
	hscreen=CreateCompatibleDC(hwindow);
	hmem=CreateCompatibleDC(hwindow);
	hmem2=CreateCompatibleDC(hwindow);//用于魔法特效的临时DC
	
	//用空位图初始化各个DC
	hmapnull=CreateCompatibleBitmap(hwindow,GAMEW*32*5,GAMEH*32);
	SelectObject(hscreen,hmapnull);
	SelectObject(hmem,hmapnull);
	SelectObject(hmem2,hmapnull);
	
	ReleaseDC(hWnd, hwindow);
	ShowWindow(hWnd, nCmdShow);
	//pic
	bmPre.Init(hInstance,IDB_BITMAP_PRE1,1,5);
	bmPre.SetDevice(hscreen,hmem,GAMEW*32,GAMEH*32);
	bmPre.SetPos(BM_USER,0,0);
	
	bmSky.Init(hInstance,IDB_BITMAP_MAP_SKY,1,4);
	bmSky.SetDevice(hscreen,hmem,GAMEW*32*MAX_PAGE,GAMEH*32);
	bmSky.SetPos(BM_USER,0,0);
	
	bmMap.Init(hInstance,IDB_BITMAP_MAP,1,1);
	bmMap.SetDevice(hscreen,hmem,GAMEW*32*MAX_PAGE,GAMEH*32);
	bmMap.InitAniList(mapsolid[0],mapsolid[1], sizeof(mapsolid[0])/sizeof(int),0);
	
	bmMapBkObj.Init(hInstance,IDB_BITMAP_MAP_BK,1,1);
	bmMapBkObj.SetDevice(hscreen,hmem,GAMEW*32*MAX_PAGE,GAMEH*32);
	bmMapBkObj.InitAniList(mapanibk[0],mapanibk[1],sizeof(mapanibk[0])/sizeof(int),1);
	
	bmAniObj.Init(hInstance,IDB_BITMAP_ANI,1,1);
	bmAniObj.SetDevice(hscreen,hmem,GAMEW*32*MAX_PAGE,GAMEH*32);
	bmAniObj.InitAniList(mapani[0],mapani[1],sizeof(mapani[0])/sizeof(int),1);
	
	bmMagic.Init(hInstance,IDB_BITMAP_MAGIC,1,1);
	bmMagic.SetDevice(hscreen,hmem, hmem2);
	bmMagic.InitAniList(mapanimagic[0],mapanimagic[1],sizeof(mapanimagic[0])/sizeof(int));
	
	rmain.Init(hInstance,IDB_BITMAP_ROLE,5,1);
	rmain.SetDevice(hscreen,hmem,GAMEW*32*MAX_PAGE,GAMEH*32);

	//text
	myfont.SetDevice(hscreen);
	
	//map
	gamemap.Init();
	rmain.SetPos(BM_USER,3*32,8*32);
	rmain.InitRole(0,GAMEW*32*MAX_PAGE-32);
	//文件检查
	if(!gamemap.LoadMap())
	{
		gamemap.CodeErr(ERR_MAP_FILE);
	}
	
	c1.SetDevice(hscreen);
	c1.Begin(hWnd, GAME_TIME_CLIP ,-1);

	
	UpdateWindow(hWnd);
	
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			hwindow = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			//初始化空图
			SelectObject(hscreen,hmapnull);

			switch(gamemap.iGameState)
			{
			case GAME_ERR:
				gamemap.viewx=0;
				//显示错误信息
				bmPre.Stretch(2,2,0);
				myfont.SelectColor(TC_WHITE,TC_BLACK);
				myfont.SelectFont(0);
				myfont.ShowText(150,290,pPreText[3]);
				break;

			case GAME_PRE:
				gamemap.viewx=0;
				bmPre.Stretch(2,2,0);
				
				myfont.SelectFont(0);
				myfont.SelectColor(TC_BLACK, TC_YELLOW_0);
				myfont.ShowText(150,260,pPreText[4]);
				myfont.ShowText(150,290,pPreText[5]);
				myfont.ShowText(150,320,pPreText[6]);
				gamemap.ShowMenu(bmAniObj);
				break;

			case GAME_HELP:
				gamemap.viewx=0;
				bmPre.Stretch(2,2,4);

				myfont.SelectFont(0);
				myfont.SelectColor(TC_BLACK, TC_YELLOW_0);
				myfont.ShowText(150,230,pPreText[8]);
				myfont.ShowText(50,260,pPreText[1]);
				myfont.ShowText(50,290,pPreText[7]);
				break;

			case GAME_IN_PRE:
				gamemap.viewx=0;
				bmPre.Stretch(2,2,2);	
				gamemap.ShowInfo(hscreen);
				break;

			case GAME_IN:
			case GAME_WIN:			
			case GAME_FAIL_WAIT:
			case GAME_PUMP_IN:
				bmSky.DrawRollStretch(2,2,gamemap.mapinfo.iBackBmp);
				gamemap.ShowBkObj(bmMapBkObj);
				gamemap.Show(bmMap);
				gamemap.ShowAniObj(bmAniObj);
				gamemap.ShowOther(hscreen);
				rmain.Draw();
				break;

			case GAME_OVER:
				gamemap.viewx=0;
				//输出图片GAME OVER
				bmPre.Stretch(2,2,1);	
				break;
					
			case GAME_PASS:
				gamemap.viewx=0;
				//输出图片GAME OVER
				bmPre.Stretch(2,2,3);	
				break;
			}
			if(gamemap.iScreenScale)
			{
				StretchBlt(hwindow,0,0,
					wwingame,hwingame,
					hscreen,
					gamemap.viewx,0,
					GAMEW*32,GAMEH*32,
					SRCCOPY);	
			}
			else
			{
				BitBlt(hwindow,0,0,GAMEW*32,GAMEH*32,hscreen,gamemap.viewx,0,SRCCOPY);
			}

			EndPaint(hWnd, &ps);
			break;

		case WM_TIMER:
			switch(gamemap.iGameState)
			{
			case GAME_PRE:
				c1.DecCount();
				if(0 == c1.iNum%MENU_ARROW_TIME)
				{
					InvalidateRect(hWnd,NULL,false);
				}				
				break;

			case GAME_IN_PRE:
				if(c1.DecCount())
				{
					gamemap.SetGameState(GAME_IN);
					c1.ReStart(TIME_GAME_IN);					
				}
				InvalidateRect(hWnd,NULL,false);
				break;
				
			case GAME_IN:
			case GAME_WIN:
				c1.DecCount();
				if(0 == c1.iNum%SKY_TIME)
				{
					bmSky.MoveRoll(SKY_SPEED);//云彩移动
				}
				gamemap.ChangeFrame(c1.iNum);//帧控制
				rmain.Move();//人物移动
				gamemap.MoveView();//视图移动
				gamemap.CheckRole();//角色检测
				gamemap.CheckAni(c1.iNum);//逻辑数据检测
				gamemap.IsWin();
				InvalidateRect(hWnd,NULL,false);
				break;

			case GAME_WIN_WAIT:
				if(c1.DecCount())
				{
					gamemap.SetGameState(GAME_IN_PRE);					
					InvalidateRect(hWnd,NULL,false);
				}
				break;

			case GAME_PUMP_IN:
				if(c1.DecCount())
				{
					gamemap.ChangeMap();
					gamemap.SetGameState(GAME_IN);	
					c1.ReStart(TIME_GAME_IN);					
					rmain.SetAni(ROLE_ANI_UP);
				}
				InvalidateRect(hWnd,NULL,false);
				break;

			case GAME_FAIL_WAIT:
				if(c1.DecCount())
				{
					gamemap.Fail_Wait();
				}
				break;

			case GAME_PASS:
				if(c1.DecCount())
				{
					gamemap.SetGameState(GAME_PRE);					
				}
				InvalidateRect(hWnd,NULL,false);
				break;
					
			case GAME_OVER:
				if(c1.DecCount())
				{
					gamemap.SetGameState(GAME_PRE);					
				}
				InvalidateRect(hWnd,NULL,false);
				break;
			}
			break;

		case WM_KEYDOWN:
			if(gamemap.KeyProc(wParam))	
				InvalidateRect(hWnd,NULL,false);
			break;

		case WM_KEYUP:
			gamemap.KeyUpProc(wParam);						
			break;

		case WM_SIZE:
			wwingame=LOWORD(lParam);
			hwingame=HIWORD(lParam);
			
			if( wwingame <= GAMEW*32 || hwingame <= GAMEH*32)
			{
				wwingame = GAMEW*32;
				hwingame = GAMEH*32;
				gamemap.iScreenScale = 0;				
			}
			else
			{
				if(wwingame*3 > hwingame*4)
				{
					//宽高不成比例
					wwingame = hwingame*4/3;
				}
				else
				{
					hwingame = wwingame*3/4;
				}
				gamemap.iScreenScale =1;
			}
			break;			

		case WM_DESTROY:
			c1.Destroy();
			DeleteObject((HBITMAP)hmapnull);
			DeleteDC(hwindow);
			DeleteDC(hscreen);
			DeleteDC(hmem);

			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

