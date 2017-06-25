// Finishing codefans.net


#include "stdafx.h"
#include "bitmaptool.h"
#include "gamemap.h"

extern GAMEMAP gamemap;

extern FILEREPORT f1;

MYBITMAP::MYBITMAP()
{

}

MYBITMAP::~MYBITMAP()
{
	DeleteObject(hBm);
}

void MYBITMAP::Init(HINSTANCE hInstance,int iResource,int row,int col)
{
	BITMAP bm;
	inum=row;
	jnum=col;

	hBm=LoadBitmap(hInstance,MAKEINTRESOURCE(iResource));
	GetObject(hBm,sizeof(BITMAP),&bm);

	width=bm.bmWidth/inum;
	height=bm.bmHeight/jnum;
}

void MYBITMAP::SetDevice(HDC hdest,HDC hsrc,int wwin,int hwin)
{
	hdcdest=hdest;
	hdcsrc=hsrc;
	screenwidth=wwin;
	screenheight=hwin;
}

void MYBITMAP::Show(int x,int y)
{
	xpos=x;
	ypos=y;
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,width,height,hdcsrc,0,0,SRCCOPY);	
}

void MYBITMAP::ShowCenter(int y)
{
	xpos=(screenwidth-width)/2;
	ypos=y;

	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,width,height,hdcsrc,0,0,SRCCOPY);	
}

void MYBITMAP::ShowLoop(int left,int top,int right,int bottom,int iframe)
{
	int i,j;

	SelectObject(hdcsrc,hBm);
	for(j=top;j<bottom;j+=height)
	{
		for(i=left;i<right;i+=width)
		{
			BitBlt(hdcdest,i,j,width,height,hdcsrc,iframe*width,0,SRCCOPY);	
		}
	}
}

void MYBITMAP::ShowNoBack(int x,int y,int iFrame)
{
	xpos=x;
	ypos=y;
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,width,height/2,hdcsrc,iFrame*width,height/2,SRCAND);	
	BitBlt(hdcdest,xpos,ypos,width,height/2,hdcsrc,iFrame*width,0,SRCPAINT);		
}


void MYBITMAP::ShowNoBackLoop(int x,int y,int iFrame,int iNum)
{
	int i;
	xpos=x;
	ypos=y;
	SelectObject(hdcsrc,hBm);
	for(i=0;i<iNum;i++)
	{
		BitBlt(hdcdest,xpos+i*width,ypos,width,height/2,hdcsrc,iFrame*width,height/2,SRCAND);	
		BitBlt(hdcdest,xpos+i*width,ypos,width,height/2,hdcsrc,iFrame*width,0,SRCPAINT);		
	}
}

void MYBITMAP::ShowAni()
{
/*	if(!iStartAni)
		return;

	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,width,height/2,hdcsrc,framenow*width,height/2,SRCAND);	
	BitBlt(hdcdest,xpos,ypos,width,height/2,hdcsrc,framenow*width,0,SRCPAINT);		

	framenow++;
	//播放结束
	if(framenow>=inum)
		iStartAni=0;
		*/
}
void MYBITMAP::SetAni(int x,int y)
{
	xpos=x;
	ypos=y;
	/*
	framenow=0;
	iStartAni=1;
	*/
}


void MYBITMAP::SetPos(int istyle,int x,int y)
{
	switch(istyle)
	{
	case BM_CENTER:
		xpos=(screenwidth-width)/2;
		ypos=y;
		break;
	case BM_USER:
		xpos=x;
		ypos=y;
		break;
	}
}

void MYBITMAP::Draw(DWORD dwRop)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,width,height,hdcsrc,0,0,dwRop);	
}

void MYBITMAP::Stretch(int x,int y)
{
	SelectObject(hdcsrc,hBm);
	StretchBlt(hdcdest,xpos,ypos,width*x,height*y,
		hdcsrc,0,0,width,height,
		SRCCOPY);	
}

void MYBITMAP::Stretch(int x,int y,int id)
{
	SelectObject(hdcsrc,hBm);
	StretchBlt(hdcdest,xpos,ypos,width*x,height*y,
		hdcsrc,0,id*height,
		width,height,
		SRCCOPY);	
}


///////////////////////// bitmap object animation//////////////////
MYBKSKY::MYBKSKY()
{
	xseparate=0;
}

MYBKSKY::~MYBKSKY()
{}

void MYBKSKY::MoveTo(int x,int y)
{
	xpos=x;
	ypos=y;
}

void MYBKSKY::MoveRoll(int x)
{
	xseparate+=x;
	xseparate%=width;
	
	if(xseparate<0)
	{
		xseparate=width;
	}	
}

void MYBKSKY::DrawRoll()
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,xpos,ypos,
		width-xseparate,height,
		hdcsrc,xseparate,0,SRCCOPY);
	
	BitBlt(hdcdest,xpos+width-xseparate,ypos,
		xseparate,height,
		hdcsrc,0,0,SRCCOPY);	
}

void MYBKSKY::DrawRollStretch(int x,int y)
{
	SelectObject(hdcsrc,hBm);
	
	StretchBlt(hdcdest,xpos,ypos,
		(width-xseparate)*x,height*y,
		hdcsrc,xseparate,0,
		width-xseparate,height,
		SRCCOPY);
	
	StretchBlt(hdcdest,xpos+(width-xseparate)*x,ypos,
		xseparate*x,height*y,
		hdcsrc,0,0,
		xseparate,height,
		SRCCOPY);	
}

void MYBKSKY::DrawRollStretch(int x,int y, int id)
{
	SelectObject(hdcsrc,hBm);
	
	StretchBlt(hdcdest,xpos,ypos,
		(width-xseparate)*x,height*y,
		hdcsrc,xseparate,id*height,
		width-xseparate,height,
		SRCCOPY);
	
	StretchBlt(hdcdest,xpos+(width-xseparate)*x,ypos,
		xseparate*x,height*y,
		hdcsrc,0,id*height,
		xseparate,height,
		SRCCOPY);	
}

///////////////////////// bitmap role//////////////////
MYROLE::MYROLE()
{
}

MYROLE::~MYROLE()
{}

void MYROLE::MoveTo(int x,int y)
{
	xpos=x;
	ypos=y;
}

void MYROLE::MoveOffset(int x,int y)
{
	if(x==0 && y==0)
		return;

	if(!gamemap.RoleCanMove(x,y))
		return;

	xpos+=x;
	ypos+=y;
	
	if(xpos<minx)
		xpos=minx;

	if(xpos>maxx)
		xpos=maxx;	
}

void MYROLE::MoveStepTo(int x,int y)
{
	if(xpos<x)
		xpos+=ROLE_STEP;

	if(ypos<y)
		ypos+=8;

	if(ypos>y)
		ypos=y;
}

// 去背显示
void MYROLE::Draw()
{
	if(iAniBegin)
	{
		PlayAni();	
	}
	else
	{
		SelectObject(hdcsrc,hBm);
		BitBlt(hdcdest,xpos,ypos,
			width,height/2,
			hdcsrc,iFrame*width,height/2,SRCAND);
		
		BitBlt(hdcdest,xpos,ypos,
			width,height/2,
			hdcsrc,iFrame*width,0,SRCPAINT);	
	}
}

void MYROLE::Draw(int x,int y,int frame)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,
		width,height/2,
		hdcsrc,frame*width,height/2,SRCAND);
	
	BitBlt(hdcdest,x,y,
		width,height/2,
		hdcsrc,frame*width,0,SRCPAINT);	
}

void MYROLE::InitRole(int xleft,int xright)
{
	iFrame=0;
	iState=0;

	maxx=xright;
	minx=xleft;

	jumpheight=0;

	movex=0;
	movey=0;

	jumpx=0;

	idirec=0;

	iAniBegin=0;
}

void MYROLE::SetLimit(int xleft,int xright)
{
	maxx=xright;
	minx=xleft;
}

void MYROLE::Move()
{
	if(0 == movey)
	{
		//水平移动
		MoveOffset(movex, 0);
	}
	else
	{
		//跳动		
		MoveOffset(jumpx, 0);
		MoveOffset(0, movey);
	}

	//帧控制
	if(movex<0 && iFrame<3)
	{
		iFrame=3;
	}
	
	if(movex>0 && iFrame>=3)
	{
		iFrame=0;
	}

	if(movex!=0)
	{
		if(0==idirec)
			iFrame=1-iFrame;
		else
			iFrame=7-iFrame;		
	}
	if(movey!=0)
	{
		iFrame=idirec*3;
	}

	
	if(movey<0)
	{
		//up
		jumpheight+=(-movey);
		
		//根据重力影响,加速度减慢
		if(movey<-1)
		{
			movey++;
		}

		//到达顶点后向下落
		if(jumpheight >= JUMP_HEIGHT * 32)
		{	
			jumpheight =  JUMP_HEIGHT * 32;
			movey=4;			
		}
	}
	else if(movey>0)
	{
		jumpheight -= movey;

		//根据重力影响,加速度增大
		movey++;			
	}
}

void MYROLE::Jump()
{
	//上升过程
	MoveOffset(0, -4);

}


void MYROLE::ChangeFrame()
{

}

void MYROLE::SetState(int i)
{	
	iState=i;
}

void MYROLE::SetAni(int istyle)
{
	iAniStyle=istyle;

	iparam1=0;
	iAniBegin=1;
}
//是否正在播放动画
int  MYROLE::IsInAni()
{
	return iAniBegin;
}

void MYROLE::PlayAni()
{
	switch(iAniStyle)
	{
	case ROLE_ANI_DOWN:
		if(iparam1>31)
		{
			break;			
		}
		//人物下降动画
		SelectObject(hdcsrc,hBm);
		BitBlt(hdcdest,
			xpos,ypos+iparam1,
			width,height/2-iparam1,
			hdcsrc,
			iFrame*width,height/2,SRCAND);
		
		BitBlt(hdcdest,
			xpos,ypos+iparam1,
			width,height/2-iparam1,
			hdcsrc,
			iFrame*width,0,SRCPAINT);	
		
		iparam1++;		
		break;
	
	case ROLE_ANI_UP:
		if(iparam1>31)
		{
			break;			
		}
		//人物上升动画
		SelectObject(hdcsrc,hBm);
		BitBlt(hdcdest,
			xpos,ypos+32-iparam1,
			width,iparam1,
			hdcsrc,
			iFrame*width,height/2,SRCAND);
		
		BitBlt(hdcdest,
			xpos,ypos+32-iparam1,
			width,iparam1,
			hdcsrc,
			iFrame*width,0,SRCPAINT);	
		
		iparam1++;
		if(iparam1>31)
		{
			iAniBegin=0;	//动画结束
		}
	default:
		break;
	}	
}
/////////////////////////////////// my animation /////////////////////

MYANIOBJ::MYANIOBJ()
{
	memset(wlist, 0, sizeof(wlist));
	memset(hlist, 0, sizeof(hlist));
	memset(ylist, 0, sizeof(ylist));	
	
	iframeplay=0;
}

MYANIOBJ::~MYANIOBJ()
{}

void MYANIOBJ::DrawItem(int x,int y,int id,int iframe)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id]+hlist[id],SRCAND);	

	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id],SRCPAINT);	
}

void MYANIOBJ::DrawItemNoMask(int x,int y,int id,int iframe)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id],SRCCOPY);	
}

void MYANIOBJ::DrawItemNoMaskWidth(int x,int y,int id,int w,int iframe)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,w,hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id],SRCCOPY);	
}

void MYANIOBJ::PlayItem(int x,int y,int id)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframeplay*wlist[id],ylist[id]+hlist[id],SRCAND);	

	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframeplay*wlist[id],ylist[id],SRCPAINT);	

	iframeplay=(iframeplay+1)%2;
}

void MYANIOBJ::InitAniList(int *pw,int *ph,int inum,int ismask)
{
	int i;
	memcpy(wlist, pw, inum*sizeof(int));
	memcpy(hlist, ph, inum*sizeof(int));

	ylist[0]=0;
	for(i=1;i<inum;i++)
	{
		//如果是mask，两张图片的间隔高度要乘以2
		ylist[i]=ylist[i-1]+hlist[i-1]*(ismask+1);
	}
	
}

void MYANIOBJ::InitAniList(int style,int a, int b)
{
	int i;

	switch(style)
	{
	case BM_SQUARE:
		// a : 数量, b: 边长
		for(i=0;i<a;i++)
		{
			wlist[i]=b;
			hlist[i]=b;
			ylist[i]=i*b;
		}
		break;

	default:
		break;
	}	
}

/////////////////////////////////// my animation magic/////////////////////

MYANIMAGIC::MYANIMAGIC()
{
	memset(wlist, 0, sizeof(wlist));
	memset(hlist, 0, sizeof(hlist));
	memset(ylist, 0, sizeof(ylist));	
}

MYANIMAGIC::~MYANIMAGIC()
{}

void MYANIMAGIC::DrawItem(int x,int y,int id,int iframe)
{
	SelectObject(hdcsrc,hBm);
	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id]+hlist[id],SRCAND);	

	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id],SRCPAINT);
		/*
	//先在临时DC上画出效果总图
	BitBlt(hdctemp,0,0,wlist[id],hlist[id],
		hdcsrc,
		0,ylist[id],SRCCOPY);	

	//用MASK图作裁剪
	BitBlt(hdctemp,0,0,wlist[id],hlist[id],
		hdcsrc,
		iframe*wlist[id],ylist[id],SRCPAINT);

	//最后画到屏幕上
	BitBlt(hdcdest,x,y,wlist[id],hlist[id],
		hdctemp,
		0,0,SRCPAINT);
		*/
}

//带MASK
void MYANIMAGIC::InitAniList(int *pw,int *ph,int inum)
{
	int i;
	memcpy(wlist, pw, inum*sizeof(int));
	memcpy(hlist, ph, inum*sizeof(int));

	ylist[0]=0;
	for(i=1;i<inum;i++)
	{
		ylist[i]=ylist[i-1]+hlist[i-1]*2;
	}
	
}

void MYANIMAGIC::SetDevice(HDC hdest, HDC hsrc, HDC htemp)
{
	hdcdest=hdest;
	hdcsrc=hsrc;
	hdctemp=htemp;
}