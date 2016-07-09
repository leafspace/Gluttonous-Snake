#include "OpenInit.h"
#include "resource.h"

extern HINSTANCE hInst;
extern HWND hMainWnd;
extern BOOL isCheck;
int SPEED = 100, VOL = 20;

BOOL WINAPI DoDispPopupMenu(HWND hWnd, int x, int y)
{
	HMENU hMenu;                                                                  //�����Ӳ˵�
	HMENU hPopupMenu;
	POINT PT = { x,y };                                                           //������������Ļ�ϵ�λ��
	//����Դ�ļ�������˵���Դ
	hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU));                           //����Ӳ˵��ľ��
	hPopupMenu = GetSubMenu(hMenu, 0);                                            //��������ת������Ļ����
	ClientToScreen(hMainWnd, &PT);                                                //���������˵�
	if (TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		PT.x, PT.y, 0, hMainWnd, NULL) == FALSE)
		return FALSE;                                                             //ʹ����ϣ������ڴ�
	DestroyMenu(hMenu);
	return TRUE;
}

VOID WINAPI DoCheckMenuItem(HWND hWnd)
{
	HMENU hPopupMenu = GetSubMenu(GetMenu(hMainWnd), 1);                          //���Ӳ˵�ת������Ļ����
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
		hDlgModeless = CreateDialog(hInst, TEXT("CONTROL"),                       //�����򿪿��ƴ���
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
		hCtrl = GetDlgItem(hDlg, 11);
		SetScrollRange(hCtrl, SB_CTL, 0, 100, FALSE);
		SetScrollPos(hCtrl, SB_CTL, VOL, FALSE); 
		return TRUE;                                                              //��ʼ���ɹ�������true
	case WM_VSCROLL:                                                              //���"��������ĳ��"�õ�����Ϣ
		hCtrl = (HWND)lParam;                                                     //������Ϣ��lParam����������Ĵ���
		iCtrlID = GetWindowLong(hCtrl, GWL_ID);                                   //���ݴ������ID
		iIndex = iCtrlID - 10;                                                    //�������
		switch (LOWORD(wParam))                                                   //���"��������ĳ��"��λ�þ�����ͬ��֪ͨ��
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
		InvalidateRect(hwndParent, NULL, TRUE);                                              //��������Ч
		waveOutSetVolume(NULL, (0xFFFF) / 100 * VOL*(0x10000 + 1));                          //����VOL����ֵ����������
		return TRUE;
	}
	return (INT_PTR)FALSE;
}