#pragma once
#include "TinyEngine\T_Engine.h"
#include "TinyEngine\T_Menu.h"
#include "TinyEngine\T_Scene.h"
typedef vector<T_Sprite*> vSpriteSet;
class MoTaGame :
	public T_Engine
{
private:
	//��Ϸ�йر���---------------------------------
	int wnd_width, wnd_height;	//���ڵĿ��
	int scn_width, scn_height;	//�����Ŀ��
	int helpPageIndex;			//�����˵����
	int blue_key_num, yellow_key_num, red_key_num;//�����ӵ�е�������ɫԿ�׵���Ŀ
	int currentLevel;			//��ǰ¥��
	//��ͼƬ��Դ��صı���--------------------------
	T_Graph* yellowKey;			//״̬����ɫԿ��ͼƬ
	T_Graph* redKey;			//״̬����ɫԿ��ͼƬ
	T_Graph* blueKey;			//״̬����ɫԿ��ͼƬ
	T_Graph* player_img;		//״̬�����ͼƬ
	//��Ϸ��������ر���-----------------------------
	T_Scene* t_scene;			//��Ϸ����
	T_Menu* gameMenu;			//��Ϸ�˵�
	T_Sprite* player;			//��Ϸ���
	vSpriteSet  npc_set;				//npc����
	//��Ϸ֡��������--------------------------------
	static int FRAME_LEFT[20];		//�����֡����
	static int FRAME_RIGHT[20];		//�ҷ����֡����
	static int FRAME_UP[20];			//�Ϸ����֡����
	static int FRAME_DOWN[20];		//�·����֡����

public:
	virtual ~MoTaGame(void);
	MoTaGame(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);
	//����Ϸ������غ���--------------------------
	void LoadGameMenu(int type);				//���ز�ͬ�Ĳ˵�����
	void LoadMap(const char* MapFilepath);		//���ص�ͼ
	void LoadPlayer();							//������ҽ�ɫ
	void LoadImageRes();						//������ϷͼƬ
	void LoadNpc(const char * filePath);	    //����npc
	//��Ϸ״̬�����ຯ��---------------------------
	void UpdatePlayerPos(int dir);	   //�������λ��
	void UpdateFrames();			   //���¶���֡����
	//��Ϸϸ�ڴ�����-----------------------------
	void displayInfo(HDC hdc);
	void Collide(T_Sprite *sp);
	//��������-----------------------------------
	void setMenuPara(wstring* menuItem,	//�˵�����������
					 int itemSize,		//�˵������
					 int m_w,int m_h,	//�˵�����
					 int posType);		//�˵����




	// ����T_Engine���е��麯��ʵ����Ϸ����
	void GameInit();								// ��Ϸ��ʼ��	
	void GameLogic();								// ��Ϸ�߼�����
	void GameEnd();									// ��Ϸ��������
	void GamePaint(HDC hdc);						// ��Ϸ��ʾ
	void GameKeyAction(int Action = KEY_SYS_NONE);	// ������Ϊ����	
	void GameMouseAction(int x, int y, int Action); // �����Ϊ����	
};

