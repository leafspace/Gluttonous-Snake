#ifndef __GAMEMAP
#define __GAMEMAP

#include "bitmaptool.h"

struct MapObject
{
	int x;
	int y;
	int w;
	int h;
	int id;
	int iframe;
	int iframemax;//���֡��
	int show;	//�Ƿ���ʾ
};

struct ROLE
{
	int x;
	int y;
	int w;
	int h;
	int id;
	int iframe;
	int iframemax;//���֡��

	//�ƶ�����
	int xleft;//�����	
	int xright;//�ҽ���
	int movex;

	//��������
	int health;

	int show;	//�Ƿ���ʾ
};

struct MAPINFO
{
	int iNextMap;
	int iSubMap;
	int xReturnPoint;
	int yReturnPoint;
	int iBackBmp;
	int viewmax;
};

class GAMEMAP
{
public:
	int LoadMap();
	
	void Init();
	void InitMatch();
	//���,��������ͼ
	void Show(MYANIOBJ & bmobj);
	//���,������������ͼ
	void ShowBkObj(MYANIOBJ & bmobj);
	//���,������������ͼ
	void ShowAniObj(MYANIOBJ & bmobj);
	void ShowInfo(HDC h);
	void ShowOther(HDC h);

	//���̴���
	int KeyProc(int iKey);
	void KeyUpProc(int iKey);

	void MoveView();
	//������ͼ��ʼ����
	void SetView(int x);
	void SetViewState(int i);
	void SetGameState(int i);
	//hittest
	int RoleCanMove(int xoff, int yoff);
	int CheckRole();
	int CheckAni(int itimeclip);
	void ClearEnemy(int i);
	void ClearCoin(int i);
	//����֡����
	void ChangeFrame(int itimeclip);//��Σ�ʱ��Ƭ
	
	//�߼����
	int IsWin();
	void Fail();
	void Fail_Wait();

	//��ͼ�л�
	void ChangeMap();
	//������
	void CodeErr(int i);
	//�˵�����
	void ShowMenu(MYANIOBJ & bmobj);

	GAMEMAP();
	~GAMEMAP();
	
	//data
	int iMatch;
	int iLife;
	int iGameState;

	struct MapObject MapArray[MAX_MAP_OBJECT];
	int iMapObjNum;

	struct MapObject MapBkArray[MAX_MAP_OBJECT];
	int iMapBkObjNum;

	struct ROLE MapEnemyArray[MAX_MAP_OBJECT];
	int iMapEnemyCursor;

	struct MapObject MapCoinArray[MAX_MAP_OBJECT];
	int iCoinNum;
	//MAP DATA
	int iNextMap;

	// role data
	int iMoney;
	int iAttack;	//������ʽ

	//view
	int viewx;
	int viewy;
	int iViewState;

	//��ͼ����
	struct MAPINFO mapinfo;

	//frame control
	int ienemyframe;
	int ibkobjframe;

	//FIRE
	struct ROLE FireArray[MAX_MAP_OBJECT];
	int iFireNum;
	int iTimeFire;//�����ӵ���ʱ����
	int iBeginFire;

	//bomb
	struct MapObject BombArray[MAX_MAP_OBJECT];
	int iBombNum;

	//����������ʾ
	char AttackName[20];
	int iAttackLife;
	int iAttackMaxLife;

	//�˵�����
	int iMenu;

	//��Ļ����
	int iScreenScale;
};

#endif