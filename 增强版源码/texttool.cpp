// Finishing codefans.net


#include "stdafx.h"
#include "texttool.h"

///////////////////////// FONT ///////////////////

MYFONT::MYFONT()
{
	char temp[10]={0};
	int i;

	for(i=0;i<=sizeof(hf)/sizeof(HFONT);i++)
	{
		sprintf(temp,"black%d",i);
		hf[i]=CreateFont(10*i,0,0,0,FW_NORMAL,
			0,0,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,temp);
	}

}

MYFONT::~MYFONT()
{

}

void MYFONT::SetDevice(HDC h)
{
	hdc=h;

	//ÉèÖÃÄ¬ÈÏ×ÖÌå
	oldhf=(HFONT)SelectObject(hdc,hf[0]);
	SelectObject(hdc,oldhf);
}

void MYFONT::SelectFont(int i)
{
	SelectObject(hdc,hf[i]);
}

void MYFONT::SelectOldFont()
{
	SelectObject(hdc,oldhf);
}

void MYFONT::ShowText(int x,int y,char *p)
{
	TextOut(hdc,x,y,p,strlen(p));
}

void MYFONT::SetColor(COLORREF cbk, COLORREF ctext)
{
	c1=cbk;
	c2=ctext;
}

void MYFONT::SelectColor(COLORREF cbk, COLORREF ctext)
{
	SetBkColor(hdc,cbk);
	SetTextColor(hdc,ctext);
}