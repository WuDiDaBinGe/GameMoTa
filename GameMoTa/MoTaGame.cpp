#include "MoTaGame.h"


int MoTaGame::FRAME_DOWN[20] = { 0,0,1,1,1,2,2,2,3,3,0,0,1,1,1,2,2,2,3,3 };
int MoTaGame::FRAME_LEFT[20] = { 4,4,5,5,5,6,6,6,7,7,4,4,5,5,5,6,6,6,7,7 };
int MoTaGame::FRAME_RIGHT[20] = { 8,8,9,9,9,10,10,10,11,11,8,8,9,9,9,10,10,10,11,11 };
int MoTaGame::FRAME_UP[20] = { 12,12,13,13,13,14,14,14,15,15,12,12,13,13,13,14,14,14,15,15 };
MoTaGame::MoTaGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));

	wnd_width = winwidth;
	wnd_height = winheight;
	t_scene = new T_Scene();
}
//记载菜单函数
void MoTaGame::LoadGameMenu(int type)
{
	if (gameMenu == NULL) gameMenu = new T_Menu();
	gameMenu->SetMenuIndex(-1);
	if (type==GAME_START)
	{
		wstring menuItems[] = { L"开始游戏",L"游戏帮助",L"关于游戏",L"退出游戏" };
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg",200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png",250,70);
		setMenuPara(menuItems,4,250,70,0);
	}
	if (type == GAME_HELP)
	{
		wstring menuItems[] = {L"上一页",L"返回",L"下一页"};
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg", 200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems,3,250,70,2);
	}
	if (type==GAME_ABOUT)
	{
		wstring menuItems[] = {L"返回"};
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg", 200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems, 1, 250, 70, 2);
	}
}
//加载地图
void MoTaGame::LoadMap(const char* MapFilepath)
{
	t_scene->LoadTxtMap(MapFilepath);
	scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();
	//视图初始位置以地图作为参照
	int scn_x = (wnd_width - scn_width) / 2;
	int scn_y = (wnd_height - scn_height) / 2;
	//将游戏地图初始化为屏幕中央位置
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
	//将所有地图图层定位在屏幕中央
	SCENE_LAYERS::iterator p;
	for (p = t_scene->getSceneLayers()->begin(); p != t_scene->getSceneLayers()->end(); p++)
	{
		if (p->layer->ClassName() == "T_Map")
		{
			p->layer->SetPosition(scn_x, scn_y);
		}
	}
}
void MoTaGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	player = new T_Sprite(L".\\res\\player\\player.png", 32, 32);
	SPRITEINFO spinfo;
	spinfo.Active = false;
	spinfo.Alpha = 250;
	spinfo.Dead = false;
	spinfo.Dir = DIR_UP;
	spinfo.Level = 0;
	spinfo.Ratio = 1.0f;
	spinfo.Rotation = TRANS_NONE;
	spinfo.Score = 0;
	spinfo.Speed = 33;
	spinfo.X = (wnd_width - scn_width) / 2 + 33 * 11;
	spinfo.Y = (wnd_height - scn_height) / 2 + 11 * 33;
	spinfo.Visible = true;
	MOTASPINFO playerInfo;
	playerInfo.SpBasicInfo = spinfo;

	playerInfo.Aggressivity = 14;
	playerInfo.Defense = 14;
	playerInfo.LifeValue = 800;
	playerInfo.Money = 0;

	player->Initiate(playerInfo);
	player->SetSequence(FRAME_DOWN, 20);
	player->SetLayerTypeID(LAYER_PLY);


	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);

}
//加载游戏图片资源
void MoTaGame::LoadImageRes()
{
	if (yellowKey == NULL) yellowKey = new T_Graph(L".\\res\\Npc\\yellowKey.png");
	if (redKey == NULL) redKey = new T_Graph(L".\\res\\Npc\\redKey.png");
	if (blueKey == NULL) blueKey = new T_Graph(L".\\res\\Npc\\blueKey.png");
	if (player_img == NULL) player_img = new T_Graph(L".\\res\\Player\\Player.png");
}
//显示信息函数
void MoTaGame::displayInfo(HDC hdc)
{
	int FontHeight = 0;			//字号
	Gdiplus::RectF rect;
	wstring Content = L"";
	switch (GameState)
	{
	case GAME_START:
		{
		wchar_t *GameName = L"魔塔 V1.0";
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 4;
		FontHeight = 36;
		T_Graph::PaintText(hdc, rect, GameName, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

		wchar_t *Notice = L"本游戏使用TinyEngine引擎开发";
		rect.X = 0.00;
		rect.Y= (float)wnd_height / 9;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 5;
		FontHeight = 10;
		T_Graph::PaintText(hdc, rect, Notice, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_HELP:
		{
		rect.X = 60.00;
		rect.Y = 10.00;
		rect.Width = float(wnd_width-120);
		rect.Height = 80;
		FontHeight = 38;
		T_Graph::PaintText(hdc, rect, L"帮  助", (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		
		switch (helpPageIndex)
		{
		case 0:
		{
			Content = L"1.1、勇士上、下、左、右移动:键盘上、下、左、右键\n";
			Content.append(L"2、与游戏设定人物对话:键盘空格键;游戏中商店操作:鼠标操作:\n");
			Content.append(L"3、获取游戏中“怪兽手册”:按X键查看，按X键关闭；\n");
			Content.append(L"4、退出游戏:按Q键；\n");
		}
			break;
		case 1:
		{
			Content = L"1、方向键移动人物，撞到怪物自动打怪，还不能打败的则不能";
			Content.append(L"通过。走到物品上自动拾起获得，黄、蓝、红门碰撞打开（会耗费相应颜色钥匙）\n");
			Content.append(L"2、机关门（花门和铁门）在打败指定的怪物后自动打开，如2");
			Content.append(L"楼6个铁门在打败两个中级卫兵后自动打开。");
		}
			break;
		case 2:
		{
			Content = L"1.三色钥匙：打开对应颜色的门。\n";
			Content.append(L"2、红血瓶：生命+50×区域  蓝血瓶：生命+200×区域\n");
			Content.append(L"3、红、蓝宝石：提升攻击、防御+1×区域数\n");
			Content.append(L"4、铁剑、盾：攻击、防御+10\n");
			Content.append(L"   银剑、盾：攻击、防御+20\n");
			Content.append(L"   骑士剑、盾：攻击、防御+40\n");
			Content.append(L"   圣剑、盾：攻击、防御+50\n");
			Content.append(L"   神圣剑、盾：攻击、防御+100\n");
		}
			break;
		default:
			break;
		}
		
		rect.X = 60.00;
		rect.Y = 80.00;
		rect.Width = float(wnd_width-120);
		rect.Height = WIN_HEIGHT - 450;
		FontHeight = 20;
		T_Graph::PaintText(hdc, rect,Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_ABOUT:
		{
			rect.X = 60.00;
			rect.Y = 20.00;
			rect.Width = float(wnd_width - 120);
			rect.Height = 80;
			FontHeight = 38;
			T_Graph::PaintText(hdc, rect, L"关  于", (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);

			Content = L"魔塔V1.0 \n";
			Content.append(L"版本：1.0.0 \n");
			Content.append(L"作者：6栋222宿舍\n");
			Content.append(L"Copyright 2018  \n");
			Content.append(L"All Rights Reserved\n");
			Content.append(L"保留所有权利\n");
			rect.X = 60.00;
			rect.Y = 100.00;
			rect.Width = float(wnd_width - 120);
			rect.Height = WIN_HEIGHT - 450;
			FontHeight = 20;
			T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_RUN:
		{
		int title_wh = 33;
		
		int x= (wnd_width - scn_width) / 2 + title_wh;
		int y = (wnd_height - scn_height) / 2 + title_wh;
		//画背景框
		T_Graph::PaintBlank(hdc, x, y, title_wh * 4, 33 * 5 + 12, Color::Black, 100);
		//玩家图片
		player_img->PaintRegion(player_img->GetBmpHandle(),hdc,x+title_wh+10,y,0,0,player_img->GetImageWidth()/4,player_img->GetImageHeight()/4,1.3);
		//玩家的基本信息
		Content = L"生命  ";
		Content.append(T_Util::int_to_wstring(player->GetLifeValue()));
		Content.append(L"\n攻击  ");
		Content.append(T_Util::int_to_wstring(player->GetAggressivity()));
		Content.append(L"\n防御  ");
		Content.append(T_Util::int_to_wstring(player->GetDefense()));
		Content.append(L"\n金币  ");
		Content.append(T_Util::int_to_wstring(player->GetMoney()));
		Content.append(L"\n经验  ");
		Content.append(T_Util::int_to_wstring(player->GetScore()));
		rect.X = (float)x;
		rect.Y = float(y+ title_wh);
		rect.Height = (float)title_wh *5;
		rect.Width = 132;
		FontHeight = 14;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
		
		//钥匙信息
		//画背景框
		T_Graph::PaintBlank(hdc, x, y + title_wh * 6, title_wh * 4, 33 * 3, Color::Black, 100);
		//画黄色钥匙
		yellowKey->PaintRegion(yellowKey->GetBmpHandle(),hdc,x,y+ title_wh*6,0,0,yellowKey->GetImageWidth(),yellowKey->GetImageHeight(),1);
		//画黄色钥匙数量
		Content = T_Util::int_to_wstring(yellow_key_num);
		
		rect.X = (float)x+title_wh*2;
		rect.Y = (float)y + title_wh * 6;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
		//画红色钥匙
		redKey->PaintRegion(redKey->GetBmpHandle(), hdc, x, y + title_wh * 7, 0, 0, redKey->GetImageWidth(), redKey->GetImageHeight(), 1);
		//画红色钥匙数量
		Content = T_Util::int_to_wstring(red_key_num);
	
		rect.X = (float)x + title_wh * 2;
		rect.Y = (float)y + title_wh * 7;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
		//画蓝色钥匙
		blueKey->PaintRegion(blueKey->GetBmpHandle(), hdc, x, y + title_wh *8, 0, 0, blueKey->GetImageWidth(), blueKey->GetImageHeight(), 1);
		//画蓝色钥匙数量
		Content = T_Util::int_to_wstring(blue_key_num);
		
		rect.X = (float)x + title_wh * 2;
		rect.Y = (float)y + title_wh * 8;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
		

		//画所在的层数
		//画背景框
		T_Graph::PaintBlank(hdc, x, y + title_wh * 10, title_wh*4, title_wh, Color::Black, 100);
		Content = L"第 ";
		Content.append(T_Util::int_to_wstring(currentLevel));
		Content.append(L" 层\n");
		rect.X = (float)x;
		rect.Y = (float)y + title_wh * 10;
		rect.Width =(float) 4 * title_wh;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	default:
		break;
	}
}
//设置菜单参数函数
void MoTaGame::setMenuPara(wstring * menuItems, int itemSize, int m_w, int m_h, int posType)
{
	int x = 0, y = 0;
	for (int i=0;i<itemSize;i++)
	{
		switch (posType)
		{
		case 0:
			//垂直居中布局的坐标
			x = (wnd_width - m_w) / 2;
			y = i*(m_h + MENU_SPACE) + (wnd_height - 3 * m_h - 2 * MENU_SPACE) / 2;
			break;
		case 1:
			x = i*(m_w + MENU_SPACE) + (wnd_width - 2 * m_w - MENU_SPACE) / 2;
			y = (wnd_height-m_h)/2;
			break;
		case 2:
			//水平居中布局的坐标
			x = i*(m_w + MENU_SPACE) + (wnd_width - 3 * m_w - 2 * MENU_SPACE) / 2;
			y = wnd_height - 2 * m_h;
			break;
		default:
			break;
		}
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		gameMenu->AddMenuItem(mItem);
	}
	/*

		在这里添加菜单声音代码


	*/
	MENU_INFO menuinfo;
	menuinfo.align = 1;							//对齐方式
	menuinfo.space = MENU_SPACE;				//菜单项之间的间隔距离
	menuinfo.width = m_w;						//菜单项宽
	menuinfo.height = m_h;						//菜单项高
	menuinfo.fontName = L"宋体";				    //菜单项字体
	menuinfo.isBold = true;					    //是否粗体
	menuinfo.normalTextColor = Color::White;	//正常状态文字颜色
	menuinfo.focusTextColor = Color::Red;	    //选中状态文字颜色
	gameMenu->SetMenuInfo(menuinfo);
}
void MoTaGame::GameInit()
{
	GameState = GAME_START;
	LoadGameMenu(GAME_START);
	LoadMap(".\\map\\map_level1.txt");
	LoadImageRes();
	LoadPlayer();
}
void MoTaGame::GameLogic()
{
}
void MoTaGame::GameEnd()
{
}
void MoTaGame::GamePaint(HDC hdc)
{
	//非游戏运行状态
	if (GameState!=GAME_RUN)
	{
		if (gameMenu != NULL) gameMenu->DrawMenu(hdc);	
	}
	//游戏运行状态
	if (GameState==GAME_RUN)
	{
		t_scene->Draw(hdc,0,0);
	}
	displayInfo(hdc);
}
void MoTaGame::GameKeyAction(int Action)
{
	if (Action==KEY_DOWN)
	{
		if (GameState!=GAME_RUN)
		{
			if (GameState ==GAME_START)
			{
				if (GetAsyncKeyState(VK_UP) < 0)			gameMenu->MenuKeyDown(VK_UP);
				if (GetAsyncKeyState(VK_DOWN) < 0)			gameMenu->MenuKeyDown(VK_DOWN);
				if (GetAsyncKeyState(VK_LEFT) < 0)			gameMenu->MenuKeyDown(VK_LEFT);
				if (GetAsyncKeyState(VK_RIGHT) < 0)			gameMenu->MenuKeyDown(VK_RIGHT);
			}
			if (GameState==GAME_HELP||GameState==GAME_ABOUT)
			{
				if (GetAsyncKeyState(VK_LEFT) < 0)			gameMenu->MenuKeyDown(VK_LEFT);
				if (GetAsyncKeyState(VK_RIGHT) < 0)			gameMenu->MenuKeyDown(VK_RIGHT);
			}
			
			if (GetAsyncKeyState(VK_RETURN) < 0)
			{
				if (gameMenu->GetMenuIndex() >= 0)
				{
					if (GameState==GAME_START)
					{
						switch (gameMenu->GetMenuIndex())
						{
						case 0://添加开始游戏代码
							GameState = GAME_RUN;
							break;
							/*
							这里添加游戏开始声音代码
							*/
						case 1:
							//添加游戏帮助代码
							GameState = GAME_HELP;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_HELP);
							helpPageIndex = 0;
							break;
						case 2:
							//添加关于游戏代码
							GameState = GAME_ABOUT;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_ABOUT);
							break;
						case 3:
							//添加游戏代码
							SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
							break;
						default:
							break;
						}
					}
					if (GameState==GAME_HELP)
					{
						switch (gameMenu->GetMenuIndex())
						{
						case 0://上一页
							helpPageIndex = helpPageIndex - 1;
							if (helpPageIndex<0)
							{
								helpPageIndex=2;
							}
							break;
						case 1://返回主菜单
							GameState = GAME_START;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_START);
							break;
						case 2://下一页
							helpPageIndex = (helpPageIndex + 1) % 3;
							break;
						default:
							break;
						}
					}
					if (GameState==GAME_ABOUT)
					{
						switch (gameMenu->GetMenuIndex())
						{
						case 0://返回主菜单
							GameState = GAME_START;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_START);
							break;
						default:
							break;
						}
					}
				}
			}
		}

	}
}
void MoTaGame::GameMouseAction(int x, int y, int Action)
{
	if (Action==MOUSE_MOVE)
	{
		if (GameState!=GAME_RUN)
		{
			if (gameMenu != NULL) gameMenu->MenuMouseMove(x,y);
		}
	}
	if (Action == MOUSE_LCLICK)
	{
		if (gameMenu!=NULL)
		{
			if (GameState==GAME_START)
			{
				int index = gameMenu->MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0://添加开始游戏代码
						GameState = GAME_RUN;
						/*
						这里添加游戏开始声音代码
						*/
						break;
					case 1:
						//添加游戏帮助代码
						GameState = GAME_HELP;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_HELP);
						helpPageIndex = 0;
						break;
					case 2:
						//添加关于游戏代码
						GameState = GAME_ABOUT;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_ABOUT);
						break;
					case 3:
						//添加游戏代码
						SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
						break;
					default:
						break;
					}
				}
			}
			if (GameState == GAME_HELP)
			{
				int index = gameMenu->MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0://上一页
						helpPageIndex = helpPageIndex - 1;
						if (helpPageIndex<0)
						{
							helpPageIndex=2;
						}
						break;
					case 1://返回主菜单
						GameState = GAME_START;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_START);
						break;
					case 2://下一页
						helpPageIndex = (helpPageIndex + 1) % 3;
						break;
					default:
						break;
					}
				}
			}

			if (GameState == GAME_ABOUT)
			{
				int index = gameMenu->MenuMouseClick(x, y);
				if (index >= 0)
				{
					switch (index)
					{
					case 0://返回主菜单
						GameState = GAME_START;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_START);
						break;
					default:
						break;
					}
				}
			}
			
		}
		
	}
	
}
MoTaGame::~MoTaGame()
{
	
}
