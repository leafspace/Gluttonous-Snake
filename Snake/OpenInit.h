#pragma once
#include <windows.h>

ATOM                MyRegisterClass(HINSTANCE);                              //ע�ᴰ��
BOOL                InitInstance(HINSTANCE, int);                            //��ʼ������

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);                     //��������Ӧ�ص�����
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);                       //���ڴ�����Ӧ�ص�����
INT_PTR CALLBACK    HelpDlgProc(HWND, UINT, WPARAM, LPARAM);                 //����������Ӧ�ص�����
INT_PTR CALLBACK    MoreDlgProd(HWND, UINT, WPARAM, LPARAM);                 //�����߰���������Ӧ�ص�����
INT_PTR CALLBACK    ControlDlg(HWND, UINT, WPARAM, LPARAM);                  //������������Ӧ�ص�����
INT_PTR CALLBACK    RoleDlgSlet(HWND, UINT, WPARAM, LPARAM);                 //��ɫѡ�񴰿���Ӧ�ص�����

BOOL WINAPI DoDispPopupMenu(HWND, int, int);		                         //��������ʽ�˵�
VOID WINAPI DoCheckMenuItem(HWND);			                                 //���ò˵���Ϊѡ�б�־

BOOL GetTimer();                                                             //�ӳٺ���

bool Backgroundpaste(HWND,int,int,int);                                              //����ͼƬճ������
bool BackgroundButton(HWND, LPARAM);                                         //������ť���ú���
