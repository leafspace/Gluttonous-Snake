#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "OpenInit.h"
#include "resource.h"
#include "SnakeRun.h"
#include <mmsystem.h>                           //����֧��playsound����������  
#pragma comment(lib, "winmm.lib")               //����playsound�Ŀ�
#define MAX_LOADSTRING 100                      //�������ô�����󳤶�

HINSTANCE hInst;                                //���
HWND hMainWnd;
WCHAR szTitle[MAX_LOADSTRING];                  //������
WCHAR szWindowClass[MAX_LOADSTRING];            //��������
BOOL isCheck = 0;			                    //ѡ�б�־

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	                  _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPWSTR    lpCmdLine,
	                  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);                                      //���߱���������Щ�����Ѿ�ʹ�ã����þ���
	UNREFERENCED_PARAMETER(lpCmdLine);                                          //���߱���������Щ�����Ѿ�ʹ�ã����þ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);             //����Դ�ڼ������ֵ�����szTitle��
	LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);    //����Դ�ڼ������ֵ�����szWindowClass��
	MyRegisterClass(hInstance);                                                 //ע�ᴰ��

	if (!InitInstance(hInstance, nCmdShow))                                     //��ʼ������
	{
		MessageBox(NULL, TEXT("Failed to initialize !"), TEXT("Error"), 0);
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU));//��ʼ���˵�
	MSG Msg;
	PlaySound(TEXT("��«С���.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (GetMessage(&Msg, nullptr, 0, 0))                                     //��ȡ��Ϣ
	{
		if (!TranslateAccelerator(Msg.hwnd, hAccelTable, &Msg))                 //������Ϣ
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
	return (int)Msg.wParam;
}

//���崰������
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

//��ʼ������
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int nScreenWidth, nScreenHeight;
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);                               //��ȡ������Ļ���
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);                              //��ȡ������Ļ�߶�

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU|WS_VISIBLE, //����������
		(nScreenWidth - 590) / 2, (nScreenHeight - 385) / 2, 590, 385, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)                                                                  //���δ�����ɹ�
	{
		MessageBox(NULL, TEXT("Failed to create window !"), TEXT("Error"), 0);
		return FALSE;
	}
	hMainWnd = hWnd;                                                            //���浱ǰ������
	hInst = hInstance;                                                          //���浱ǰʾ��

	ShowWindow(hWnd, nCmdShow);                                                 //��������
	Backgroundpaste(hWnd, IDB_BITMAP_OPENBACKGROUND, 0, 0);                           //Ϊ������ӱ���
	UpdateWindow(hWnd);                                                         //���´�����Ϣ
	return TRUE;
}

//����ѡ��˵�
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
	case WM_RBUTTONDOWN:                                               //����ڴ������Ҽ�������ʱ����������ʽ�˵�
		if (DoDispPopupMenu(hWnd, LOWORD(lParam), HIWORD(lParam)))
			return 0;                                                  //�������óɹ�
		else
			break;                                                     //��������ʧ��
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_MENU1_START:
			DialogBox(hInst, MAKEINTRESOURCE(ROLE_SELECT),             //����ѡ�񴰿�
				hWnd, RoleDlgSlet);
			break;
		case ID_MENU1_OPERAT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP_FORM),           //������������
				hWnd, HelpDlgProc);
			break;
		case ID_MENU1_EXIT:
			PostQuitMessage(0);                                        //�����˳���Ϣ
			break;
		case ID_MENU2_SPINBOX:
			DoCheckMenuItem(hWnd);                                     //�ڲ˵�ѡ��ǰ�򹳣�������ǰ��������������ƿ�ѡ��
			break;
		case ID_MENU2_ABOUTPRU:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MORE_PRODUCER),       //���������߿���Ϣ
				hWnd, MoreDlgProd);
			break;
		case ID_MENU2_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(ID_MENU2_ABOUT),          //�������ڳ����
				hWnd, About); 
			break;
		case OPENBUTTON1:
			DialogBox(hInst, MAKEINTRESOURCE(ROLE_SELECT),             //����ѡ�񴰿�
				hWnd, RoleDlgSlet);
			break;
		case OPENBUTTON2:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_HELP_FORM),           //������������
				hWnd, HelpDlgProc);
			break;
		case OPENBUTTON3:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MORE_PRODUCER),       //���������߿���Ϣ
				hWnd, MoreDlgProd);
			break;
		}
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);                                            //�����˳���Ϣ
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