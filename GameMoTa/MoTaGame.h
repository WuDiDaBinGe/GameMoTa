#pragma once
#include "TinyEngine\T_Engine.h"
#include "TinyEngine\T_Menu.h"
#include "TinyEngine\T_Scene.h"
typedef vector<T_Sprite*> vSpriteSet;
class MoTaGame :
	public T_Engine
{
private:
	//游戏有关变量---------------------------------
	int wnd_width, wnd_height;	//窗口的宽高
	int scn_width, scn_height;	//场景的宽高
	int helpPageIndex;			//帮助菜单标号
	int blue_key_num, yellow_key_num, red_key_num;//玩家所拥有的三种三色钥匙的数目
	int currentLevel;			//当前楼层
	//与图片资源相关的变量--------------------------
	T_Graph* yellowKey;			//状态栏黄色钥匙图片
	T_Graph* redKey;			//状态栏红色钥匙图片
	T_Graph* blueKey;			//状态栏蓝色钥匙图片
	T_Graph* player_img;		//状态栏玩家图片
	//游戏对象类相关变量-----------------------------
	T_Scene* t_scene;			//游戏场景
	T_Menu* gameMenu;			//游戏菜单
	T_Sprite* player;			//游戏玩家
	vSpriteSet  npc_set;				//npc集合
	//游戏帧动画序列--------------------------------
	static int FRAME_LEFT[20];		//左方向的帧动画
	static int FRAME_RIGHT[20];		//右方向的帧动画
	static int FRAME_UP[20];			//上方向的帧动画
	static int FRAME_DOWN[20];		//下方向的帧动画

public:
	virtual ~MoTaGame(void);
	MoTaGame(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);
	//与游戏加载相关函数--------------------------
	void LoadGameMenu(int type);				//加载不同的菜单函数
	void LoadMap(const char* MapFilepath);		//加载地图
	void LoadPlayer();							//加载玩家角色
	void LoadImageRes();						//加载游戏图片
	void LoadNpc(const char * filePath);	    //加载npc
	//游戏状态更新类函数---------------------------
	void UpdatePlayerPos(int dir);	   //更新玩家位置
	void UpdateFrames();			   //更新动作帧函数
	//游戏细节处理函数-----------------------------
	void displayInfo(HDC hdc);
	void Collide(T_Sprite *sp);
	//其他函数-----------------------------------
	void setMenuPara(wstring* menuItem,	//菜单项文字数组
					 int itemSize,		//菜单项个数
					 int m_w,int m_h,	//菜单项宽高
					 int posType);		//菜单项布局




	// 重载T_Engine类中的虚函数实现游戏功能
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};

