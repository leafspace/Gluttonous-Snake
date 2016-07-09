#pragma once
#include <windows.h>

ATOM                MyRegisterClass(HINSTANCE);                              //注册窗口
BOOL                InitInstance(HINSTANCE, int);                            //初始化窗口

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);                     //主窗口响应回调函数
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);                       //关于窗口响应回调函数
INT_PTR CALLBACK    HelpDlgProc(HWND, UINT, WPARAM, LPARAM);                 //帮助窗口响应回调函数
INT_PTR CALLBACK    MoreDlgProd(HWND, UINT, WPARAM, LPARAM);                 //制作者帮助窗口响应回调函数
INT_PTR CALLBACK    ControlDlg(HWND, UINT, WPARAM, LPARAM);                  //控制条窗口响应回调函数
INT_PTR CALLBACK    RoleDlgSlet(HWND, UINT, WPARAM, LPARAM);                 //角色选择窗口响应回调函数

BOOL WINAPI DoDispPopupMenu(HWND, int, int);		                         //创建弹出式菜单
VOID WINAPI DoCheckMenuItem(HWND);			                                 //设置菜单项为选中标志

BOOL GetTimer();                                                             //延迟函数

bool Backgroundpaste(HWND,int,int,int);                                              //背景图片粘贴函数
bool BackgroundButton(HWND, LPARAM);                                         //背景按钮设置函数
