#pragma once
#include "TinyEngine\T_Engine.h"
#include "TinyEngine\T_Menu.h"
#include "TinyEngine\T_Scene.h"
class MoTaGame :
	public T_Engine
{
private:
	//��Ϸ�йر���----------------------
	int wnd_width, wnd_height;
	int helpPageIndex;			//�����˵����
	//��Ϸ��������ر���----------------
	T_Scene* t_scene;			//��Ϸ����
	T_Menu* gameMenu;			//��Ϸ�˵�
public:
	virtual ~MoTaGame(void);
	MoTaGame(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);
	//����Ϸ������غ���--------------------------
	void LoadGameMenu(int type);			//���ز�ͬ�Ĳ˵�����
	//��Ϸ״̬�����ຯ��---------------------------
	//��Ϸϸ�ڴ�����-----------------------------
	void displayInfo(HDC hdc);
	//��������-----------------------------------
	void setMenuPara(wstring* menuItem,			//�˵�����������
					 int itemSize,				//�˵������
					 int m_w,int m_h,			//�˵�����
					 int posType);				//�˵����
	// ����T_Engine���е��麯��ʵ����Ϸ����
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};

