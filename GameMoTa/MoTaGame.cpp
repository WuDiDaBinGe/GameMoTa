#include "MoTaGame.h"



MoTaGame::MoTaGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));

	wnd_width = winwidth;
	wnd_height = winheight;
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
		if (GameState==GAME_START)
		{
			if (gameMenu != NULL) gameMenu->DrawMenu(hdc);
						 
		}
	}
	//��Ϸ����״̬
	if (GameState==GAME_RUN)
	{

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
							/*
							���������Ϸ��ʼ��������
							*/
							break;
						case 1:
							//�����Ϸ��������
							GameState == GAME_HELP;
							delete gameMenu;
							gameMenu = NULL;
							LoadGameMenu(GAME_HELP);
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
		if (GameState == GAME_START)
		{
			if (gameMenu!=NULL)
			{
				int index = gameMenu->MenuMouseClick(x,y);
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
						GameState == GAME_HELP;
						delete gameMenu;
						gameMenu = NULL;
						LoadGameMenu(GAME_HELP);
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
		}
	}
	
}
MoTaGame::~MoTaGame()
{
	
}
