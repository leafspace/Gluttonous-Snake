#ifndef __BITMAPTOOL
#define __BITMAPTOOL

#include "filereport.h"


#define BM_BOTTOM_RIGHT 0
#define BM_BOTTOM_CENTER 1
#define BM_CENTER 2
#define BM_USER 3

//
class MYBITMAP
{
public:
	MYBITMAP();
	~MYBITMAP();
	//初始化
	void Init(HINSTANCE hInstance,int iResource,int row,int col);
	void SetDevice(HDC hdest,HDC hsrc,int wwin,int hwin);
	//
	void SetPos(int istyle,int x,int y);
	
	//显示	
	void Draw(DWORD dwRop);
	void Stretch(int x,int y);
	void Stretch(int x,int y,int id);
	void Show(int x,int y);
	void ShowCenter(int y);
	void ShowLoop(int left,int top,int right,int bottom,int iframe);
	void ShowNoBack(int x,int y,int iFrame);
	void ShowNoBackLoop(int x,int y,int iFrame,int iNum);
	
	//动画播放
	void ShowAni();
	void SetAni(int x,int y);
	
//	FILEREPORT f;
	HBITMAP hBm;
public:
	//按照行列平均分成几个
	int inum;
	int jnum;

	int width;
	int height;
	int screenwidth;
	int screenheight;
	HDC hdcdest;
	HDC hdcsrc;	

	//当前位置
	int xpos;
	int ypos;

	int iStartAni;
};

class MYBKSKY:public MYBITMAP
{
public:
	MYBKSKY();
	~MYBKSKY();

	//show
	void DrawRoll(); //循环补空
	void DrawRollStretch(int x,int y);
	void DrawRollStretch(int x,int y,int id);
	void MoveTo(int x,int y);
	void MoveRoll(int x);

	//data
	int xseparate;
};

#define BM_SQUARE 1

class MYANIOBJ:public MYBITMAP
{
public:
	MYANIOBJ();
	~MYANIOBJ();

	//init list
	void InitAniList(int *pw,int *ph,int inum,int ismask);
	void InitAniList(int style,int a,int b);

	//show
	void DrawItem(int x,int y,int id,int iframe);
	void DrawItemNoMask(int x,int y,int id,int iframe);
	//指定宽度
	void DrawItemNoMaskWidth(int x,int y,int id,int w,int iframe);
	//自动播放
	void PlayItem(int x,int y,int id);

	//
	int wlist[20];
	int hlist[20];
	int ylist[20];

	int iframeplay;
};

class MYANIMAGIC:public MYBITMAP
{
public:
	MYANIMAGIC();
	~MYANIMAGIC();

	//init list
	void InitAniList(int *pw,int *ph,int inum);
	void SetDevice(HDC hdest,HDC hsrc,HDC htemp);

	//show
	void DrawItem(int x,int y,int id,int iframe);
	//
	int wlist[20];
	int hlist[20];
	int ylist[20];

	HDC hdctemp;
};

class MYROLE:public MYBITMAP
{
public:
	MYROLE();
	~MYROLE();
	//init
	void InitRole(int xleft, int xright);
	void SetLimit(int xleft, int xright);
	//show
	void Draw();
	void Draw(int x,int y,int iframe);
	void ChangeFrame();
	void SetState(int i);
	void Move();
	void Jump();
	//ANI
	void MoveTo(int x,int y);
	void MoveOffset(int x,int y);
	void MoveStepTo(int x,int y);//向目的地移动单位距离
	
	void PlayAni();//播放一段动画
	void SetAni(int istyle);
	int IsInAni();//是否正在播放动画

	//data
	int iState;
	int iFrame;
	int minx;
	int maxx;

	//move
	int movex;
	int movey;

	// jump
	int jumpheight;
	int jumpx;

	//方向
	int idirec;

	//动画
	int iAniBegin;
	int iparam1;
	int iAniStyle;
};

#endif