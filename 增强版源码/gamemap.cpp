// Finishing codefans.net


#include "stdafx.h"
#include "gamemap.h"
#include "myclock.h"
#include "tool01.h"
#include "texttool.h"

extern int wwin,hwin;
extern HWND hWndMain;

//基本图像
extern MYANIOBJ bmMap;

//天空背景
extern MYBKSKY bmSky;

//角色
extern MYROLE rmain;

//计时器
extern MYCLOCK c1;

extern MYANIMAGIC bmMagic;

extern struct MAPINFO allmapinfo[];

extern struct ROLE gl_enemy_normal;

//extern FILEREPORT f1;

GAMEMAP::GAMEMAP()
{
	iScreenScale=0;
	Init();
}

GAMEMAP::~GAMEMAP()
{
}

int GAMEMAP::LoadMap()
{
	FILE *fp;
	char temp[50]={0};
	int find=0;
	int i;
	
	
	memset(MapArray,0,sizeof(MapArray));
	iMapObjNum=0;
	
	memset(MapBkArray,0,sizeof(MapBkArray));
	iMapBkObjNum=0;
	
	memset(MapEnemyArray,0,sizeof(MapEnemyArray));
	
	memset(MapCoinArray,0,sizeof(MapCoinArray));
	iCoinNum=0;

	fp=fopen(PATH_MAP,"r");
	if(!fp)
	{
		return 0;
	}

	while(!find && !feof(fp))
	{
		FGetLine(temp,fp);
		if(temp[0]=='*' && temp[1]=='0'+iMatch)
		{
			find=1;
		}
	}
	if(!find)
	{
		return 0;
	}

	//找到了某一关的地图数据
	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		//map data
		sscanf(temp,"%d %d %d %d %d",
			&MapArray[i].x,
			&MapArray[i].y,
			&MapArray[i].w,
			&MapArray[i].h,
			&MapArray[i].id);			
		MapArray[i].show=0;
		iMapObjNum++;
		i++;
		FGetLineJumpCom(temp,fp);	
	}

	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		sscanf(temp,"%d %d %d %d %d",
			&MapBkArray[i].x,
			&MapBkArray[i].y,
			&MapBkArray[i].w,
			&MapBkArray[i].h,
			&MapBkArray[i].id);			
		MapBkArray[i].show=0;
		MapBkArray[i].iframe=0;
		iMapBkObjNum++;

		i++;
		FGetLineJumpCom(temp,fp);	
	}


	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		sscanf(temp,"%d %d %d %d %d %d %d",
			&MapEnemyArray[i].x,
			&MapEnemyArray[i].y,
			&MapEnemyArray[i].w,
			&MapEnemyArray[i].h,
			&MapEnemyArray[i].id,
			&MapEnemyArray[i].xleft,
			&MapEnemyArray[i].xright);
		
		//动画元件，使用绝对坐标
		MapEnemyArray[i].x*=32;
		MapEnemyArray[i].y*=32;
		MapEnemyArray[i].w*=32;
		MapEnemyArray[i].h*=32;
		MapEnemyArray[i].xleft*=32;
		MapEnemyArray[i].xright*=32;
		MapEnemyArray[i].show=1;
		MapEnemyArray[i].movex=-ENEMY_STEP_X;
		MapEnemyArray[i].iframe=0;
		MapEnemyArray[i].iframemax=2;
		
		//设置生命值
		switch(MapEnemyArray[i].id)
		{
		case ID_ANI_BOSS_HOUSE:
			MapEnemyArray[i].health=BOSS_HEALTH;
			break;

		case ID_ANI_BOSS_HOUSE_A:
			MapEnemyArray[i].health=BOSS_A_HEALTH;
			break;

		default:
			MapEnemyArray[i].health=1;
			break;
		}

		//将BOSS存储在数组的后半段
		if ( i<BOSS_CURSOR
			 && (  MapEnemyArray[i].id == ID_ANI_BOSS_HOUSE
			    || MapEnemyArray[i].id == ID_ANI_BOSS_HOUSE_A) )
		{
			//move data to BOSS_CURSOR
			MapEnemyArray[BOSS_CURSOR]=MapEnemyArray[i];
			memset(&MapEnemyArray[i],0,sizeof(MapEnemyArray[i]));			
			i=BOSS_CURSOR;
		}

		i++;
		FGetLineJumpCom(temp,fp);	
	}

	i=0;
	FGetLineJumpCom(temp,fp);	
	while(temp[0]!='#' && !feof(fp))
	{
		sscanf(temp,"%d %d %d %d %d",
			&MapCoinArray[i].x,
			&MapCoinArray[i].y,
			&MapCoinArray[i].w,
			&MapCoinArray[i].h,
			&MapCoinArray[i].id);			
		MapCoinArray[i].show=1;
		MapCoinArray[i].iframe=0;
		//索引坐标转化为绝对坐标
		MapCoinArray[i].x*=32;
		MapCoinArray[i].y*=32;
		//设置这个动画元件的最大帧
		switch(MapCoinArray[i].id)
		{
		case ID_ANI_COIN:
			MapCoinArray[i].iframemax=4;
			break;

		default:
			MapCoinArray[i].iframemax=2;
			break;
		}

		i++;
		iCoinNum++;

		FGetLineJumpCom(temp,fp);	
	}

	fclose(fp);
	return 1;
}

void GAMEMAP::Init()
{	
	iGameState=GAME_PRE;
	iMatch=0;
	iLife=3;
	iMoney=0;
	//攻击种类
	iAttack=ATTACK_NORMAL;

	iMenu=0;
	viewx=0;
	InitMatch();
}

void GAMEMAP::InitMatch()
{
	memset(MapArray,0,sizeof(MapArray));
	memset(MapBkArray,0,sizeof(MapBkArray));
	memset(MapEnemyArray,0,sizeof(MapEnemyArray));
	memset(MapCoinArray,0,sizeof(MapCoinArray));
	
	memset(FireArray,0,sizeof(FireArray));
	memset(BombArray,0,sizeof(BombArray));

	iMapObjNum=0;
	iMapBkObjNum=0;
	iBombNum=0;
	iCoinNum=0;

	//设置视图坐标
	viewy=0;

	ienemyframe=0;

	iFireNum=0;

	iTimeFire=0;
	iBeginFire=0;
	
	ibkobjframe=0;

	//获取地图信息
	mapinfo=allmapinfo[iMatch];

	//初始化攻击对象提示
	memset(AttackName,0,sizeof(AttackName));
	iAttackLife=0;
	iAttackMaxLife=0;

	iMapEnemyCursor=0;
}

//显示地图
void GAMEMAP::Show(MYANIOBJ & bmobj)
{
	int i;
	int xstart,ystart;
	int j,k;
	
	for(i=0;i<iMapObjNum;i++)
	{
		ystart=MapArray[i].y*32;

		switch(MapArray[i].id)
		{
		//进 出的下水道
		case ID_MAP_PUMP_IN:
		case ID_MAP_PUMP_OUT:
			xstart=MapArray[i].x*32;
			bmobj.DrawItemNoMask(xstart, ystart, MapArray[i].id, 0);			
			break;
			
		default:
			for(j=0;j<MapArray[i].h;j++)
			{
				xstart=MapArray[i].x*32;
				for(k=0;k<MapArray[i].w;k++)
				{
					bmobj.DrawItemNoMask(xstart, ystart, MapArray[i].id, 0);			
					xstart+=32;
				}
				ystart+=32;				
			} // end of for
			break;
		}//end of switch
	}
}

void GAMEMAP::ChangeMap()
{
	viewx=0;
	//	
	iMatch=mapinfo.iSubMap;
	InitMatch();						
	rmain.SetPos(BM_USER,mapinfo.xReturnPoint*32,mapinfo.yReturnPoint*32);
	rmain.InitRole(0,GAMEW*32*MAX_PAGE-32);	
	//设定视图位置
	if(rmain.xpos - viewx > 150)
	{
		SetView(mapinfo.xReturnPoint*32-32);//往左让一格
		if(viewx>(mapinfo.viewmax-1)*GAMEW*32)
			viewx=(mapinfo.viewmax-1)*GAMEW*32;
	}
	//设定人物活动范围
	rmain.SetLimit(viewx, GAMEW*32*MAX_PAGE);
	//设定背景图片坐标
	bmSky.SetPos(BM_USER,viewx,0);
	LoadMap();
}
//根据人物与视图的位置关系决定是否移动视图
int GAMEMAP::KeyProc(int iKey)
{
	int i;


	switch(iGameState)
	{
	case GAME_PRE:
		switch(iKey)
		{
		case 0xd:
			//enter
			switch(iMenu)
			{
			case 0:
				c1.ReStart(TIME_GAME_IN_PRE); //停顿两秒
				iGameState=GAME_IN_PRE;
				break;
				
			case 1:
				SetGameState(GAME_HELP);
				break;
			}
			break;
			
		case VK_UP:
			iMenu=(iMenu+1)%2;
			break;

		case VK_DOWN:
			iMenu=(iMenu+1)%2;
			break;
		}

		return 1;//重绘

	case GAME_HELP:
		switch(iKey)
		{
		case 0xd:	//enter
			SetGameState(GAME_PRE);
			break;			
		}
		return 1;

	case GAME_IN:
		//如果人物正在播放动画，拒绝键盘响应
		if(rmain.IsInAni())
		{
			break;
		}

		switch(iKey)
		{
		case VK_RIGHT:	
			if(rmain.movey!=0)
			{
				rmain.jumpx=4;
			}
			rmain.movex=4;
			rmain.idirec=0;
			break;
			
		case VK_LEFT:
			if(rmain.movey!=0)
			{
				rmain.jumpx=-4;
			}
			rmain.movex=-4;
			rmain.idirec=1;//朝左
			break;

		case VK_DOWN:
			for(i=0;i<iMapObjNum;i++)
			{
				//下方线
				if( LINE_IN_LINE(rmain.xpos,
					rmain.ypos+32,
					32,
					MapArray[i].x*32,
					MapArray[i].y*32,
					MapArray[i].w*32)
					)
				{				
					if(MapArray[i].id == ID_MAP_PUMP_IN)
					{
						//进入地图
						rmain.SetAni(ROLE_ANI_DOWN);
						iGameState=GAME_PUMP_IN;
						c1.ReStart(TIME_GAME_PUMP_WAIT);
					}
				}
			}
			break;
			
		case KEY_X: //跳
			if(rmain.movey!=0)
				break;
			rmain.movey=-SPEED_JUMP;
			rmain.jumpx=rmain.movex;//解决x键和方向键的响应问题
			break;
			
		case KEY_Z: //FIRE
			if(iBeginFire)
				break;
			iTimeFire=0;
			iBeginFire=1;		
			break;

		case 0x7a://f11
			iAttack=(iAttack+1)%ATTACK_MAX_TYPE;
			break;

		case 0x7b://f12
			//直接通关
			rmain.xpos = MAX_PAGE*GAMEW*32;
			break;
		}
		break;
	}

	return 0;
}

void GAMEMAP::SetViewState(int i)
{
	iViewState=i;
}
void GAMEMAP::SetGameState(int i)
{
	iGameState=i;
}

void GAMEMAP::SetView(int x)
{
	viewx=x;
}
//移动视图
//视图不能往右移
void GAMEMAP::MoveView()
{
	//只有一屏宽，不移动视图
	if(mapinfo.viewmax == 1)
		return;
	
	if(rmain.xpos - viewx > 150)
	{
		viewx+=ROLE_STEP;	
		if(viewx>(mapinfo.viewmax-1)*GAMEW*32)
			viewx=(mapinfo.viewmax-1)*GAMEW*32;
		
		//设置人物移动的最大范围
		rmain.SetLimit(viewx, GAMEW*32*MAX_PAGE);
	}

	bmSky.SetPos(BM_USER,viewx,0);
}

void GAMEMAP::ShowBkObj(MYANIOBJ & bmobj)
{
	int i;
	int xstart,ystart;

	for(i=0;i<iMapBkObjNum;i++)
	{
		ystart=MapBkArray[i].y*32;
		xstart=MapBkArray[i].x*32;
		bmobj.DrawItem(xstart,ystart,MapBkArray[i].id,ibkobjframe);	
	}
}


void GAMEMAP::ShowInfo(HDC h)
{
	char temp[50]={0};

	SetTextColor(h, TC_WHITE);
	SetBkColor(h, TC_BLACK);

	sprintf(temp, "LIFE  : %d",iLife);
	TextOut(h, 220,100,temp,strlen(temp));

	sprintf(temp, "WORLD : %d",iMatch+1);
	TextOut(h, 220,130,temp,strlen(temp));
}

void GAMEMAP::ShowOther(HDC h)
{
	char temp[50]={0};
	int xstart;

	//show money
	sprintf(temp,"MONEY: %d",iMoney);
	TextOut(h,viewx+20,20,temp,strlen(temp));

	//显示攻击对象生命值
	if(iAttackLife)
	{
		TextOut(h,viewx+ATTACK_TO_TEXT_X,
			ATTACK_TO_TEXT_Y,AttackName,strlen(AttackName));
		
		//画生命条
		xstart=viewx+ATTACK_TO_X-iAttackMaxLife*10;

		bmMap.DrawItemNoMaskWidth(xstart-1, ATTACK_TO_Y-1,ID_MAP_HEALTH_BK,
			iAttackMaxLife*BMP_WIDTH_HEALTH, 0);
	
		bmMap.DrawItemNoMaskWidth(xstart, ATTACK_TO_Y,ID_MAP_HEALTH,
			iAttackLife*BMP_WIDTH_HEALTH, 0);
	}
}

void GAMEMAP::ShowAniObj(MYANIOBJ & bmobj)
{
	int i;
	int xstart,ystart;

	//显示金币,和其他物品
	for(i=0;i<iCoinNum;i++)
	{
		ystart=MapCoinArray[i].y;
		xstart=MapCoinArray[i].x;
		bmobj.DrawItem(xstart,ystart,MapCoinArray[i].id, MapCoinArray[i].iframe);	
	}

	//显示敌人
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if (MapEnemyArray[i].show)
		{
			bmobj.DrawItem(MapEnemyArray[i].x,MapEnemyArray[i].y,
				MapEnemyArray[i].id,MapEnemyArray[i].iframe);	
		}
	}

	//显示子弹，魔法攻击
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if (FireArray[i].show)
		{
			ystart=FireArray[i].y;
			xstart=FireArray[i].x;
			
			switch(FireArray[i].id)
			{
			case ID_ANI_FIRE:
				bmobj.DrawItem(xstart,ystart,FireArray[i].id,FireArray[i].iframe);
				break;

			case ID_ANI_FIRE_MAGIC:
				bmMagic.DrawItem(xstart,ystart, 0, FireArray[i].iframe);
				break;

			default:
				bmobj.DrawItem(xstart,ystart,FireArray[i].id,FireArray[i].iframe);
				break;
			}
		}
	}


	//显示爆炸效果
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if (BombArray[i].show)
		{
			ystart=BombArray[i].y;
			xstart=BombArray[i].x;
			bmobj.DrawItem(xstart,ystart,BombArray[i].id, BombArray[i].iframe);	
		}
	}
}

// 返回0，不能走；1，能走
int GAMEMAP::RoleCanMove(int xoff, int yoff)
{
	int canmove=1;
	int i;
	
	
	for(i=0;i<iMapObjNum;i++)
	{
		if( RECT_HIT_RECT(rmain.xpos+xoff,
			rmain.ypos+yoff,
			32,
			32,
			MapArray[i].x*32,
			MapArray[i].y*32,
			MapArray[i].w*32,
			MapArray[i].h*32)
			)
		{
			canmove=0;
			if(yoff<0)
			{
				//碰到了上面的物体要向下落
				rmain.movey=1;
			}
			if(yoff>0)
			{
				//碰到了下面的物体,要停止下落
				rmain.jumpheight=0;
				rmain.movey=0;
				rmain.ypos=MapArray[i].y*32-32;//纵坐标要修正
			}
			break;
		}
	}
	
	return canmove;
}

//检查人物水平移动过程中,是否走出地图物体边界
int GAMEMAP::CheckRole()
{
	int i;
	
	//角色的检查
	if(rmain.movey == 0 )
	{
		//在水平移动，
		//检测角色是否站在某个物体上
		for(i=0;i<iMapObjNum;i++)
		{
			//下方线
			if( LINE_ON_LINE(rmain.xpos,
				rmain.ypos+32,
				32,
				MapArray[i].x*32,
				MapArray[i].y*32,
				MapArray[i].w*32)
				)
			{				
				return 1;
			}
		}
		//角色开始下落
		rmain.movey=1;	
		rmain.jumpx=0;//此时要清除跳跃速度，否则将变成跳跃，而不是落体
		return 0;
	}

	return 0;
}

void GAMEMAP::ChangeFrame(int itimeclip)
{
	int i;

	///////////// 帧控制 ////////////////
	if(0 == itimeclip% ENEMY_SPEED)
	{
		//敌人移动
		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(MapEnemyArray[i].show)
			{
				//帧控制
				MapEnemyArray[i].iframe=(MapEnemyArray[i].iframe+1)%MapEnemyArray[i].iframemax;
				
				switch(MapEnemyArray[i].id)
				{
				case ID_ANI_ENEMY_NORMAL:
				case ID_ANI_ENEMY_SWORD:
					MapEnemyArray[i].x+=MapEnemyArray[i].movex;
					
					//控制敌人移动
					if(MapEnemyArray[i].movex<0)
					{
						if(MapEnemyArray[i].x<=MapEnemyArray[i].xleft)
						{
							MapEnemyArray[i].movex=ENEMY_STEP_X;						
						}
					}
					else 
					{
						if(MapEnemyArray[i].x>=MapEnemyArray[i].xright)
						{
							MapEnemyArray[i].movex=-ENEMY_STEP_X;				
						}
					}
					break;
				}
			}	
		}		
	}

	if(0 == itimeclip% WATER_SPEED)
	{
		ibkobjframe=1-ibkobjframe;	

		for(i=0;i<MAX_MAP_OBJECT;i++)
		{
			if(MapCoinArray[i].show)
			{
				MapCoinArray[i].iframe=(MapCoinArray[i].iframe+1)%MapCoinArray[i].iframemax;
			}
		}
	}

	//子弹，攻击控制
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(FireArray[i].show)
		{
			switch(FireArray[i].id)
			{
			case ID_ANI_FIRE_MAGIC:
				FireArray[i].iframe++;				
				if(FireArray[i].iframe == 2)
				{
					FireArray[i].x+=FIRE_MAGIC_W;									
				}
				if(FireArray[i].iframe>3)
				{
					FireArray[i].show=0;
				}
				break;

			default:
				FireArray[i].iframe=1-FireArray[i].iframe;
				break;
			}
		}	
	}

	//bomb frame control
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(BombArray[i].show)
		{
			BombArray[i].iframe++;
			if(BombArray[i].iframe>3)
			{
				BombArray[i].show=0;
			}
		}	
	}
}

int GAMEMAP::CheckAni(int itimeclip)
{
	int i,j;
	int tempx,tempy;

	//检测子弹和敌人的碰撞（包括魔法攻击）
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(MapEnemyArray[i].show)
		{
			for(j=0;j<MAX_MAP_OBJECT;j++)
			{
				if (FireArray[j].show)
				{
					if(RECT_HIT_RECT(FireArray[j].x+FIRE_XOFF,
						FireArray[j].y,
						FireArray[j].w,
						FireArray[j].h,
						MapEnemyArray[i].x,
						MapEnemyArray[i].y,
						MapEnemyArray[i].w,
						MapEnemyArray[i].h)
						)
					{
						ClearEnemy(i);

						//魔法攻击时，攻击效果的消除由帧控制
						switch(iAttack)
						{
						case ATTACK_NORMAL:
							FireArray[j].show=0;
							break;

						default:
							break;
						}
						return 0;
					}
				}
			}
		}
	}

	//发子弹
	if(iBeginFire)
	{
		if(0 == iTimeFire )
		{
			FireArray[iFireNum].show=1;
			FireArray[iFireNum].iframe = 0;

			//子弹方向
			if(0==rmain.idirec)
			{
				FireArray[iFireNum].movex=1;
			}
			else
			{
				FireArray[iFireNum].movex=-1;
			}

			switch(iAttack)
			{
			case ATTACK_NORMAL:
				FireArray[iFireNum].id=ID_ANI_FIRE;	
				FireArray[iFireNum].x=rmain.xpos;
				FireArray[iFireNum].y=rmain.ypos;
				FireArray[iFireNum].w=FIREW;
				FireArray[iFireNum].h=FIREH;
				FireArray[iFireNum].movex*=FIRE_SPEED;
				break;	
				
			case ATTACK_MAGIC:
				FireArray[iFireNum].id=ID_ANI_FIRE_MAGIC;
				FireArray[iFireNum].x=rmain.xpos-ID_ANI_FIRE_MAGIC_XOFF;
				FireArray[iFireNum].y=rmain.ypos-ID_ANI_FIRE_MAGIC_YOFF;
				FireArray[iFireNum].w=FIRE_MAGIC_W;
				FireArray[iFireNum].h=FIRE_MAGIC_H;
				FireArray[iFireNum].movex=0;
				break;	
			}
			//移动数组游标
			iFireNum=(iFireNum+1)%MAX_MAP_OBJECT;
		}
		iTimeFire=(iTimeFire+1)%TIME_FIRE_BETWEEN;
	}


	//检查敌人，动画元件
	//如果在显示范围之内，则设置显示属性
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if ( IN_AREA(MapEnemyArray[i].x, viewx, VIEWW) )
		{
			if(MapEnemyArray[i].health)
			{
				MapEnemyArray[i].show=1;

				//boss触发
				switch(MapEnemyArray[i].id)
				{
				case ID_ANI_BOSS_HOUSE:
					//每隔一段时间, 产生新的敌人
					if(itimeclip == TIME_CREATE_ENEMY)
					{
						MapEnemyArray[iMapEnemyCursor]=gl_enemy_normal;
						MapEnemyArray[iMapEnemyCursor].x=MapEnemyArray[i].x;
						MapEnemyArray[iMapEnemyCursor].y=MapEnemyArray[i].y+32;

						//移动游标
						iMapEnemyCursor=(iMapEnemyCursor+1)%BOSS_CURSOR;					
					}
					break;

				case ID_ANI_BOSS_HOUSE_A:
					if(itimeclip == TIME_CREATE_ENEMY)
					{
						MapEnemyArray[iMapEnemyCursor]=gl_enemy_normal;
						MapEnemyArray[iMapEnemyCursor].x=MapEnemyArray[i].x;
						MapEnemyArray[iMapEnemyCursor].y=MapEnemyArray[i].y+32;
						MapEnemyArray[iMapEnemyCursor].id=ID_ANI_ENEMY_SWORD;
						MapEnemyArray[iMapEnemyCursor].health=ENEMY_SWORD_HEALTH;

						//移动游标
						iMapEnemyCursor=(iMapEnemyCursor+1)%BOSS_CURSOR;					
					}
					break;
					
				default:
					break;
				}
			}
		}
		else
		{
			MapEnemyArray[i].show=0;
		}		
	}

	//子弹移动
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if (FireArray[i].show)
		{
			FireArray[i].x+=FireArray[i].movex;
			//子弹超出显示范围，消失
			if( FireArray[i].x > viewx+VIEWW || FireArray[i].x<viewx-FIRE_MAGIC_MAX_W)
			{
				FireArray[i].show = 0;
			}
		}
	}
		


	//检测角色和金币的碰撞,角色和其他物体的碰撞
	for(i=0;i<iCoinNum;i++)
	{
		tempx=MapCoinArray[i].x;
		tempy=MapCoinArray[i].y;
		
		if ( IN_AREA(tempx, viewx-32, VIEWW) )
		{
			if(	RECT_HIT_RECT(rmain.xpos,
				rmain.ypos,
				32,32,
				tempx,
				tempy,
				MapCoinArray[i].w,MapCoinArray[i].h)
				)
			{
				switch(MapCoinArray[i].id)
				{
				case ID_ANI_COIN:
					//碰到金币
					iMoney+=10;					
					ClearCoin(i);
					break;

				case ID_ANI_ATTACK:		
					//碰到武器
					iAttack=ATTACK_MAGIC;
					ClearCoin(i);
					break;
				}				
				return 0;
			}		
		}
	} // end of for

	return 0;
}

//清除敌人数据，只需要设置显示属性，不需要移动数组
void GAMEMAP::ClearEnemy(int i)
{

	//create bomb
	BombArray[iBombNum].show=1;
	BombArray[iBombNum].id=ID_ANI_BOMB;
	BombArray[iBombNum].iframe=0;
	BombArray[iBombNum].x=MapEnemyArray[i].x-BOMB_XOFF;
	BombArray[iBombNum].y=MapEnemyArray[i].y-BOMB_YOFF;
	iBombNum=(iBombNum+1)%MAX_MAP_OBJECT;

	//清除显示属性	
	MapEnemyArray[i].health--;


	if(MapEnemyArray[i].health<=0)
	{
		MapEnemyArray[i].show=0;
	}

	//攻击对象提示
	iAttackLife=MapEnemyArray[i].health;
	switch(MapEnemyArray[i].id)
	{
	case ID_ANI_BOSS_HOUSE:
		strcpy(AttackName,"普通级火圈");
		iAttackMaxLife=BOSS_HEALTH;
		break;

	case ID_ANI_BOSS_HOUSE_A:
		strcpy(AttackName,"战斗级火圈");
		iAttackMaxLife=BOSS_A_HEALTH;
		break;

	case ID_ANI_ENEMY_SWORD:
		strcpy(AttackName,"蘑菇刺客");
		iAttackMaxLife=ENEMY_SWORD_HEALTH;
		break;

	default:
		strcpy(AttackName,"敌人 ");
		break;
	}
}
void GAMEMAP::ClearCoin(int i)
{
	if(i<0 || i>=iCoinNum)
		return;

	switch(MapCoinArray[i].id)
	{
	case ID_ANI_COIN:
		//碰到了金币，播放一个得分的动画
		BombArray[iBombNum].show=1;
		BombArray[iBombNum].id=ID_ANI_COIN_SCORE;
		BombArray[iBombNum].iframe=0;
		BombArray[iBombNum].x=MapCoinArray[i].x-COIN_XOFF;
		BombArray[iBombNum].y=MapCoinArray[i].y-COIN_YOFF;
		iBombNum=(iBombNum+1)%MAX_MAP_OBJECT;
		break;
	}

	//减少一个金币,或者减少一个其他物品 
	for(;i<iCoinNum;i++)
	{
		MapCoinArray[i]=MapCoinArray[i+1];
	}
	iCoinNum--;
}

int GAMEMAP::IsWin()
{
	int i;

	//检测角色和敌人的碰撞
	for(i=0;i<MAX_MAP_OBJECT;i++)
	{
		if(MapEnemyArray[i].show)
		{
			if(HLINE_ON_RECT(rmain.xpos, rmain.ypos+32,32,
				MapEnemyArray[i].x,
				MapEnemyArray[i].y,
				MapEnemyArray[i].w,
				MapEnemyArray[i].h))
			{
				if(0 == rmain.movey)
				{
					Fail();
				}
				else
				{
					//踩到敌人上
					switch(MapEnemyArray[i].id)
					{
					case ID_ANI_BOSS_HOUSE:
					case ID_ANI_BOSS_HOUSE_A:
						//踩到蘑菇火圈，失败
						Fail();
						break;

					default:
						ClearEnemy(i);
						break;
					}
				}
				return 0;
			}
		}
	}

	if(rmain.ypos > GAMEH*32)
	{
	
		Fail();
		return 0;
	}

	//到达目的地后，自动走到屏幕右侧
	if(rmain.xpos > X_WIN*32 )
	{
		//已经胜利，但要播放玩家走出屏幕的动画，不接受消息响应
		iGameState=GAME_WIN;

		//到达目的地
		rmain.MoveStepTo(MAX_PAGE*GAMEW*32, Y_WIN*32);
	}

	if(rmain.xpos >= MAX_PAGE*GAMEW*32 )
	{
		//玩家已经走出屏幕,启动一定的时间间隔
		iGameState=GAME_WIN_WAIT;
		c1.ReStart(TIME_GAME_WIN_WAIT);
		
		iMatch=mapinfo.iNextMap;

		if(iMatch>=MAX_MATCH)
		{
			//全部通过
			Init();
			iGameState=GAME_PASS;			
			LoadMap();   
		}
		else
		{
			InitMatch();
			
			rmain.SetPos(BM_USER,3*32,8*32);
			rmain.InitRole(0,GAMEW*32*MAX_PAGE-32);			
			//加载下一关的地图
			LoadMap();   
		}
	}

	return 1;
}

void GAMEMAP::Fail_Wait()
{
	InitMatch();

	if(	iLife <=0)
	{
		Init();
		iGameState=GAME_OVER;
		c1.ReStart(TIME_GAME_END);	
	}
	else
	{
		iGameState=GAME_IN_PRE;
		c1.ReStart(TIME_GAME_IN_PRE);	
	}
	
	rmain.SetPos(BM_USER,3*32,8*32);
	rmain.InitRole(0,GAMEW*32*MAX_PAGE-32);

	LoadMap();   
	
}

void GAMEMAP::Fail()
{
	rmain.movex=0;
	rmain.movey=0;

	iLife--;
	rmain.iFrame=FAIL_FRAME;
	
	iGameState=GAME_FAIL_WAIT;
	c1.ReStart(TIME_GAME_FAIL_WAIT);

}


void GAMEMAP::KeyUpProc(int iKey)
{
	switch(iKey)
	{
	case VK_RIGHT:	
		rmain.movex=0;
		break;

	case VK_LEFT:
		rmain.movex=0;
		break;

	case KEY_X: //跳
		break;

	case KEY_Z: //FIRE
		iBeginFire=0;
		break;

	case KEY_W: //调整窗口大小
		
		MoveWindow(hWndMain, 
			(wwin-GAMEW*32)/2,
			(hwin-GAMEH*32)/2,
			GAMEW*32,
			GAMEH*32+32,
			true);			

		break;
	}
}

void GAMEMAP::CodeErr(int i)
{
	switch(i)
	{
	case ERR_MAP_FILE:
		SetGameState(GAME_ERR);
		break;

	default:
		break;

	}
}

void GAMEMAP::ShowMenu(MYANIOBJ & bmobj)
{
	bmobj.PlayItem(115,280+iMenu*30, ID_ANI_MENU_ARROW);
}