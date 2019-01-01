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
	if (type==GAME_PAUSE&&(pause_State==2||pause_State==3))
	{
		wstring menuItems1[] = {L"增加5点攻击",L"增加5点防御",L"返回游戏" };
		gameMenu->SetBtnBmp(L".\\res\\Menu\\button.png", 250, 70);
		setMenuPara(menuItems1, 3, 250, 70, 0);
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
//加载游戏图片资源
void MoTaGame::LoadImageRes()
{
	if (yellowKey == NULL) yellowKey = new T_Graph(L".\\res\\Npc\\yellowKey.png");
	if (redKey == NULL) redKey = new T_Graph(L".\\res\\Npc\\redKey.png");
	if (blueKey == NULL) blueKey = new T_Graph(L".\\res\\Npc\\blueKey.png");
	if (player_img == NULL) player_img = new T_Graph(L".\\res\\Player\\Player.png");
}
//从文件中加载NPC信息
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

//加载关卡资源
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
	//如果到达的关卡是一个新的关卡
	if (level > arrivedLevel)
	{
		LoadNpc(npcfiles[level]);
	}
	//如果关卡是曾经到达过的关卡，则加载原来的资源
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
//更新玩家位置
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
				//防止两把钥匙打开一个门
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
		

		//进入下一个关卡
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
		//返回上一个关卡
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
//更新帧状态函数
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
//战斗函数
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
//显示信息函数
void MoTaGame::DisplayInfo(HDC hdc)
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
		player_img->PaintRegion(player_img->GetBmpHandle(),hdc,x+title_wh+10,y,0,0,player_img->GetImageWidth()/4,player_img->GetImageHeight()/4,(double)1.3);
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
	case GAME_BATTLE:
	{
		wchar_t *GameName = L"魔塔 V1.0";
		rect.X = 0.00;
		rect.Y = 0.00;
		rect.Width = (float)wnd_width;
		rect.Height = (float)wnd_height / 4;
		FontHeight = 36;
		T_Graph::PaintText(hdc, rect, GameName, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
	}
	break;
	case GAME_PAUSE:
	{
		//经验商人
		if (pause_State==2)
		{
			Content = L"花费25经验购买如下：";
			rect.X = (float)(wnd_width - scn_width)/2;
			rect.Y = (float)(wnd_height - scn_height)/2;
			rect.Width = 33 * 18.00;
			rect.Height = 33 * 5.00;
			FontHeight = 20;
			T_Graph::PaintText(hdc,rect,Content,(REAL)FontHeight,L"黑体",Color::White,FontStyleBold,StringAlignmentNear);
		}
		//金币商人
		if (pause_State==3)
		{
			Content = L"花费25金币购买如下：";
			rect.X = (float)(wnd_width - scn_width) / 2;
			rect.Y = (float)(wnd_height - scn_height) / 2;
			rect.Width = 33 * 18.00;
			rect.Height = 33 * 5.00;
			FontHeight = 20;
			T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
		}
	}
	break;
	default:
		break;
	}
}
//碰撞检测函数
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
		//打斗处理
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
		player->SetAggressivity(player->GetAggressivity() + sp->GetAggressivity()); //攻击力增加
		player->SetDefense(player->GetDefense() + sp->GetDefense());				//防御力增加
		player->SetLifeValue(player->GetLifeValue() + sp->GetLifeValue());			//生命值增加
		sp->SetDead(true);
		break;
	case 8:
		GameState = GAME_PAUSE;
		pause_State = 3;
		delete gameMenu;
		gameMenu = NULL;
		LoadGameMenu(GAME_PAUSE);
		break;
	//碰撞到经验商人
	case 9:
		GameState = GAME_PAUSE;
		pause_State = 2;
		delete gameMenu;
		gameMenu = NULL;
		LoadGameMenu(GAME_PAUSE);
		break;
	//碰到怪物手册道具
	case 10:
		handbook = true;
		sp->SetVisible(false);
		sp->SetDead(true);
		break;

	default:
		break;
	}


}
//打斗时的画面显示
void MoTaGame::DisplayCombat(T_Sprite *sp,HDC hdc)
{
	wstring Content = L"";
	Gdiplus::RectF rect;
	int FontHeight=0;
	int title_wh = 33;
	int x = (wnd_width-scn_width)/2+title_wh;
	int y = (wnd_height - scn_height) / 2+title_wh;
	//红色边框
	//T_Graph::PaintBlank(hdc, 0, 0, title_wh * 16+20, title_wh * 8+20,Color::OrangeRed,100);
	//黑色背景
	T_Graph::PaintBlank(hdc, x, y, title_wh * 16, title_wh *8, Color::Black, 190);
	//怪物战斗信息
	Content = L"生命值：";
	Content.append(T_Util::int_to_wstring(sp->GetLifeValue()));
	Content.append(L"\n\n攻击力:");
	Content.append(T_Util::int_to_wstring(sp->GetAggressivity()));
	Content.append(L"\n\n防御力：");
	Content.append(T_Util::int_to_wstring(sp->GetDefense()));
	rect.X =(float) (x + 100);
	rect.Y = (float)(y + 20);
	rect.Width = (float)title_wh*3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 10;
	T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	//VS
	wchar_t *vs = L"V\nS";
	rect.X = rect.X + rect.Width+15;
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 20;
	T_Graph::PaintText(hdc, rect, vs, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentCenter);
	//勇士战斗战斗
	Content = T_Util::int_to_wstring(player->GetLifeValue());
	Content.append(L":生命\n\n");
	Content.append(T_Util::int_to_wstring(player->GetAggressivity()));
	Content.append(L":攻击力\n\n");
	Content.append(T_Util::int_to_wstring(player->GetDefense()));
	Content.append(L":防御力");
	rect.X = (float)(x+ title_wh * 13 - 100);
	rect.Y = (float)(y + 20);
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 6;
	FontHeight = 10;
	T_Graph::PaintText(hdc, rect, Content, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	//怪物
	FontHeight = 20;
	wchar_t *NpcName = L"怪 物";
	rect.X = (float)(x + 10);
	rect.Y = (float)(y + title_wh * 6);
	rect.Width = (float)title_wh * 3;
	rect.Height = (float)title_wh * 2+5;
	T_Graph::PaintText(hdc, rect, NpcName, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	//勇士
	wchar_t *PlayName = L"勇 士";
	rect.X = (float)(x + 13*title_wh-10);
	rect.Y = (float)(y + title_wh * 6);
	T_Graph::PaintText(hdc, rect, PlayName, (REAL)FontHeight, L"黑体", Color::White, FontStyleBold, StringAlignmentNear);
	//画怪兽的图片
	//T_Graph::PaintFrame(sp->GetImage()->GetBmpHandle(), hdc,x,y+2*title_wh,4,1,32,32,3);
	T_Graph::PaintRegion(sp->GetImage()->GetBmpHandle(),hdc,x,y+2*title_wh,0,0,32,32,3);
	//画玩家图片
	T_Graph::PaintRegion(player->GetImage()->GetBmpHandle(),hdc,x+13*title_wh,y+2*title_wh,0,0,32,32,3);
}
//显示怪物手册
void MoTaGame::DisplayHandBook(HDC hdc, vSpriteSet npcSet)
{
	int title_wh = 33;
	int x = (wnd_width - scn_width) / 2 + title_wh;
	int y = (wnd_height - scn_height) / 2 + title_wh;
	int count = 0;
	Gdiplus::RectF rect;
	int FontHeight;
	//画图形
	T_Graph::PaintBlank(hdc, x, y, title_wh * 16, title_wh * 11, Color::Black, 190);
	vSpriteSet tempSet;
	vSpriteSet::iterator it;
	for (it = npcSet.begin(); it != npcSet.end(); it++)
	{
		//npc为怪物类型
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
		Content = L"攻击力：";
		Content.append(T_Util::int_to_wstring((*it)->GetAggressivity()));
		Content.append(L"   防御力：");
		Content.append(T_Util::int_to_wstring((*it)->GetDefense()));
		Content.append(L"   金钱：");
		Content.append(T_Util::int_to_wstring((*it)->GetMoney()));
		Content.append(L"   经验:");
		Content.append(T_Util::int_to_wstring((*it)->GetScore()));
		rect.X = (float)x + 2 * title_wh;
		rect.Y = (float)y + count*title_wh;
		rect.Width = (float)15 * title_wh;
		rect.Height = (float)title_wh;
		FontHeight = 10;
		T_Graph::PaintText(hdc, rect, Content, (float)FontHeight, L"楷体", Color::White, FontStyleBold, StringAlignmentNear);
		count++;
	}
	rect.X = (float)x;
	rect.Y = (float)y + title_wh * 10;
	rect.Width = (float)16 * title_wh;
	rect.Height = (float)title_wh;
	T_Graph::PaintText(hdc, rect, L"--按下Backspace键退出--", (float)FontHeight, L"楷体", Color::White, FontStyleBold, StringAlignmentCenter);
}
//显示金钱购买菜单
void MoTaGame::DisplayMoneyBuyMenu(HDC hdc)
{

}
//玩家是否能与该怪物战斗
BOOL MoTaGame::IsBattle(T_Sprite * sp)
{
	int pa = player->GetAggressivity();
	int pd = player->GetDefense();
	int pl = player->GetLifeValue();
	//怪物信息
	int sa = sp->GetAggressivity();
	int sd = sp->GetDefense();
	int sl = sp->GetLifeValue();

	//玩家攻击力小于怪物防御力不能战斗
	if (pa <= sd)
	{
		return false;
	}
	else
	{

		//怪物攻击力小于玩家防御力，可以战斗
		if (sa <= pd)
		{
			return true;
		}

		while (sl>0 && pl>0)
		{
			sl = sl - (pa - sd);
			pl = pl - (sa - pd);
		}
		//如果不能战胜怪物，那么不能战斗
		if (pl <= 0)
		{
			return false;
		}
		//如果可以战胜怪物，那么可以战斗
		if (sl <= 0)
		{
			return true;
		}
	}
}
//判断是否为同样的怪兽
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
//集合中是否存在该怪兽
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
	//非游戏运行状态
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
	//游戏运行状态
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
	/*玩家控制*/
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

					if (GameState==GAME_PAUSE&&pause_State==2)
					{
						switch (gameMenu->GetMenuIndex())
						{
							//购买攻击力
						case 0:
							if (player->GetScore()>25)
							{
								player->SetAggressivity(player->GetAggressivity()+5);
								player->SetScore(player->GetScore() - 25);
							}
							break;
							//购买防御力
						case 1:
							if (player->GetScore()>25)
							{
								player->SetDefense(player->GetDefense()+5);
								player->SetScore(player->GetScore()-25);
							}
							break;
							//返回游戏
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
							//购买攻击力
						case 0:
							if (player->GetMoney()>25)
							{
								player->SetAggressivity(player->GetAggressivity() + 5);
								player->SetMoney(player->GetMoney() - 25);
							}
							break;
							//购买防御力
						case 1:
							if (player->GetMoney()>25)
							{
								player->SetDefense(player->GetDefense() + 5);
								player->SetMoney(player->GetMoney() - 25);
							}
							break;
							//返回游戏
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
						pause_State = 1;		//查看怪物手册
					}
				}
			}
		}
		//按下Backspace键 返回游戏
		if (GameState==GAME_PAUSE&&pause_State==1)
		{
			if (GetAsyncKeyState(VK_BACK)<0)
			{
				if (handbook == true)
				{
					GameState = GAME_RUN;
					pause_State = 0;		//查看怪物手册
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
