#include "MoTaGame.h"


int MoTaGame::FRAME_DOWN[20] = { 0,0,1,1,1,2,2,2,3,3,0,0,1,1,1,2,2,2,3,3 };
int MoTaGame::FRAME_LEFT[20] = { 4,4,5,5,5,6,6,6,7,7,4,4,5,5,5,6,6,6,7,7 };
int MoTaGame::FRAME_RIGHT[20] = { 8,8,9,9,9,10,10,10,11,11,8,8,9,9,9,10,10,10,11,11 };
int MoTaGame::FRAME_UP[20] = { 12,12,13,13,13,14,14,14,15,15,12,12,13,13,13,14,14,14,15,15 };

const char *MoTaGame::mapfiles[TOTAL_LEVEL] = {".\\map\\map_level1.txt",".\\map\\map_level2.txt",".\\map\\map_level3.txt",".\\map\\map_level4.txt",".\\map\\map_level5.txt",".\\map\\map_level6.txt" };
const char* MoTaGame::npcfiles[TOTAL_LEVEL] = { ".\\npcfile\\level1.csv",".\\npcfile\\level2.csv",".\\npcfile\\level3.csv",".\\npcfile\\level4.csv",".\\npcfile\\level5.csv",".\\npcfile\\level6.csv" };
Pos MoTaGame::startPos[TOTAL_LEVEL] = { Pos(0,0),Pos(6 * 33,33),Pos(6*33,11*33)};
Pos MoTaGame::endPos[TOTAL_LEVEL] = { Pos(6*33,33),Pos(6*33,11*33),Pos(16*33,11*33) };
Pos MoTaGame::startNpcPos[TOTAL_LEVEL] = {Pos(11*33,11*33),Pos(6*33,2*33),Pos(7*33,11*33)};
Pos MoTaGame::endNpcPos[TOTAL_LEVEL] = {Pos(7*33,33),Pos(6*33,10*33),Pos(15*33,11*33)};


MoTaGame::MoTaGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));

	wnd_width = winwidth;
	wnd_height = winheight;
	t_scene = new T_Scene();
	yellow_key_num = 100;
	red_key_num = 1;
	blue_key_num = 1;
	currentLevel = 0;
	arrivedLevel = 0;
	handbook = false;
	pause_State = 0;
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
	if (type==GAME_PAUSE&&(pause_State==2||pause_State==3))
	{
		wstring menuItems1[] = {L"����5�㹥��",L"����5�����",L"������Ϸ" };
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems1, 3, 250, 70, 0);
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
	spinfo.Score = 100;
	spinfo.Speed = 33;
	spinfo.X = (wnd_width - scn_width) / 2 + 33 * 11;
	spinfo.Y = (wnd_height - scn_height) / 2 + 11 * 33;
	spinfo.Visible = true;
	MOTASPINFO playerInfo;
	playerInfo.SpBasicInfo = spinfo;

	playerInfo.Aggressivity = 1400;
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
//���ļ��м���NPC��Ϣ
void MoTaGame::LoadNpc(const char * filePath)
{
	vector<MOTASPINFO> npcInfos;
	npcInfos = T_Util::ParseCsv(filePath);
	GAMELAYER gameLayer;
	vector<MOTASPINFO>::iterator it;
	for (it = npcInfos.begin(); it < npcInfos.end(); it++)
	{
		T_Sprite *sp = new T_Sprite(it->ImaPath, 32, 32);
		it->SpBasicInfo.X = it->SpBasicInfo.X + (wnd_width - scn_width) / 2;
		it->SpBasicInfo.Y = it->SpBasicInfo.Y + (wnd_height - scn_height) / 2;
		sp->Initiate(*it);
		sp->SetLayerTypeID(LAYER_NPC);
		npc_set.push_back(sp);
		gameLayer.layer = sp;
		gameLayer.type_id = LAYER_NPC;
		gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
		sp = NULL;
	}
}

void MoTaGame::ClearGameLevel()
{
	delete t_scene;
	t_scene = NULL;
	vector<T_Sprite*>temp = vector<T_Sprite*>();
	npc_set.clear();
	npc_set.swap(temp);
}

//���عؿ���Դ
void MoTaGame::LoadGameLevel(int level)
{
	MOTASPINFO tempPlayerInfo;
	tempPlayerInfo.Aggressivity = player->GetAggressivity();
	tempPlayerInfo.Defense = player->GetDefense();
	tempPlayerInfo.LifeValue = player->GetLifeValue();
	tempPlayerInfo.Money = player->GetMoney();
	tempPlayerInfo.SpBasicInfo.Score = player->GetScore();
	ClearGameLevel();
	t_scene = new T_Scene();
	LoadMap(mapfiles[level]);
	//�������Ĺؿ���һ���µĹؿ�
	if (level > arrivedLevel)
	{
		LoadNpc(npcfiles[level]);
	}
	//����ؿ�������������Ĺؿ��������ԭ������Դ
	else
	{
		npc_set = npc_vec[level];
		vSpriteSet::iterator it;
		for (it = npc_set.begin(); it < npc_set.end(); it++)
		{
			GAMELAYER gameLayer;
			gameLayer.layer =*it;
			gameLayer.type_id = LAYER_NPC;
			gameLayer.z_order = t_scene->getSceneLayers()->size() + 1;
			gameLayer.layer->setZorder(gameLayer.z_order);
			t_scene->Append(gameLayer);
		}
	}
	LoadPlayer();
	player->SetMoney(tempPlayerInfo.Money);
	player->SetAggressivity(tempPlayerInfo.Aggressivity);
	player->SetDefense(tempPlayerInfo.Defense);
	player->SetLifeValue(tempPlayerInfo.LifeValue);
	player->SetScore(tempPlayerInfo.SpBasicInfo.Score);
}
//�������λ��
void MoTaGame::UpdatePlayerPos(int dir)
{
	if (player == NULL)return;
	int nextStepX, nextStepY;
	int SpeedX = 0, SpeedY = 0;
	if (player->IsVisible() == true && player->IsActive() == true && player->IsDead() == false)
	{
		switch (dir)
		{
		case DIR_LEFT:
			SpeedX = -player->GetSpeed();
			SpeedY = 0;
			nextStepX = player->GetX() - player->GetSpeed();
			if (nextStepX <= 0) SpeedX = 0 - player->GetX();
			break;
		case DIR_RIGHT:
			SpeedX = player->GetSpeed();
			SpeedY = 0;
			nextStepX = player->GetX() + player->GetRatioSize().cx + player->GetSpeed();
			if (nextStepX >= wnd_width)SpeedX = wnd_width - player->GetRatioSize().cx - player->GetX();
			break;
		case DIR_UP:
			SpeedX = 0;
			SpeedY = -player->GetSpeed();
			nextStepY = player->GetY() - player->GetSpeed();
			if (nextStepY >= wnd_height)SpeedY = 0 - player->GetY();
			break;
		case DIR_DOWN:
			SpeedX = 0;
			SpeedY = player->GetSpeed();
			nextStepY = player->GetY() + player->GetRatioSize().cy + player->GetSpeed();
			if (nextStepY >= wnd_height)SpeedY = wnd_height - player->GetRatioSize().cy - player->GetY();
			break;
		}
		int x = player->GetX();
		int y = player->GetY();
		if (!player->CollideWith(t_scene->getBarrier()))player->Move(SpeedX, SpeedY);
		if (player->CollideWith(t_scene->getBarrier()))player->SetPosition(x, y);
		vSpriteSet::iterator it;
		for (it = npc_set.begin(); it < npc_set.end(); it++)
		{
			if (player->CollideWith((*it)))
			{
				//��ֹ����Կ�״�һ����
				int type = (*it)->GetRoleType();
				if (type==1||type==2||type==3)
				{
					if ((*it)->GetDoorOpen() == 0)
					{
						Collide((*it));
						player->SetPosition(x, y);
					}
				}
				else
				{
					Collide((*it));
					player->SetPosition(x, y);
				}
				
			}
		}

		int tempEndX = (wnd_width - scn_width) / 2 +endPos[currentLevel].first;
		int tempEndY = (wnd_height - scn_height) / 2 + endPos[currentLevel].second;

		int tempStartX = (wnd_width - scn_width) / 2 + startPos[currentLevel].first;
		int tempStartY = (wnd_height - scn_height) / 2 + startPos[currentLevel].second;
		

		//������һ���ؿ�
		if (player->GetX() == tempEndX&&player->GetY() == tempEndY)
		{
			if (currentLevel >= npc_vec.size())
			{
				npc_vec.push_back(npc_set);
			}
			else
			{
				npc_vec[currentLevel] = npc_set;
			}
			currentLevel++;
			LoadGameLevel(currentLevel);
			if (currentLevel > arrivedLevel)
			{
				arrivedLevel = currentLevel;
			}
			int tempNpcX = (wnd_width - scn_width) / 2 + startNpcPos[currentLevel].first;
			int tempNpcY = (wnd_height - scn_height) / 2 + startNpcPos[currentLevel].second;
			player->SetPosition(tempNpcX, tempNpcY);
		}
		//������һ���ؿ�
		if (player->GetX() ==tempStartX&&player->GetY()==tempStartY)
		{
			if (currentLevel >= npc_vec.size())
			{
				npc_vec.push_back(npc_set);
			}
			else
			{
				npc_vec[currentLevel] = npc_set;
			}
			currentLevel--;
			LoadGameLevel(currentLevel);
			int tempNpcX = (wnd_width - scn_width) / 2 + endNpcPos[currentLevel].first;
			int tempNpcY = (wnd_height - scn_height) / 2 + endNpcPos[currentLevel].second;
			player->SetPosition(tempNpcX, tempNpcY);
		}

	}
}
//����֡״̬����
void MoTaGame::UpdateFrames()
{
	vSpriteSet::iterator p;
	if (npc_set.size() > 0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); p++)
		{
			if ((*p)->GetRoleType() == 0)
			{
				(*p)->LoopFrame();
			}
			if ((*p)->GetDoorOpen() == 1)
			{
				bool endFrame = (*p)->LoopFrameOnce();
				if (endFrame)
				{
					(*p)->SetDead(true);
				}
			}
		}
	}

	if (npc_set.size() > 0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); )
		{
			if ((*p)->IsDead() == true)
			{
				SCENE_LAYERS::iterator q;
				for (q = t_scene->getSceneLayers()->begin(); q != t_scene->getSceneLayers()->end(); q++)
				{
					if ((*q).layer == (*p))
					{
						q = t_scene->getSceneLayers()->erase(q);
						break;
					}
				}
				delete(*p);
				p = npc_set.erase(p);
			}
			else {
				p++;
			}
		}
	}

	if (player != NULL)
	{
		if (player->IsVisible() == true && player->IsActive() == true)player->LoopFrame();
	}
}
//ս������
void MoTaGame::Battling()
{
	
	if (battleNpc->GetLifeValue() == 0)
	{
		battleNpc->SetDead(true);
		GameState = GAME_RUN;
		player->SetMoney(battleNpc->GetMoney()+player->GetMoney());
		player->SetScore(battleNpc->GetScore()+player->GetScore());
	}
	if (battleNpc != NULL)
	{
		player->SetEndTime(GetTickCount());
		if (player->GetEndTime() - player->GetStartTime() >= 400)
		{
			player->SetStartTime(player->GetEndTime());
			if (player->GetDefense() >= battleNpc->GetAggressivity())
			{
				int tempLV = battleNpc->GetLifeValue() - (player->GetAggressivity() - battleNpc->GetDefense());
				if (tempLV < 0)
				{
					tempLV = 0;
				}
				battleNpc->SetLifeValue(tempLV);
			}
			else {
				int pa = player->GetAggressivity();
				int pd = player->GetDefense();
				int pl = player->GetLifeValue();
				int sa = battleNpc->GetAggressivity();
				int sd = battleNpc->GetDefense();
				int sl = battleNpc->GetLifeValue();
				int tempNLV = sl - (pa - sd);
				int tempPLV = pl - (sa - pd);
				if (tempNLV < 0)
				{
					tempNLV = 0;
				}
				battleNpc->SetLifeValue(tempNLV);
				player->SetLifeValue(tempPLV);
			}
		}
		
		
	}
}
//��ʾ��Ϣ����
void MoTaGame::DisplayInfo(HDC hdc)
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
		player_img->PaintRegion(player_img->GetBmpHandle(),hdc,x+title_wh+10,y,0,0,player_img->GetImageWidth()/4,player_img->GetImageHeight()/4,(double)1.3);
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
	case GAME_BATTLE:
	{
		wchar_t *GameName = L"ħ�� V1.0";
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 4;
		FontHeight = 36;
		T_Graph::PaintText(hdc, rect, GameName, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
	}
	break;
	case GAME_PAUSE:
	{
		//��������
		if (pause_State==2)
		{
			Content = L"����25���鹺�����£�";
			rect.X = (float)(wnd_width - scn_width)/2;
			rect.Y = (float)(wnd_height - scn_height)/2;
			rect.Width = 33 * 18.00;
			rect.Height = 33 * 5.00;
			FontHeight = 20;
			T_Graph::PaintText(hdc,rect,Content,(REAL)FontHeight,L"����",Color::White,FontStyleBold,StringAlignmentNear);
		}
		//�������
		if (pause_State==3)
		{
			Content = L"����25��ҹ������£�";
			rect.X = (float)(wnd_width - scn_width) / 2;
			rect.Y = (float)(wnd_height - scn_height) / 2;
			rect.Width = 33 * 18.00;
			rect.Height = 33 * 5.00;
			FontHeight = 20;
			T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		}
	}
	break;
	default:
		break;
	}
}
//��ײ��⺯��
void MoTaGame::Collide(T_Sprite * sp)
{
	switch (sp->GetRoleType())
	{
	case 0:
		if (IsBattle(sp))
		{
			battleNpc = sp;
			GameState = GAME_BATTLE;
		}
		break;
		//�򶷴���
	case 1:
		if (yellow_key_num > 0)
		{
			sp->SetDoorOpen(1);
			sp->SetActive(false);
			yellow_key_num--;
		}
		break;
	case 2:
		if (red_key_num > 0)
		{
			sp->SetDoorOpen(1);
			red_key_num--;
		}
		break;
	case 3:
		if (blue_key_num > 0)
		{
			sp->SetDoorOpen(1);
			blue_key_num--;
		}
		break;
	case 4:
		yellow_key_num++;
		sp->SetDead(true);
		break;
	case 5:
		red_key_num++;
		sp->SetDead(true);
		break;
	case 6:
		blue_key_num++;
		sp->SetDead(true);
		break;
	case 7:
		player->SetAggressivity(player->GetAggressivity() + sp->GetAggressivity()); //����������
		player->SetDefense(player->GetDefense() + sp->GetDefense());				//����������
		player->SetLifeValue(player->GetLifeValue() + sp->GetLifeValue());			//����ֵ����
		sp->SetDead(true);
		break;
	case 8:
		GameState = GAME_PAUSE;
		pause_State = 3;
		delete gameMenu;
		gameMenu = NULL;
		LoadGameMenu(GAME_PAUSE);
		break;
	//��ײ����������
	case 9:
		GameState = GAME_PAUSE;
		pause_State = 2;
		delete gameMenu;
		gameMenu = NULL;
		LoadGameMenu(GAME_PAUSE);
		break;
	//���������ֲ����
	case 10:
		handbook = true;
		sp->SetVisible(false);
		sp->SetDead(true);
		break;

	default:
		break;
	}


}
//��ʱ�Ļ�����ʾ
void MoTaGame::DisplayCombat(T_Sprite *sp,HDC hdc)
{
	wstring Content = L"";
	Gdiplus::RectF rect;
	int FontHeight=0;
	int title_wh = 33;
	int x = (wnd_width-scn_width)/2+title_wh;
	int y = (wnd_height - scn_height) / 2+title_wh;
	//��ɫ�߿�
	//T_Graph::PaintBlank(hdc, 0, 0, title_wh * 16+20, title_wh * 8+20,Color::OrangeRed,100);
	//��ɫ����
	T_Graph::PaintBlank(hdc, x, y, title_wh * 16, title_wh *8, Color::Black, 190);
	//����ս����Ϣ
	Content = L"����ֵ��";
	Content.append(T_Util::int_to_wstring(sp->GetLifeValue()));
	Content.append(L"\n\n������:");
	Content.append(T_Util::int_to_wstring(sp->GetAggressivity()));
	Content.append(L"\n\n��������");
	Content.append(T_Util::int_to_wstring(sp->GetDefense()));
	rect.X =(float) (x + 100);
	rect.Y = (float)(y + 20);
	rect.Width = (float)title_wh*3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 10;
	T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
	//VS
	wchar_t *vs = L"V\nS";
	rect.X = rect.X + rect.Width+15;
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 20;
	T_Graph::PaintText(hdc, rect, vs, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
	//��ʿս��ս��
	Content = T_Util::int_to_wstring(player->GetLifeValue());
	Content.append(L":����\n\n");
	Content.append(T_Util::int_to_wstring(player->GetAggressivity()));
	Content.append(L":������\n\n");
	Content.append(T_Util::int_to_wstring(player->GetDefense()));
	Content.append(L":������");
	rect.X = (float)(x+ title_wh * 13 - 100);
	rect.Y = (float)(y + 20);
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 10;
	T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
	//����
	FontHeight = 20;
	wchar_t *NpcName = L"�� ��";
	rect.X = (float)(x + 10);
	rect.Y = (float)(y + title_wh * 6);
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 2+5;
	T_Graph::PaintText(hdc, rect, NpcName, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
	//��ʿ
	wchar_t *PlayName = L"�� ʿ";
	rect.X = (float)(x + 13*title_wh-10);
	rect.Y = (float)(y + title_wh * 6);
	T_Graph::PaintText(hdc, rect, PlayName, (REAL)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
	//�����޵�ͼƬ
	//T_Graph::PaintFrame(sp->GetImage()->GetBmpHandle(), hdc,x,y+2*title_wh,4,1,32,32,3);
	T_Graph::PaintRegion(sp->GetImage()->GetBmpHandle(),hdc,x,y+2*title_wh,0,0,32,32,3);
	//�����ͼƬ
	T_Graph::PaintRegion(player->GetImage()->GetBmpHandle(),hdc,x+13*title_wh,y+2*title_wh,0,0,32,32,3);
}
//��ʾ�����ֲ�
void MoTaGame::DisplayHandBook(HDC hdc, vSpriteSet npcSet)
{
	int title_wh = 33;
	int x = (wnd_width - scn_width) / 2 + title_wh;
	int y = (wnd_height - scn_height) / 2 + title_wh;
	int count = 0;
	Gdiplus::RectF rect;
	int FontHeight;
	//��ͼ��
	T_Graph::PaintBlank(hdc, x, y, title_wh * 16, title_wh * 11, Color::Black, 190);
	vSpriteSet tempSet;
	vSpriteSet::iterator it;
	for (it = npcSet.begin(); it != npcSet.end(); it++)
	{
		//npcΪ��������
		if ((*it)->GetRoleType() == 0 && (*it)->IsDead() == false && (*it)->IsVisible() == true)
		{
			if (!ContainMonter((*it), tempSet))
			{
				tempSet.push_back((*it));
			}
		}
	}
	for (it = tempSet.begin(); it != tempSet.end(); it++)
	{
		T_Graph::PaintRegion((*it)->GetImage()->GetBmpHandle(), hdc, x, y + count*title_wh, 0, 0,
			(*it)->GetImage()->GetImageWidth() / 4, (*it)->GetImage()->GetImageHeight(), 1);

		wstring Content = L"";
		Content = L"��������";
		Content.append(T_Util::int_to_wstring((*it)->GetAggressivity()));
		Content.append(L"   ��������");
		Content.append(T_Util::int_to_wstring((*it)->GetDefense()));
		Content.append(L"   ��Ǯ��");
		Content.append(T_Util::int_to_wstring((*it)->GetMoney()));
		Content.append(L"   ����:");
		Content.append(T_Util::int_to_wstring((*it)->GetScore()));
		rect.X = (float)x + 2 * title_wh;
		rect.Y = (float)y + count*title_wh;
		rect.Width = (float)15 * title_wh;
		rect.Height = (float)title_wh;
		FontHeight = 10;
		T_Graph::PaintText(hdc, rect, Content, (float)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentNear);
		count++;
	}
	rect.X = (float)x;
	rect.Y = (float)y + title_wh * 10;
	rect.Width = (float)16 * title_wh;
	rect.Height = (float)title_wh;
	T_Graph::PaintText(hdc, rect, L"--����Backspace���˳�--", (float)FontHeight, L"����", Color::White, FontStyleBold, StringAlignmentCenter);
}
//��ʾ��Ǯ����˵�
void MoTaGame::DisplayMoneyBuyMenu(HDC hdc)
{

}
//����Ƿ�����ù���ս��
BOOL MoTaGame::IsBattle(T_Sprite * sp)
{
	int pa = player->GetAggressivity();
	int pd = player->GetDefense();
	int pl = player->GetLifeValue();
	//������Ϣ
	int sa = sp->GetAggressivity();
	int sd = sp->GetDefense();
	int sl = sp->GetLifeValue();

	//��ҹ�����С�ڹ������������ս��
	if (pa <= sd)
	{
		return false;
	}
	else
	{

		//���﹥����С����ҷ�����������ս��
		if (sa <= pd)
		{
			return true;
		}

		while (sl>0 && pl>0)
		{
			sl = sl - (pa - sd);
			pl = pl - (sa - pd);
		}
		//�������սʤ�����ô����ս��
		if (pl <= 0)
		{
			return false;
		}
		//�������սʤ�����ô����ս��
		if (sl <= 0)
		{
			return true;
		}
	}
}
//�ж��Ƿ�Ϊͬ���Ĺ���
BOOL MoTaGame::IsSameMonster(T_Sprite * sp1, T_Sprite * sp2)
{
	if (sp1->GetAggressivity() == sp2->GetAggressivity() && sp1->GetDefense() == sp2->GetDefense()
		&& sp1->GetScore() == sp2->GetScore() && sp1->GetMoney() == sp2->GetMoney())
	{
		return true;
	}
	else
	{
		return false;
	}
}
//�������Ƿ���ڸù���
BOOL MoTaGame::ContainMonter(T_Sprite * sp, vSpriteSet monsterSet)
{
	vSpriteSet::iterator it;
	for (it = monsterSet.begin(); it != monsterSet.end(); it++)
	{
		if (IsSameMonster((*it), (sp)))
		{
			return true;
		}
		else
		{
			continue;
		}
	}
	return false;
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
	LoadNpc(".\\npcfile\\level1.csv");
}
void MoTaGame::GameLogic()
{
	if (GameState==GAME_RUN)
	{
		GameKeyAction();
		UpdatePlayerPos(player->GetDir());
		UpdateFrames();
	}
	if (GameState==GAME_BATTLE)
	{
		Battling();
	}
	
}
void MoTaGame::GameEnd()
{
}
void MoTaGame::GamePaint(HDC hdc)
{
	//����Ϸ����״̬
	if (GameState!=GAME_RUN)
	{
		if (GameState==GAME_START||GameState==GAME_HELP||GameState==GAME_ABOUT)
		{
			if (gameMenu != NULL) gameMenu->DrawMenu(hdc);
			wchar_t buf[256];
			wsprintf(buf, L"1total=%d\n", gameMenu->gm_menuItems.size());
			OutputDebugString(buf);
		}
				
		if (GameState == GAME_PAUSE)
		{
			if (t_scene!=NULL)
			{
				t_scene->Draw(hdc,0,0);
			}
			switch (pause_State)
			{
			case 1:
			{
				DisplayHandBook(hdc, npc_set);
			}
			break;
			case 2:
			{
				if(gameMenu!= NULL) gameMenu->DrawMenu(hdc,255,false);
				wchar_t buf[256];
				wsprintf(buf, L"2total=%d\n", gameMenu->gm_menuItems.size());
				OutputDebugString(buf);
			}
			break;
			default:
				break;
			}
		}
	}
	//��Ϸ����״̬
	if (GameState==GAME_RUN)
	{
		/*wchar_t buf[256];
		wsprintf(buf, L"total=%d\n", t_scene->GetTotalLayers());
		OutputDebugString(buf);*/

		t_scene->Draw(hdc,0,0);
	}
	if (GameState==GAME_BATTLE)
	{
		
		/*wchar_t buf[256];
		wsprintf(buf, L"total=%d\n", t_scene->GetTotalLayers());
		OutputDebugString(buf);*/

		t_scene->Draw(hdc,0,0);
		DisplayCombat(battleNpc,hdc);
	}
	DisplayInfo(hdc);
}
void MoTaGame::GameKeyAction(int Action)
{
	/*��ҿ���*/
	if (Action == KEY_SYS_NONE)
	{
		player->SetEndTime(GetTickCount());
		if (player->GetEndTime() - player->GetStartTime() >= 50)
		{
			player->SetStartTime(player->GetEndTime());
			if (GameState == GAME_RUN)
			{
				if (CheckKey(VK_LEFT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
				{
					player->SetActive(true);
					player->SetSequence(FRAME_LEFT, 20);
					player->SetDir(DIR_LEFT);
				}

				if (CheckKey(VK_RIGHT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
				{
					player->SetActive(true);
					player->SetSequence(FRAME_RIGHT, 20);
					player->SetDir(DIR_RIGHT);
				}

				if (CheckKey(VK_UP) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
				{
					player->SetActive(true);
					player->SetSequence(FRAME_UP, 20);
					player->SetDir(DIR_UP);
				}

				if (CheckKey(VK_DOWN) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
				{
					player->SetActive(true);
					player->SetSequence(FRAME_DOWN, 20);
					player->SetDir(DIR_DOWN);
				}
				if (CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT) == false &&
					CheckKey(VK_UP) == false && CheckKey(VK_DOWN) == false)
				{
					player->SetActive(false);
				}
			}
		}
	}


	if (Action==KEY_DOWN)
	{
		if (GameState!=GAME_RUN)
		{
			if (GameState ==GAME_START||(GameState==GAME_PAUSE&&pause_State==2))
			{
				if (GetAsyncKeyState(VK_UP) < 0)			gameMenu->MenuKeyDown(VK_UP);
				if (GetAsyncKeyState(VK_DOWN) < 0)			gameMenu->MenuKeyDown(VK_DOWN);
			}
			if (GameState==GAME_HELP||GameState==GAME_ABOUT|| GameState == GAME_START)
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

					if (GameState==GAME_PAUSE&&pause_State==2)
					{
						switch (gameMenu->GetMenuIndex())
						{
							//���򹥻���
						case 0:
							if (player->GetScore()>25)
							{
								player->SetAggressivity(player->GetAggressivity()+5);
								player->SetScore(player->GetScore() - 25);
							}
							break;
							//���������
						case 1:
							if (player->GetScore()>25)
							{
								player->SetDefense(player->GetDefense()+5);
								player->SetScore(player->GetScore()-25);
							}
							break;
							//������Ϸ
						case 2:
							GameState = GAME_RUN;
							pause_State = 0;
							break;
						default:
							break;
						}
					}

					if (GameState == GAME_PAUSE&&pause_State == 2)
					{
						switch (gameMenu->GetMenuIndex())
						{
							//���򹥻���
						case 0:
							if (player->GetMoney()>25)
							{
								player->SetAggressivity(player->GetAggressivity() + 5);
								player->SetMoney(player->GetMoney() - 25);
							}
							break;
							//���������
						case 1:
							if (player->GetMoney()>25)
							{
								player->SetDefense(player->GetDefense() + 5);
								player->SetMoney(player->GetMoney() - 25);
							}
							break;
							//������Ϸ
						case 2:
							GameState = GAME_RUN;
							pause_State = 0;
							break;
						default:
							break;
						}
					}


				}
			}
		}
		
		if (GameState == GAME_RUN)
		{
			if (GetAsyncKeyState(VK_TAB)<0)
			{
				if (player->IsDead() == false && player->IsVisible() == true)
				{
					if (handbook == true)
					{
						GameState = GAME_PAUSE;
						pause_State = 1;		//�鿴�����ֲ�
					}
				}
			}
		}
		//����Backspace�� ������Ϸ
		if (GameState==GAME_PAUSE&&pause_State==1)
		{
			if (GetAsyncKeyState(VK_BACK)<0)
			{
				if (handbook == true)
				{
					GameState = GAME_RUN;
					pause_State = 0;		//�鿴�����ֲ�
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
