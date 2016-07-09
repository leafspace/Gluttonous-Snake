#pragma once
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "DataStructure.h"
#define MAX_LOADSTRING 100
#define UP             51
#define DOWN           52
#define LEFT           53
#define RIGHT          54
#define KEY_W        0x57
#define KEY_S        0x53
#define KEY_A        0x41
#define KEY_D        0x44
extern WCHAR szTitle[MAX_LOADSTRING];
extern WCHAR szWindowClass[MAX_LOADSTRING];
extern HWND hMainWnd;
extern int SPEED;

BOOL SingleGame(HINSTANCE);
BOOL TwoGame(HINSTANCE);

void StartMusic(void);

INT_PTR CALLBACK GameRun(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK Game(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK GameOverPro(HWND, UINT, WPARAM, LPARAM);

INT_PTR CALLBACK TwoGamePlayer(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK TwoGameOverPro(HWND, UINT, WPARAM, LPARAM);

bool SnakeRun(void);
int RunPrognosis(void);
bool SearchHead(int);
void DrawSnakeFood(HWND, HDC);
void EndFree(void);

bool TwoSnakeRun(void);
int TwoRunPrognosis(int);
bool TwoSearchHead(int, int);
void TwoDrawSnakeFood(HWND, HDC);
void TwoEndFree(void);