#pragma once
#include "TinyEngine\T_Engine.h"
#include "TinyEngine\T_Menu.h"
#include "TinyEngine\T_Scene.h"
#define  TOTAL_LEVEL    10
typedef vector<T_Sprite*> vSpriteSet;
typedef pair<int, int>Pos;
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
	int arrivedLevel;			//到达过的楼层
	static const char * mapfiles[TOTAL_LEVEL];		//地图文件
	static const char *npcfiles[TOTAL_LEVEL];		//npc文件
	static Pos  startPos[TOTAL_LEVEL];	//每一层的入口楼梯坐标
	static Pos  startNpcPos[TOTAL_LEVEL];	//每一层的npc开始坐标
	static Pos endPos[TOTAL_LEVEL];	//每一层的出口楼梯坐标

	//与图片资源相关的变量--------------------------
	T_Graph* yellowKey;			//状态栏黄色钥匙图片
	T_Graph* redKey;			//状态栏红色钥匙图片
	T_Graph* blueKey;			//状态栏蓝色钥匙图片
	T_Graph* player_img;		//状态栏玩家图片
	//游戏对象类相关变量-----------------------------
	T_Scene* t_scene;			//游戏场景
	T_Menu* gameMenu;			//游戏菜单
	T_Sprite* player;			//游戏玩家
	T_Sprite * battleNpc;		//正在战斗的npc
	vSpriteSet  npc_set;		//npc集合
	vector<vSpriteSet> npc_vec;		//全部楼层的npc集合

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
	void LoadGameLevel(int level);
	//游戏状态更新类函数---------------------------
	void UpdatePlayerPos(int dir);	   //更新玩家位置
	void UpdateFrames();			   //更新动作帧函数
	void Battling();				   //战斗函数
	//游戏细节处理函数-----------------------------
	void DisplayInfo(HDC hdc);			//画游戏的各种信息
	void Collide(T_Sprite *sp);			//碰撞逻辑处理
	void DisplayCombat(T_Sprite* sp,HDC hdc);		//打斗画面显示
	
	//其他函数-----------------------------------
	void setMenuPara(wstring* menuItem,	//菜单项文字数组
					 int itemSize,		//菜单项个数
					 int m_w,int m_h,	//菜单项宽高
					 int posType);		//菜单项布局
	BOOL IsBattle(T_Sprite *sp);		//玩家是否能战斗

	// 重载T_Engine类中的虚函数实现游戏功能
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};

