#pragma once
#include "TinyEngine\T_Engine.h"
#include "TinyEngine\T_Menu.h"
#include "TinyEngine\T_Scene.h"
class MoTaGame :
	public T_Engine
{
private:
	//游戏有关变量----------------------
	int wnd_width, wnd_height;
	int helpPageIndex;			//帮助菜单标号
	//游戏对象类相关变量----------------
	T_Scene* t_scene;			//游戏场景
	T_Menu* gameMenu;			//游戏菜单
public:
	virtual ~MoTaGame(void);
	MoTaGame(HINSTANCE h_instance,
		LPCTSTR sz_winclass, LPCTSTR sz_title,
		WORD icon = NULL, WORD sm_icon = NULL,
		int winwidth = 800, int winheight = 600);
	//与游戏加载相关函数--------------------------
	void LoadGameMenu(int type);			//加载不同的菜单函数
	//游戏状态更新类函数---------------------------
	//游戏细节处理函数-----------------------------
	void displayInfo(HDC hdc);
	//其他函数-----------------------------------
	void setMenuPara(wstring* menuItem,			//菜单项文字数组
					 int itemSize,				//菜单项个数
					 int m_w,int m_h,			//菜单项宽高
					 int posType);				//菜单项布局
	// 重载T_Engine类中的虚函数实现游戏功能
	void GameInit();								// 游戏初始化	
	void GameLogic();								// 游戏逻辑处理
	void GameEnd();									// 游戏结束处理
	void GamePaint(HDC hdc);						// 游戏显示
	void GameKeyAction(int Action = KEY_SYS_NONE);	// 按键行为处理	
	void GameMouseAction(int x, int y, int Action); // 鼠标行为处理	
};

