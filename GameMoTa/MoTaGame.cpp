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
//���ز˵�����
void MoTaGame::LoadGameMenu(int type)
{
	if (gameMenu == NULL) gameMenu = new T_Menu();
	gameMenu->SetMenuIndex(-1);
	if (type==GAME_START)
	{
		wstring menuItems[] = { L"��ʼ��Ϸ",L"��Ϸ����",L"������Ϸ",L"�˳���Ϸ" };
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg",200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png",250,70);
		setMenuPara(menuItems,4,250,70,0);
	}
	if (type == GAME_HELP)
	{
		wstring menuItems[] = {L"��һҳ",L"����",L"��һҳ"};
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg", 200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems,3,250,70,2);
	}
	if (type==GAME_ABOUT)
	{
		wstring menuItems[] = {L"����"};
		gameMenu->SetMenuBkg(L".\\res\\Menu\\menubkg.jpg", 200);
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems, 1, 250, 70, 2);
	}
}
//���ص�ͼ
void MoTaGame::LoadMap(const char* MapFilepath)
{
	t_scene->LoadTxtMap(MapFilepath);
	scn_width = t_scene->getSceneLayers()->back().layer->GetWidth();
	scn_height = t_scene->getSceneLayers()->back().layer->GetHeight();
	//��ͼ��ʼλ���Ե�ͼ��Ϊ����
	int scn_x = (wnd_width - scn_width) / 2;
	int scn_y = (wnd_height - scn_height) / 2;
	//����Ϸ��ͼ��ʼ��Ϊ��Ļ����λ��
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
	//�����е�ͼͼ�㶨λ����Ļ����
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
//������ϷͼƬ��Դ
void MoTaGame::LoadImageRes()
{
	if (yellowKey == NULL) yellowKey = new T_Graph(L".\\res\\Npc\\yellowKey.png");
	if (redKey == NULL) redKey = new T_Graph(L".\\res\\Npc\\redKey.png");
	if (blueKey == NULL) blueKey = new T_Graph(L".\\res\\Npc\\blueKey.png");
	if (player_img == NULL) player_img = new T_Graph(L".\\res\\Player\\Player.png");
}
//��ʾ��Ϣ����
void MoTaGame::displayInfo(HDC hdc)
{
	int FontHeight = 0;			//�ֺ�
	Gdiplus::RectF rect;
	wstring Content = L"";
	switch (GameState)
	{
	case GAME_START:
		{
		wchar_t *GameName = L"ħ�� V1.0";
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 4;
		FontHeight = 36;
		T_Graph::PaintText(hdc, rect, GameName, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);

		wchar_t *Notice = L"����Ϸʹ��TinyEngine���濪��";
		rect.X = 0.00;
		rect.Y= (float)wnd_height / 9;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 5;
		FontHeight = 10;
		T_Graph::PaintText(hdc, rect, Notice, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_HELP:
		{
		rect.X = 60.00;
		rect.Y = 10.00;
		rect.Width = float(wnd_width-120);
		rect.Height = 80;
		FontHeight = 38;
		T_Graph::PaintText(hdc, rect, L"��  ��", (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
		
		switch (helpPageIndex)
		{
		case 0:
		{
			Content = L"1.1����ʿ�ϡ��¡������ƶ�:�����ϡ��¡����Ҽ�\n";
			Content.append(L"2������Ϸ�趨����Ի�:���̿ո��;��Ϸ���̵����:������:\n");
			Content.append(L"3����ȡ��Ϸ�С������ֲᡱ:��X���鿴����X���رգ�\n");
			Content.append(L"4���˳���Ϸ:��Q����\n");
		}
			break;
		case 1:
		{
			Content = L"1��������ƶ����ײ�������Զ���֣������ܴ�ܵ�����";
			Content.append(L"ͨ�����ߵ���Ʒ���Զ�ʰ���ã��ơ�����������ײ�򿪣���ķ���Ӧ��ɫԿ�ף�\n");
			Content.append(L"2�������ţ����ź����ţ��ڴ��ָ���Ĺ�����Զ��򿪣���2");
			Content.append(L"¥6�������ڴ�������м��������Զ��򿪡�");
		}
			break;
		case 2:
		{
			Content = L"1.��ɫԿ�ף��򿪶�Ӧ��ɫ���š�\n";
			Content.append(L"2����Ѫƿ������+50������  ��Ѫƿ������+200������\n");
			Content.append(L"3���졢����ʯ����������������+1��������\n");
			Content.append(L"4���������ܣ�����������+10\n");
			Content.append(L"   �������ܣ�����������+20\n");
			Content.append(L"   ��ʿ�����ܣ�����������+40\n");
			Content.append(L"   ʥ�����ܣ�����������+50\n");
			Content.append(L"   ��ʥ�����ܣ�����������+100\n");
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
		T_Graph::PaintText(hdc, rect,Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_ABOUT:
		{
			rect.X = 60.00;
			rect.Y = 20.00;
			rect.Width = float(wnd_width - 120);
			rect.Height = 80;
			FontHeight = 38;
			T_Graph::PaintText(hdc, rect, L"��  ��", (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);

			Content = L"ħ��V1.0 \n";
			Content.append(L"�汾��1.0.0 \n");
			Content.append(L"���ߣ�6��222����\n");
			Content.append(L"Copyright 2018  \n");
			Content.append(L"All Rights Reserved\n");
			Content.append(L"��������Ȩ��\n");
			rect.X = 60.00;
			rect.Y = 100.00;
			rect.Width = float(wnd_width - 120);
			rect.Height = WIN_HEIGHT - 450;
			FontHeight = 20;
			T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	case GAME_RUN:
		{
		int title_wh = 33;
		
		int x= (wnd_width - scn_width) / 2 + title_wh;
		int y = (wnd_height - scn_height) / 2 + title_wh;
		//��������
		T_Graph::PaintBlank(hdc, x, y, title_wh * 4, 33 * 5 + 12, Color::Black, 100);
		//���ͼƬ
		player_img->PaintRegion(player_img->GetBmpHandle(),hdc,x+title_wh+10,y,0,0,player_img->GetImageWidth()/4,player_img->GetImageHeight()/4,1.3);
		//��ҵĻ�����Ϣ
		Content = L"����  ";
		Content.append(T_Util::int_to_wstring(player->GetLifeValue()));
		Content.append(L"\n����  ");
		Content.append(T_Util::int_to_wstring(player->GetAggressivity()));
		Content.append(L"\n����  ");
		Content.append(T_Util::int_to_wstring(player->GetDefense()));
		Content.append(L"\n���  ");
		Content.append(T_Util::int_to_wstring(player->GetMoney()));
		Content.append(L"\n����  ");
		Content.append(T_Util::int_to_wstring(player->GetScore()));
		rect.X = (float)x;
		rect.Y = float(y+ title_wh);
		rect.Height = (float)title_wh *5;
		rect.Width = 132;
		FontHeight = 14;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		
		//Կ����Ϣ
		//��������
		T_Graph::PaintBlank(hdc, x, y + title_wh * 6, title_wh * 4, 33 * 3, Color::Black, 100);
		//����ɫԿ��
		yellowKey->PaintRegion(yellowKey->GetBmpHandle(),hdc,x,y+ title_wh*6,0,0,yellowKey->GetImageWidth(),yellowKey->GetImageHeight(),1);
		//����ɫԿ������
		Content = T_Util::int_to_wstring(yellow_key_num);
		
		rect.X = (float)x+title_wh*2;
		rect.Y = (float)y + title_wh * 6;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		//����ɫԿ��
		redKey->PaintRegion(redKey->GetBmpHandle(), hdc, x, y + title_wh * 7, 0, 0, redKey->GetImageWidth(), redKey->GetImageHeight(), 1);
		//����ɫԿ������
		Content = T_Util::int_to_wstring(red_key_num);
	
		rect.X = (float)x + title_wh * 2;
		rect.Y = (float)y + title_wh * 7;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		//����ɫԿ��
		blueKey->PaintRegion(blueKey->GetBmpHandle(), hdc, x, y + title_wh *8, 0, 0, blueKey->GetImageWidth(), blueKey->GetImageHeight(), 1);
		//����ɫԿ������
		Content = T_Util::int_to_wstring(blue_key_num);
		
		rect.X = (float)x + title_wh * 2;
		rect.Y = (float)y + title_wh * 8;
		rect.Width = (float)title_wh * 2;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		

		//�����ڵĲ���
		//��������
		T_Graph::PaintBlank(hdc, x, y + title_wh * 10, title_wh*4, title_wh, Color::Black, 100);
		Content = L"�� ";
		Content.append(T_Util::int_to_wstring(currentLevel));
		Content.append(L" ��\n");
		rect.X = (float)x;
		rect.Y = (float)y + title_wh * 10;
		rect.Width =(float) 4 * title_wh;
		rect.Height = (float)title_wh;
		T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
		}
	break;
	default:
		break;
	}
}
//���ò˵���������
void MoTaGame::setMenuPara(wstring * menuItems, int itemSize, int m_w, int m_h, int posType)
{
	int x = 0, y = 0;
	for (int i=0;i<itemSize;i++)
	{
		switch (posType)
		{
		case 0:
			//��ֱ���в��ֵ�����
			x = (wnd_width - m_w) / 2;
			y = i*(m_h + MENU_SPACE) + (wnd_height - 3 * m_h - 2 * MENU_SPACE) / 2;
			break;
		case 1:
			x = i*(m_w + MENU_SPACE) + (wnd_width - 2 * m_w - MENU_SPACE) / 2;
			y = (wnd_height-m_h)/2;
			break;
		case 2:
			//ˮƽ���в��ֵ�����
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

		��������Ӳ˵���������


	*/
	MENU_INFO menuinfo;
	menuinfo.align = 1;							//���뷽ʽ
	menuinfo.space = MENU_SPACE;				//�˵���֮��ļ������
	menuinfo.width = m_w;						//�˵����
	menuinfo.height = m_h;						//�˵����
	menuinfo.fontName = L"����";				    //�˵�������
	menuinfo.isBold = true;					    //�Ƿ����
	menuinfo.normalTextColor = Color::White;	//����״̬������ɫ
	menuinfo.focusTextColor = Color::Red;	    //ѡ��״̬������ɫ
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
	//����Ϸ����״̬
	if (GameState!=GAME_RUN)
	{
		if (gameMenu != NULL) gameMenu->DrawMenu(hdc);	
	}
	//��Ϸ����״̬
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
						case 0://��ӿ�ʼ��Ϸ����
							GameState = GAME_RUN;
							break;
							/*
							���������Ϸ��ʼ��������
							*/
						case 1:
							//�����Ϸ��������
							GameState = GAME_HELP;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_HELP);
							helpPageIndex = 0;
							break;
						case 2:
							//��ӹ�����Ϸ����
							GameState = GAME_ABOUT;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_ABOUT);
							break;
						case 3:
							//�����Ϸ����
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
						case 0://��һҳ
							helpPageIndex = helpPageIndex - 1;
							if (helpPageIndex<0)
							{
								helpPageIndex=2;
							}
							break;
						case 1://�������˵�
							GameState = GAME_START;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_START);
							break;
						case 2://��һҳ
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
						case 0://�������˵�
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
					case 0://��ӿ�ʼ��Ϸ����
						GameState = GAME_RUN;
						/*
						���������Ϸ��ʼ��������
						*/
						break;
					case 1:
						//�����Ϸ��������
						GameState = GAME_HELP;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_HELP);
						helpPageIndex = 0;
						break;
					case 2:
						//��ӹ�����Ϸ����
						GameState = GAME_ABOUT;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_ABOUT);
						break;
					case 3:
						//�����Ϸ����
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
					case 0://��һҳ
						helpPageIndex = helpPageIndex - 1;
						if (helpPageIndex<0)
						{
							helpPageIndex=2;
						}
						break;
					case 1://�������˵�
						GameState = GAME_START;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_START);
						break;
					case 2://��һҳ
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
					case 0://�������˵�
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
