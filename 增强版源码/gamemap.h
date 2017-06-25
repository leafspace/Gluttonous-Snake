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
	int iframemax;//最大帧数
	int show;	//是否显示
};

struct ROLE
{
	int x;
	int y;
	int w;
	int h;
	int id;
	int iframe;
	int iframemax;//最大帧数

	//移动部分
	int xleft;//左界限	
	int xright;//右界限
	int movex;

	//人物属性
	int health;

	int show;	//是否显示
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
	//入参,物体索引图
	void Show(MYANIOBJ & bmobj);
	//入参,背景物体索引图
	void ShowBkObj(MYANIOBJ & bmobj);
	//入参,动画物体索引图
	void ShowAniObj(MYANIOBJ & bmobj);
	void ShowInfo(HDC h);
	void ShowOther(HDC h);

	//键盘处理
	int KeyProc(int iKey);
	void KeyUpProc(int iKey);

	void MoveView();
	//设置视图起始坐标
	void SetView(int x);
	void SetViewState(int i);
	void SetGameState(int i);
	//hittest
	int RoleCanMove(int xoff, int yoff);
	int CheckRole();
	int CheckAni(int itimeclip);
	void ClearEnemy(int i);
	void ClearCoin(int i);
	//动画帧控制
	void ChangeFrame(int itimeclip);//入参，时间片
	
	//逻辑检测
	int IsWin();
	void Fail();
	void Fail_Wait();

	//地图切换
	void ChangeMap();
	//错误检查
	void CodeErr(int i);
	//菜单控制
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
	int iAttack;	//攻击方式

	//view
	int viewx;
	int viewy;
	int iViewState;

	//地图数据
	struct MAPINFO mapinfo;

	//frame control
	int ienemyframe;
	int ibkobjframe;

	//FIRE
	struct ROLE FireArray[MAX_MAP_OBJECT];
	int iFireNum;
	int iTimeFire;//两个子弹的时间间隔
	int iBeginFire;

	//bomb
	struct MapObject BombArray[MAX_MAP_OBJECT];
	int iBombNum;

	//攻击对象提示
	char AttackName[20];
	int iAttackLife;
	int iAttackMaxLife;

	//菜单部分
	int iMenu;

	//屏幕缩放
	int iScreenScale;
};

#endif