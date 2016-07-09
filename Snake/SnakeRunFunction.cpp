#include <math.h>
#include "SnakeRun.h"
#include "OpenInit.h"
#include "SnakeResource.h"
#define PI 3.14
LinkList SnakeHead;
LinkList Head = NULL;
int RunDirection = RIGHT;
int Food;
int WidthNode = 104, HeightNode = 55;
int PointNumber = 0;

extern LinkList SnakeHead_A, SnakeHead_B;

BOOL SingleGame(HINSTANCE hInst)
{
	SnakeHead = (LNode*)malloc(sizeof(LNode));
	DialogBox(hInst, MAKEINTRESOURCE(IDD_GAMEWINDOW), hMainWnd, GameRun);
	return true;
}

BOOL TwoGame(HINSTANCE hInst)
{
	SnakeHead_A = (LNode*)malloc(sizeof(LNode));
	SnakeHead_B = (LNode*)malloc(sizeof(LNode));
	DialogBox(hInst, MAKEINTRESOURCE(IDD_TWOGAMEWINDOW), hMainWnd, TwoGamePlayer);
	return true;
}

void StartMusic(void)
{
	PlaySound(TEXT("ready.wav"), NULL, SND_FILENAME | SND_ASYNC);
	Sleep(700);
	PlaySound(NULL, NULL, SND_FILENAME);                                        //Í£Ö¹²¥·ÅÉùÒô
	PlaySound(TEXT("go.wav"), NULL, SND_FILENAME | SND_ASYNC);
	Sleep(1000);
	PlaySound(TEXT("½ð¸ÕºùÂ«ÍÞ.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

}

INT_PTR CALLBACK GameRun(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HDC hDC = 0;
	HBRUSH hBrush;
	HPEN hPen;
	PAINTSTRUCT PtStr;
	switch (message)
	{
	case WM_TIMER:
		SetTimer(hDlg, 0, SPEED * 8 + 200, 0);
		if (!SnakeRun()) {
			PostMessage(hDlg, WM_GAMEOVER, wParam, lParam);
		}
		RECT rect;
		GetWindowRect(NULL,&rect);
		InvalidateRect(hDlg,&rect, false);
		hDC = GetDC(hDlg);
		InvalidateRgn(hDlg, NULL, false);
		UpdateWindow(hDlg);
		break;
	case WM_INITDIALOG:
	{
		StartMusic();
		LinkList q = (LNode*)malloc(sizeof(LNode));
		q->data = 1;
		q->next = NULL;
		SnakeHead->data = 1;
		SnakeHead->next = q;
		Head = q;
		srand((unsigned)time(NULL));
		Food = rand() % (WidthNode*HeightNode) + 1;
		PostMessage(hDlg, WM_TIMER, wParam, lParam);
	}
		break;
	case WM_PAINT:
	{
		hDC = BeginPaint(hDlg, &PtStr);

		HBITMAP hbm;
		BITMAP bminfo;
		HDC memdc = CreateCompatibleDC(hDC);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(139, 69, 19));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 0, 0, 1050, 570);

		hbm = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SKYBG));
		GetObject(hbm, sizeof(bminfo), &bminfo);
		SelectObject(memdc, hbm);
		BitBlt(hDC, 10, 10, bminfo.bmWidth, bminfo.bmHeight, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);

		/*
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hBrush= CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 10, 10, 1040, 560);
		*/

		DrawSnakeFood(hDlg, hDC);
		EndPaint(hDlg, &PtStr);
		
	}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		PlaySound(NULL, NULL, SND_FILENAME);
		DialogBox(NULL, MAKEINTRESOURCE(IDD_GAMEOVER), hDlg, GameOverPro);
		break;
	case WM_GAMEOVER:
		KillTimer(hDlg, 0);
		PlaySound(TEXT("gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
		EndFree();
		Sleep(3000);
		PostMessage(hDlg, WM_CLOSE, wParam, lParam);
		break;
	default:
		switch (wParam)
		{
		case 0x70: 
			DoCheckMenuItem(hDlg);
			break;
		case 0x6B:
			SPEED = SPEED - 5 < 0 ? 0 : SPEED - 5;
			break;
		case 0x6D:
			SPEED = SPEED + 5 > 100 ? 100 : SPEED + 5;
			break;
		case VK_UP:
		case KEY_W:
			if (RunDirection != DOWN) {
				RunDirection = UP;
			}			
			break;
		case VK_DOWN:
		case KEY_S:
			if (RunDirection != UP) {
				RunDirection = DOWN;
			}			
			break;
		case VK_LEFT:
		case KEY_A:
			if (RunDirection != RIGHT){
				RunDirection = LEFT;
			}			
			break;
		case VK_RIGHT:
		case KEY_D:
			if (RunDirection != LEFT) {
				RunDirection = RIGHT;
			}			
			break;
		case VK_LBUTTON:
			if (message == WM_LBUTTONDOWN) {
				POINT mousePos;
				GetCursorPos(&mousePos);
				ScreenToClient(hDlg, &mousePos);
				int Space = Head->data;
				int x = 10 + ((Space - 1) % WidthNode) * 10, y = 10 + Space / WidthNode * 10;
				double Tan = (mousePos.y - y)*1.0 / (mousePos.x - x);
				double Result = asin(Tan);
				int RunDirectionTemp;
				if (Result > -PI / 2 && Result < -PI / 4) {
					if (mousePos.y > y) {
						RunDirectionTemp = UP;
					}
					else {
						RunDirectionTemp = DOWN;
					}
				}
				else if (Result > -PI / 4 && Result < 0) {
					if (mousePos.y > y) {
						RunDirectionTemp = LEFT;
					}
					else {
						RunDirectionTemp = RIGHT;
					}
				}
				else if (Result > 0 && Result < PI / 4) {
					if (mousePos.y > y) {
						RunDirectionTemp = RIGHT;
					}
					else {
						RunDirectionTemp = LEFT;
					}
				}
				else {
					if (mousePos.y > y) {
						RunDirectionTemp = DOWN;
					}
					else {
						RunDirectionTemp = UP;
					}
				}
				switch (RunDirectionTemp)
				{
				case UP:
					if (RunDirection != DOWN) {
						RunDirection = RunDirectionTemp;
					}
					break;
				case DOWN:
					if (RunDirection != UP) {
						RunDirection = RunDirectionTemp;
					}
					break;
				case LEFT:
					if (RunDirection != RIGHT) {
						RunDirection = RunDirectionTemp;
					}
					break;
				case RIGHT:
					if (RunDirection != LEFT) {
						RunDirection = RunDirectionTemp;
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
				break;
		}
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
		PointNumber++;
	}
	else {
		//È¥µôÎ²½áµã
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

int RunPrognosis(void)
{
	switch (RunDirection)
	{
	case UP:
		if (Head->data - WidthNode < 0){
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
		if ((Head->data + 1) % WidthNode == 0) {
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

void DrawSnakeFood(HWND hDlg, HDC hDC)
{
	HPEN hPen;
	LinkList P = Head;
	while (P) {
		int Space = P->data;
		hPen = CreatePen(PS_SOLID, 2, RGB(20, 252, 108));
		SelectObject(hDC, hPen);
		int x = 10 + ((Space - 1) % WidthNode) * 10, y = 10 + Space / WidthNode * 10;
		Rectangle(hDC, x, y, x + 10, y + 10);
		P = P->next;
	}
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 55, 55));
	SelectObject(hDC, hPen);
	int x = 10 + ((Food - 1) % WidthNode) * 10, y = 10 + Food / WidthNode * 10;
	Rectangle(hDC, x, y, x + 10, y + 10);
	
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
	PointNumber = 0;
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
		_itoa_s(PointNumber, str3, 20);
		strcat_s(str1, str3);
		strcat_s(str2, str3);
		strcat_s(str2, "0");
		TextOutA(hDC, 210, 460, str1, strlen(str1));
		TextOutA(hDC, 210, 490, str2, strlen(str2));
		EndPaint(hDlg, &PtStr);
		hCtrlBlock = GetDlgItem(hDlg, IDC_PAINT);
		Backgroundpaste(hCtrlBlock, IDB_SNAKE, -20, 0);
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