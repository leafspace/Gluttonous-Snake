#include "SnakeRun.h"
#include "OpenInit.h"
#include "SnakeResource.h"

int GameWindowX_A = 10, GameWindowX_B = 540;
int WindowWidth = 50, WindowHeight = 55;
int RunDirection_A = RIGHT, RunDirection_B = RIGHT;
LinkList SnakeHead_A = NULL, SnakeHead_B = NULL;
LinkList Head_A = NULL, Head_B = NULL;
int FoodBegin_A = 0, FoodBegin_B = 0;
int Point_A = 0, Point_B = 0;
Queue FoodQueue;

INT_PTR CALLBACK TwoGamePlayer(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		if (!TwoSnakeRun()) {
			PostMessage(hDlg, WM_GAMEOVER, wParam, lParam);
		}
		RECT rect;
		GetWindowRect(NULL, &rect);
		InvalidateRect(hDlg, &rect, false);
		hDC = GetDC(hDlg);
		InvalidateRgn(hDlg, NULL, false);
		UpdateWindow(hDlg);

		break;
	case WM_INITDIALOG:
		StartMusic();
		Head_A = (LNode*)malloc(sizeof(LNode));
		Head_A->data = 1;
		Head_A->next = NULL;
		SnakeHead_A->data = 1;
		SnakeHead_A->next = Head_A;
		Head_B = (LNode*)malloc(sizeof(LNode));
		Head_B->data = 1;
		Head_B->next = NULL;
		SnakeHead_B->data = 1;
		SnakeHead_B->next = Head_B;

		srand((unsigned)time(NULL));
		FoodQueue.front = 0;
		FoodQueue.data[FoodQueue.front] = rand() % (WindowWidth*WindowHeight + 1);
		FoodQueue.front++;
		PostMessage(hDlg, WM_TIMER, wParam, lParam);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hDlg, &PtStr);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(139, 69, 19));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 0, 0, 1050, 570);
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hDC, hBrush);
		SelectObject(hDC, hPen);
		Rectangle(hDC, 10, 10, 510, 560);
		Rectangle(hDC, 540, 10, 1040, 560);
		TwoDrawSnakeFood(hDlg, hDC);
		EndPaint(hDlg, &PtStr);

		break;
	case WM_CLOSE:
		EndDialog(hDlg, LOWORD(wParam));
		PlaySound(NULL, NULL, SND_FILENAME);
		DialogBox(NULL, MAKEINTRESOURCE(IDD_TWOGAMEOVER), hDlg, TwoGameOverPro);
		break;
	case WM_GAMEOVER:
		KillTimer(hDlg, 0);
		PlaySound(TEXT("E:\\³ÌÐò\\ÓïÑÔ³ÌÐò\\Windows±à³Ì\\Ì°³ÔÉß\\Ì°³ÔÉß\\Ì°³ÔÉß\\gameover.wav"), NULL, SND_FILENAME | SND_ASYNC);
		Point_A = SnakeHead_A->data - 1;
		Point_B = SnakeHead_B->data - 1;
		TwoEndFree();
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
		case KEY_A:
			if (RunDirection_A != RIGHT) {
				RunDirection_A = LEFT;
			}
			break;
		case KEY_S:
			if (RunDirection_A != UP) {
				RunDirection_A = DOWN;
			}
			break;
		case KEY_W:
			if (RunDirection_A != DOWN) {
				RunDirection_A = UP;
			}
			break;
		case KEY_D:
			if (RunDirection_A != LEFT) {
				RunDirection_A = RIGHT;
			}
		case VK_UP:
			if (RunDirection_B != DOWN) {
				RunDirection_B = UP;
			}
			break;
		case VK_DOWN:
			if (RunDirection_B != UP) {
				RunDirection_B = DOWN;
			}
			break;
		case VK_LEFT:
			if (RunDirection_B != RIGHT) {
				RunDirection_B = LEFT;
			}
			break;
		case VK_RIGHT:
			if (RunDirection_B != LEFT) {
				RunDirection_B = RIGHT;
			}
			break;
			break;
		default:
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool TwoSnakeRun(void)
{
	int NextNode_A = TwoRunPrognosis(0), NextNode_B = TwoRunPrognosis(1);
	if (NextNode_A <= 0 || TwoSearchHead(NextNode_A, 0) ||
		NextNode_B <= 0 || TwoSearchHead(NextNode_B, 1)) {
		return false;
	}
	LinkList q;
	q = (LNode*)malloc(sizeof(LNode));
	q->data = NextNode_A;
	q->next = Head_A;
	Head_A = q;
	SnakeHead_A->next = Head_A;
	q = (LNode*)malloc(sizeof(LNode));
	q->data = NextNode_B;
	q->next = Head_B;
	Head_B = q;
	SnakeHead_B->next = Head_B;

	int Food_A = FoodQueue.data[FoodBegin_A], Food_B = FoodQueue.data[FoodBegin_B];
	if (NextNode_A == Food_A) {
		/*do
		{
			srand((unsigned)time(NULL));
			FoodQueue.data[FoodQueue.front] = rand() % (WindowWidth*WindowHeight) + 1;
			FoodQueue.front++;
			if (!TwoSearchHead(Food_A, 0)) {
				break;
			}
		} while (true);*/
		srand((unsigned)time(NULL));
		FoodQueue.data[FoodQueue.front] = rand() % (WindowWidth*WindowHeight) + 1;
		FoodQueue.front++;

		SnakeHead_A->data++;
		FoodBegin_A++;
	}
	else {
		LinkList P = SnakeHead_A;
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
	if (NextNode_B == Food_B) {
		/*do
		{
			srand((unsigned)time(NULL));
			FoodQueue.data[FoodQueue.front] = rand() % (WindowWidth*WindowHeight) + 1;
			FoodQueue.front++;
			if (!TwoSearchHead(Food_B, 1)) {
				break;
			}
		} while (true);*/
		srand((unsigned)time(NULL));
		FoodQueue.data[FoodQueue.front] = rand() % (WindowWidth*WindowHeight) + 1;
		FoodQueue.front++;
		SnakeHead_B->data++;
		FoodBegin_B++;
	}
	else {
		LinkList P = SnakeHead_B;
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

int TwoRunPrognosis(int member)
{
	int Direction = !member ? RunDirection_A : RunDirection_B;
	int Node = -1;
	switch (Direction)
	{
	case UP:
		if (!member) {
			if (Head_A->data - WindowWidth > 0) {
				Node = Head_A->data - WindowWidth;
			}
		}
		else {
			if (Head_B->data - WindowWidth > 0) {
				Node = Head_B->data - WindowWidth;
			}
		}
		break;
	case DOWN:
		if (!member) {
			if (Head_A->data + WindowWidth < WindowWidth*WindowHeight) {
				Node = Head_A->data + WindowWidth;
			}
		}
		else {
			if (Head_B->data + WindowWidth < WindowWidth*WindowHeight) {
				Node = Head_B->data + WindowWidth;
			}
		}
		break;
	case LEFT:
		if (!member) {
			if ((Head_A->data - 1) % WindowWidth) {
				Node = Head_A->data - 1;
			}
		}
		else {
			if ((Head_B->data - 1) % WindowWidth) {
				Node = Head_B->data - 1;
			}
		}
		break;
	case RIGHT:
		if (!member) {
			if ((Head_A->data + 1) % WindowWidth) {
				Node = Head_A->data + 1;
			}
		}
		else {
			if ((Head_B->data + 1) % WindowWidth) {
				Node = Head_B->data + 1;
			}
		}
		break;
	default:
		MessageBox(NULL, TEXT("Failed to direction !"), TEXT("Error"), 0);
		exit(0);
	}
	return Node;
}

bool TwoSearchHead(int Node,int member)
{
	LinkList P = !member ? Head_A : Head_B;
	while (P) {
		if (P->data == Node) {
			return TRUE;
		}
		P = P->next;
	}
	return FALSE;
}

void TwoDrawSnakeFood(HWND hDlg, HDC hDC)
{
	HPEN hPen;
	LinkList P;
	P = Head_A;
	while (P) {
		int Space = P->data;
		hPen = CreatePen(PS_SOLID, 2, RGB(20, 252, 108));
		SelectObject(hDC, hPen);
		int x = GameWindowX_A + ((Space - 1) % WindowWidth) * 10, y = 10 + (Space / WindowWidth) * 10;
		Rectangle(hDC, x, y, x + 10, y + 10);
		P = P->next;
	}
	P = Head_B;
	while (P) {
		int Space = P->data;
		hPen = CreatePen(PS_SOLID, 2, RGB(20, 252, 108));
		SelectObject(hDC, hPen);
		int x = GameWindowX_B + ((Space - 1) % WindowWidth) * 10, y = 10 + (Space / WindowWidth) * 10;
		Rectangle(hDC, x, y, x + 10, y + 10);
		P = P->next;
	}
	hPen = CreatePen(PS_SOLID, 2, RGB(255, 55, 55));
	SelectObject(hDC, hPen);
	int Food_A = FoodQueue.data[FoodBegin_A], Food_B = FoodQueue.data[FoodBegin_B];
	int x_A = GameWindowX_A + ((Food_A - 1) % WindowWidth) * 10, y_A = 10 + Food_A / WindowWidth * 10;
	Rectangle(hDC, x_A, y_A, x_A + 10, y_A + 10);
	int x_B = GameWindowX_B + ((Food_B - 1) % WindowWidth) * 10, y_B = 10 + Food_B / WindowWidth * 10;
	Rectangle(hDC, x_B, y_B, x_B + 10, y_B + 10);
}

void TwoEndFree(void)
{
	LinkList P, Next;
	P = SnakeHead_A;
	Next = Head_A;
	while (Next) {
		free(P);
		P = Next;
		Next = Next->next;
	}
	P = SnakeHead_B;
	Next = Head_B;
	while (Next) {
		free(P);
		P = Next;
		Next = Next->next;
	}
	SnakeHead_A = NULL;
	Head_A = NULL;
	RunDirection_A = RIGHT;
	SnakeHead_B = NULL;
	Head_B = NULL;
	RunDirection_B = RIGHT;
}

INT_PTR CALLBACK TwoGameOverPro(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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
		char str2[20] = "YOU SCORE:";
		char str3[20] = { 0 };
		hDC = BeginPaint(hDlg, &PtStr);
		_itoa_s(Point_A, str3, 20);
		strcat_s(str1, str3);
		strcat_s(str2, str3);
		strcat_s(str2, "0");
		TextOutA(hDC, 210, 460, str1, strlen(str1));
		TextOutA(hDC, 210, 490, str2, strlen(str2));

		_itoa_s(Point_B, str3, 20);
		strcpy_s(str1, "YOU FOOD:");
		strcpy_s(str2, "YOU SCORE:");
		strcat_s(str1, str3);
		strcat_s(str2, str3);
		strcat_s(str2, "0");
		TextOutA(hDC, 210 + GameWindowX_B, 460, str1, strlen(str1));
		TextOutA(hDC, 210 + GameWindowX_B, 490, str2, strlen(str2));
		EndPaint(hDlg, &PtStr);
		int Left, Right;
		if (Point_A > Point_B) {
			Left = IDB_SMILE;
			Right = IDB_CRY;
		}
		else if (Point_A < Point_B) {
			Left = IDB_CRY;
			Right = IDB_SMILE;
		}
		else {
			Left = IDB_SMILE;
			Right = IDB_SMILE;
		}
		hCtrlBlock = GetDlgItem(hDlg, IDC_PAINTA);
		Backgroundpaste(hCtrlBlock, Left, -10, 0);
		hCtrlBlock = GetDlgItem(hDlg, IDC_PAINTB);
		Backgroundpaste(hCtrlBlock, Right, 15, 0);
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