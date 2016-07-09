#include "OpenInit.h"
#include "resource.h"

extern HINSTANCE hInst;
extern HWND hMainWnd;
extern BOOL isCheck;
int SPEED = 100, VOL = 20;

BOOL WINAPI DoDispPopupMenu(HWND hWnd, int x, int y)
{
	HMENU hMenu;                                                                  //保存子菜单
	HMENU hPopupMenu;
	POINT PT = { x,y };                                                           //保存鼠标点在屏幕上的位置
	//从资源文件中载入菜单资源
	hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU));                           //获得子菜单的句柄
	hPopupMenu = GetSubMenu(hMenu, 0);                                            //窗口坐标转化到屏幕坐标
	ClientToScreen(hMainWnd, &PT);                                                //创建浮动菜单
	if (TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		PT.x, PT.y, 0, hMainWnd, NULL) == FALSE)
		return FALSE;                                                             //使用完毕，回收内存
	DestroyMenu(hMenu);
	return TRUE;
}

VOID WINAPI DoCheckMenuItem(HWND hWnd)
{
	HMENU hPopupMenu = GetSubMenu(GetMenu(hMainWnd), 1);                          //将子菜单转化到屏幕坐标
	static HWND hDlgModeless;                                                     //定义非模态对话框
	if (isCheck)                                                                  //如果控制窗口打开着
	{
		CheckMenuItem(hPopupMenu, 0, MF_BYPOSITION | MF_UNCHECKED);               //清除小圆圈
		EndDialog(hDlgModeless, 0);                                               //关闭控制窗口
		isCheck = 0;                                                              //设置标志位
	}
	else
	{
		CheckMenuItem(hPopupMenu, 0, MF_BYPOSITION | MF_CHECKED);                 //画上小圆圈
		hDlgModeless = CreateDialog(hInst, TEXT("CONTROL"),                       //创建打开控制窗口
			hWnd, ControlDlg);
		isCheck = 1;                                                              //设置标志位
	}
}

INT_PTR CALLBACK ControlDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND  hwndParent = GetParent(hDlg), hCtrl;
	int  iCtrlID, iIndex;
	switch (message)
	{
	case  WM_INITDIALOG: 
		hCtrl = GetDlgItem(hDlg, 10);                                             //将控制项的ID转换成代号
		SetScrollRange(hCtrl, SB_CTL, 0, 100, FALSE);                             //设置颜色值的范围
		SetScrollPos(hCtrl, SB_CTL, SPEED, FALSE);                                //设置滚动条的初始位置
		hCtrl = GetDlgItem(hDlg, 11);
		SetScrollRange(hCtrl, SB_CTL, 0, 100, FALSE);
		SetScrollPos(hCtrl, SB_CTL, VOL, FALSE); 
		return TRUE;                                                              //初始化成功，返回true
	case WM_VSCROLL:                                                              //点击"滚动条上某点"得到的消息
		hCtrl = (HWND)lParam;                                                     //根据消息的lParam求出滚动条的代号
		iCtrlID = GetWindowLong(hCtrl, GWL_ID);                                   //根据代号求出ID
		iIndex = iCtrlID - 10;                                                    //求出索引
		switch (LOWORD(wParam))                                                   //点击"滚动条上某点"的位置决定不同的通知码
		{
		case SB_PAGEDOWN:
			if (iIndex) {
				VOL = min(100, VOL + 10);
			}
			else {
				SPEED = min(100, SPEED + 10);
			}
			break;
		case SB_LINEDOWN:
			if (iIndex) {
				VOL = min(100, VOL + 1);
			}
			else {
				SPEED = min(100, SPEED + 1);
			}
			break;
		case SB_PAGEUP:
			if (iIndex) {
				VOL = max(0, VOL - 10);
			}
			else {
				SPEED = max(0, SPEED - 10);
			}
			break;
		case SB_LINEUP:
			if (iIndex) {
				VOL = max(0, VOL - 1);
			}
			else {
				SPEED = max(0, SPEED - 1);
			}
			break;
		case SB_TOP:
			if (iIndex) {
				VOL = 0; 
			}
			else {
				SPEED = 0;
			}
			break;
		case SB_BOTTOM:
			break;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			if (iIndex) {
				VOL = HIWORD(wParam); 
			}
			else {
				SPEED = HIWORD(wParam);
			}
			break;
		default:
			return FALSE;
		}
		SetScrollPos(hCtrl, SB_CTL, iIndex ?	VOL : SPEED, TRUE);
		SetDlgItemInt(hDlg, iCtrlID+3, iIndex ? VOL : SPEED, FALSE);
		InvalidateRect(hwndParent, NULL, TRUE);                                              //父窗口无效
		waveOutSetVolume(NULL, (0xFFFF) / 100 * VOL*(0x10000 + 1));                          //根据VOL的数值来设置音量
		return TRUE;
	}
	return (INT_PTR)FALSE;
}