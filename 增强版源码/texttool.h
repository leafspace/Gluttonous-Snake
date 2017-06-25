#ifndef __TEXTTOOL
#define __TEXTTOOL

#define TEXT_BOTTOM_RIGHT 0
#define TEXT_BOTTOM_CENTER 1
#define TEXT_CENTER 2
#define TEXT_USER 3

#define TC_BLACK RGB(0,0,0)
#define TC_BLUE RGB(0,0,255)
#define TC_WHITE RGB(255,255,255)
#define TC_YELLOW_0 RGB(207,193,0)

//////////////////// FONT ///////////////////

class MYFONT
{
public:
	MYFONT();
	~MYFONT();

	void SetDevice(HDC h);
	void SelectFont(int i);
	void SelectOldFont();
	void ShowText(int x,int y,char *p);
	
	void SetColor(COLORREF cbk, COLORREF ctext);
	void SelectColor(COLORREF cbk, COLORREF ctext);

	//dc
	HDC hdc;
	//font
	HFONT hf[5];
	HFONT oldhf;
	//color
	COLORREF c1;
	COLORREF c2;
};

#endif
