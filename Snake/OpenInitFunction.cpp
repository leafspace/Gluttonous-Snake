#include <windows.h>
#include "resource.h"
#include "OpenInit.h"
bool Backgroundpaste(HWND hWnd,int ID,int x,int y)
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

bool BackgroundButton(HWND hWnd, LPARAM lParam)
{
	HWND buttonhWnd1, buttonhWnd2, buttonhWnd3;
	buttonhWnd1 = CreateWindow(TEXT("button"), TEXT("Game Start"),
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 155, 170, 160, 28,
		hWnd, (HMENU)OPENBUTTON1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	buttonhWnd2 = CreateWindow(TEXT("button"), TEXT("Operating Instructions"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 155, 210, 160, 28,
		hWnd, (HMENU)OPENBUTTON2, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	buttonhWnd3 = CreateWindow(TEXT("button"), TEXT("More About"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 155, 250, 160, 28,
		hWnd, (HMENU)OPENBUTTON3, ((LPCREATESTRUCT)lParam)->hInstance, NULL);
	return true;
}