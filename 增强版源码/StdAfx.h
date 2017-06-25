// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Local Header Files
#include <stdio.h>
#include <math.h>
#include <ctype.h>

// TODO: reference additional headers your program requires here
#define GAMEW 16
#define GAMEH 12

#define VIEWW GAMEW*32 //512
#define VIEWH GAMEH*32 //384

#define MAX_MAP_OBJECT 30
#define MAX_PAGE 5

//BOSS
#define BOSS_CURSOR 15

//time
#define GAME_TIME_CLIP 40

#define TIME_GAME_IN 300
#define TIME_GAME_IN_PRE 2
#define TIME_GAME_END 3
#define TIME_GAME_WIN_WAIT 2
#define TIME_GAME_PUMP_WAIT 2
#define TIME_GAME_FAIL_WAIT 2


//game
#define MAX_MATCH 3

//游戏开始菜单
#define GAME_PRE 0
//游戏进行中
#define GAME_IN 1
#define GAME_WIN 2
#define GAME_OVER 3
#define GAME_IN_PRE 4
#define GAME_PASS 5
#define GAME_WIN_WAIT 6
#define GAME_FAIL_WAIT 7
//进出下水管道
#define GAME_PUMP_IN 8
#define GAME_PUMP_OUT 9
#define GAME_HELP 10

//程序错误
#define GAME_ERR 50

//role
#define ROLE_STEP 4
#define JUMP_HEIGHT 3
#define FAIL_FRAME 2

//map
#define LOAD_MAP 1
#define LOAD_MAP_BK_OBJ 2
#define LOAD_MAP_ENEMY 3

#define PATH_MAP "map\\map1.txt"

//fire
#define FIREW 17
#define FIREH 17
#define FIRE_XOFF 32-FIREW

//bomb
#define BOMB_XOFF 20
#define BOMB_YOFF 23
#define COIN_XOFF 0
#define COIN_YOFF 32

//ani
#define ID_ANI_ENEMY_NORMAL 0
#define ID_ANI_FIRE 1
#define ID_ANI_BOMB 2
#define ID_ANI_COIN 3
#define ID_ANI_COIN_SCORE 4
#define ID_ANI_ATTACK 5
#define ID_ANI_BOSS_HOUSE 6
#define ID_ANI_ENEMY_SWORD 7
#define ID_ANI_BOSS_HOUSE_A 8 
#define ID_ANI_MENU_ARROW 9 

//attack
#define ATTACK_MAX_TYPE 2

//magic ani
#define ID_ANI_FIRE_MAGIC 11
#define ID_ANI_FIRE_MAGIC_YOFF 96 //128 - 32
#define ID_ANI_FIRE_MAGIC_XOFF 80 //192 - 32 = 160
//
#define FIRE_MAGIC_H 128
#define FIRE_MAGIC_W 96
#define FIRE_MAGIC_MAX_W 192
#define ID_BK_WATER 2


//view
#define STATE_VIEW_STOP 0

//speed
// n(n+1)/2=32*3(jump height)
#define SPEED_JUMP 13
//人物降落,加速度从1开始递增
//#define SPEED_DOWN 16

//敌人每次移动的距离
#define ENEMY_STEP_X 4

//敌人每次移动的时间间隔
#define ENEMY_SPEED 2

#define WATER_SPEED 5

//
#define FIRE_SPEED 12
#define TIME_FIRE_BETWEEN 5
//boss生成敌人的时间间隔
#define TIME_CREATE_ENEMY 20

//WIN
#define X_WIN 73
#define Y_WIN 8
//背景移动
#define SKY_TIME 5
#define SKY_SPEED 1

#define MENU_ARROW_TIME 10

//ATTACK
#define ATTACK_NORMAL 0
#define ATTACK_MAGIC 1

//object id
#define ID_MAP_PUMP_IN 9
#define ID_MAP_PUMP_OUT 10
#define ID_MAP_HEALTH 11
#define ID_MAP_HEALTH_BK 12

#define BMP_WIDTH_HEALTH 20

//角色动画
#define ROLE_ANI_DOWN 0
#define ROLE_ANI_UP 1

//图像坐标
#define ATTACK_TO_TEXT_X 220
#define ATTACK_TO_TEXT_Y 15
#define ATTACK_TO_X 250
#define ATTACK_TO_Y 40

#define BOSS_HEALTH 5
#define BOSS_A_HEALTH 8
#define ENEMY_SWORD_HEALTH 2

//错误处理
#define ERR_MAP_FILE 0

//调试方法
//#define DEBUG_CREATE_ENEMY 


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
