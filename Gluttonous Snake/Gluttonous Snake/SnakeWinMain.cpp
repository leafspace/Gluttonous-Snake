#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include "resource.h"
#define MAX_LOADSTRING 100                      //�������ô�����󳤶�
typedef  int ElemType;

typedef struct LNode
{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;

HINSTANCE hInst;                                //���
HWND hMainWnd;
WCHAR szTitle[MAX_LOADSTRING];                  //������
WCHAR szWindowClass[MAX_LOADSTRING];            //��������
BOOL isCheck = 0;

int SPEED = 100;
int nScreenWidth, nScreenHeight;
int WindowWidth = 520, WindowHeight = 520;
int WidthNode = 50, HeightNode = 50;
int PointNumber = 0;

LinkList SnakeHead;
LinkList Head = NULL;
int RunDirection = RIGHT;
int Food;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

VOID WINAPI DoCheckMenuItem(HWND);
INT_PTR CALLBACK ControlDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK GameOverPro(HWND, UINT, WPARAM, LPARAM);

bool SnakeRun(void);
void DrawSnakeFood(HWND, HDC);
void EndFree(void);
int RunPrognosis(void);
bool SearchHead(int);
bool Backgroundpaste(HWND, int, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	                  _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPWSTR    lpCmdLine,
	                  _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);                                      //���߱���������Щ�����Ѿ�ʹ�ã����þ���
	UNREFERENCED_PARAMETER(lpCmdLine);                                          //���߱���������Щ�����Ѿ�ʹ�ã����þ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);             //����Դ�ڼ������ֵ�����szTitle��
	LoadStringW(hInstance, IDS_CLASS_TITLE, szWindowClass, MAX_LOADSTRING);    //����Դ�ڼ������ֵ�����szWindowClass��
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		MessageBox(NULL, TEXT("Failed to initialize !"), TEXT("Error"), 0);
		return FALSE;
	}
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_MENU));
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
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
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//��ʼ������
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	RECT rect;
	SetRect(&rect, 0, 0, WindowWidth, WindowHeight);
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW,true);
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		(nScreenWidth - (rect.right - rect.left)) / 2, (nScreenHeight - (rect.bottom - rect.top)) / 2,
		(rect.right - rect.left), (rect.bottom - rect.top), nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	hMainWnd = hWnd;
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//����ѡ��˵�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC = 0;
	HBRUSH hBrush;
	HPEN hPen;
	PAINTSTRUCT PtStr;
	switch (message)
	{
	case WM_CREATE:
	{
		LinkList q = (LNode*)malloc(sizeof(LNode));
		SnakeHead = (LNode*)malloc(sizeof(LNode));
		q->data = 1;
		q->next = NULL;
		SnakeHead->data = 1;
		SnakeHead->next = q;
		Head = q;
		srand((unsigned)time(NULL));
		Food = rand() % (WidthNode*HeightNode) + 1;
		SetTimer(hWnd, 0, SPEED * 8 + 200, 0);
		PostMessage(hWnd, WM_TIMER, wParam, lParam);
	}
	return 0;
	case WM_TIMER:
	{
		SetTimer(hWnd, 0, SPEED * 8 + 200, 0);
		if (!SnakeRun()) {
			PostMessage(hWnd, WM_GAMEOVER, wParam, lParam);
		}
		RECT rect;
		GetWindowRect(NULL, &rect);
		InvalidateRect(hWnd, &rect, TRUE);
		hDC = GetDC(hWnd);
		InvalidateRgn(hWnd, NULL, false);
		UpdateWindow(hWnd);
	}
	return 0;
	case WM_SIZE:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		WindowWidth = rect.right - rect.left;
		WindowHeight = rect.bottom - rect.top;
		SetRect(&rect, 0, 0, WindowWidth, WindowHeight);
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);
		MoveWindow(hWnd, (nScreenWidth - (rect.right - rect.left)) / 2, (nScreenHeight - (rect.bottom - rect.top)) / 2
			, (rect.right - rect.left), (rect.bottom - rect.top), true);
	}
	return 0;
	case WM_PAINT:
	{
		hDC = BeginPaint(hWnd, &PtStr);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(139, 69, 19));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 0, 0, WindowWidth, WindowHeight);
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 10, 10, WindowWidth - 10, WindowHeight - 10);
		DrawSnakeFood(hWnd, hDC);
		EndPaint(hWnd, &PtStr);
	}
	return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_MENU_BOX:
			DoCheckMenuItem(hWnd);
			break;
		case ID_MENU_EXIT:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
	}
	return 0;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
			if (RunDirection != DOWN) {
				RunDirection = UP;
			}
			break;
		case VK_DOWN:
			if (RunDirection != UP) {
				RunDirection = DOWN;
			}
			break;
		case VK_LEFT:
			if (RunDirection != RIGHT) {
				RunDirection = LEFT;
			}
			break;
		case VK_RIGHT:
			if (RunDirection != LEFT) {
				RunDirection = RIGHT;
			}
			break;
		default:
			break;
		}
	}
	return 0;
	case WM_GAMEOVER:
	{
		KillTimer(hWnd, 0);
		PointNumber = SnakeHead->data - 1;
		EndFree();
		Sleep(3000);
		PostMessage(hWnd, WM_CLOSE, wParam, lParam);
	}
	return 0;
	case WM_CLOSE:
		DialogBox(NULL, MAKEINTRESOURCE(IDD_GAMEOVER), hWnd, GameOverPro);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

VOID WINAPI DoCheckMenuItem(HWND hWnd)
{
	HMENU hPopupMenu = GetSubMenu(GetMenu(hMainWnd), 0);                          //���Ӳ˵�ת������Ļ����
	static HWND hDlgModeless;                                                     //�����ģ̬�Ի���
	if (isCheck)                                                                  //������ƴ��ڴ���
	{
		CheckMenuItem(hPopupMenu, 0, MF_BYPOSITION | MF_UNCHECKED);               //���СԲȦ
		EndDialog(hDlgModeless, 0);                                               //�رտ��ƴ���
		isCheck = 0;                                                              //���ñ�־λ
	}
	else
	{
		CheckMenuItem(hPopupMenu, 0, MF_BYPOSITION | MF_CHECKED);                 //����СԲȦ
		hDlgModeless = CreateDialog(hInst, TEXT("CONTROL"),                      //�����򿪿��ƴ���
			hWnd, ControlDlg);
		isCheck = 1;                                                              //���ñ�־λ
	}
}

INT_PTR CALLBACK ControlDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND  hwndParent = GetParent(hDlg), hCtrl;
	int  iCtrlID, iIndex;
	switch (message)
	{
	case  WM_INITDIALOG:
		hCtrl = GetDlgItem(hDlg, 10);                                             //���������IDת���ɴ���
		SetScrollRange(hCtrl, SB_CTL, 0, 100, FALSE);                             //������ɫֵ�ķ�Χ
		SetScrollPos(hCtrl, SB_CTL, SPEED, FALSE);                                //���ù������ĳ�ʼλ��
		return TRUE;                                                              //��ʼ���ɹ�������true
	case WM_VSCROLL:                                                              //���"��������ĳ��"�õ�����Ϣ
		hCtrl = (HWND)lParam;                                                     //������Ϣ��lParam����������Ĵ���
		iCtrlID = GetWindowLong(hCtrl, GWL_ID);                                   //���ݴ������ID
		iIndex = iCtrlID - 10;                                                    //�������
		switch (LOWORD(wParam))                                                   //���"��������ĳ��"��λ�þ�����ͬ��֪ͨ��
		{
		case SB_PAGEDOWN:
			SPEED = min(100, SPEED + 10);
			break;
		case SB_LINEDOWN:
			SPEED = min(100, SPEED + 1);
			break;
		case SB_PAGEUP:
			SPEED = max(0, SPEED - 10);
			break;
		case SB_LINEUP:
			SPEED = max(0, SPEED - 1);
			break;
		case SB_TOP:
			SPEED = 0;
			break;
		case SB_BOTTOM:
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			SPEED = HIWORD(wParam);
			break;
		default:
			return FALSE;
		}
		SetScrollPos(hCtrl, SB_CTL, SPEED, TRUE);
		SetDlgItemInt(hDlg, iCtrlID + 3, SPEED, FALSE);
		InvalidateRect(hwndParent, NULL, TRUE);                                              //��������Ч
		return TRUE;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GameOverPro(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	static HWND hCtrlBlock;
	static int num = 0;
	HDC hDC = 0;
	PAINTSTRUCT PtStr;
	switch (message)
	{
	case WM_TIMER:
		SetTimer(hDlg, 0, 10000, 0);
		if (num) {
			KillTimer(hDlg, 0);
			PostMessage(hDlg, WM_CLOSE, wParam, lParam);
		}
		num++;
		break;
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	case WM_PAINT:
	{
		char str1[20] = "YOU FOOD:";
		char str2[20] = "YOU SCORE :";
		char str3[20] = { 0 };
		hDC = BeginPaint(hDlg, &PtStr);
		//_itoa_s(PointNumber, str3, 20);
		sprintf_s(str3, "%d", PointNumber);
		strcat_s(str1, str3);
		strcat_s(str2, str3);
		strcat_s(str2, "0");
		TextOutA(hDC, 210, 460, str1, strlen(str1));
		TextOutA(hDC, 210, 490, str2, strlen(str2));
		EndPaint(hDlg, &PtStr);
		hCtrlBlock = GetDlgItem(hDlg, IDC_PAINT);
		int PIC = PointNumber >= 10 ? IDB_SMILE : IDB_CRY;
		Backgroundpaste(hCtrlBlock, PIC, -20, 0);
		PostMessage(hDlg, WM_TIMER, wParam, lParam);
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_OK || LOWORD(wParam) == IDCANCEL) {
			PostMessage(hDlg, WM_CLOSE, wParam, lParam);
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return (INT_PTR)FALSE;
}

bool SnakeRun(void)
{
	int NextNode = RunPrognosis();
	if (NextNode <= 0 || SearchHead(NextNode)) {
		return FALSE;
	}
	LinkList q = (LNode*)malloc(sizeof(LNode));
	q->data = NextNode;
	q->next = Head;
	Head = q;
	SnakeHead->next = Head;
	if (NextNode == Food) {
		do
		{
			srand((unsigned)time(NULL));
			Food = rand() % (WidthNode*HeightNode) + 1;
			if (!SearchHead(Food)) {
				break;
			}
		} while (true);
		SnakeHead->data++;
	}
	else {
		LinkList P = SnakeHead;
		while (TRUE)
		{
			if (P->next->next == NULL) {
				break;
			}
			else {
				P = P->next;
			}
		}
		q = P->next;
		free(q);
		P->next = NULL;
	}
	return TRUE;
}

void DrawSnakeFood(HWND hDlg, HDC hDC)
{
	HPEN hPen;
	LinkList P = Head;
	while (P) {
		int Space = P->data;
		hPen = CreatePen(PS_SOLID, 2, RGB(20, 252, 108));
		SelectObject(hDC, hPen);
		int x = 10 + ((Space - 1) % WidthNode) * (WindowWidth / WidthNode), y = 10 + Space / WidthNode * (WindowHeight / HeightNode);
		Rectangle(hDC, x, y, x + (WindowWidth / WidthNode), y + (WindowHeight / HeightNode));
		P = P->next;
	}
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 55, 55));
	SelectObject(hDC, hPen);
	int x = 10 + ((Food - 1) % WidthNode) * (WindowWidth / WidthNode), y = 10 + Food / WidthNode * (WindowHeight / HeightNode);
	Rectangle(hDC, x, y, x + (WindowWidth / WidthNode), y + (WindowHeight / HeightNode));

}

void EndFree(void)
{
	LinkList P, Next;
	P = SnakeHead;
	Next = Head;
	while (Next) {
		free(P);
		P = Next;
		Next = Next->next;
	}
	SnakeHead = NULL;
	Head = NULL;
	RunDirection = RIGHT;
}

int RunPrognosis(void)
{
	switch (RunDirection)
	{
	case UP:
		if (Head->data - WidthNode < 0) {
			return -1;
		}
		return Head->data - WidthNode;
	case DOWN:
		if (Head->data + WidthNode > WidthNode*HeightNode) {
			return -1;
		}
		return Head->data + WidthNode;
	case LEFT:
		if ((Head->data - 1) % WidthNode == 0) {
			return -1;
		}
		else {
			return Head->data - 1;
		}
	case RIGHT:
		if ((Head->data) % WidthNode == 0) {
			return -1;
		}
		else {
			return Head->data + 1;
		}
		break;
	default:
		MessageBox(NULL, TEXT("Failed to direction !"), TEXT("Error"), 0);
		exit(0);
	}
}

bool SearchHead(int Node)
{
	LinkList P = Head;
	while (P) {
		if (P->data == Node) {
			return TRUE;
		}
		P = P->next;
	}
	return FALSE;
}

bool Backgroundpaste(HWND hWnd, int ID, int x, int y)
{
	HBITMAP hbm;
	BITMAP bminfo;
	PAINTSTRUCT PS;
	HDC dc = BeginPaint(hWnd, &PS);
	HDC memdc = CreateCompatibleDC(dc);

	hbm = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(ID));
	GetObject(hbm, sizeof(bminfo), &bminfo);
	SelectObject(memdc, hbm);
	BitBlt(dc, x, y, bminfo.bmWidth, bminfo.bmHeight, memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);

	EndPaint(hWnd, &PS);
	UpdateWindow(hWnd);
	return true;
}