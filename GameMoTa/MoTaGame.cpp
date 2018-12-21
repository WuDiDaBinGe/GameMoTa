#include "MoTaGame.h"



MoTaGame::MoTaGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title,
	WORD icon, WORD sm_icon, int winwidth, int winheight)
	:T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{
	srand((unsigned)time(NULL));

	wnd_width = winwidth;
	wnd_height = winheight;
}
void MoTaGame::GameInit()
{
}
void MoTaGame::GameLogic()
{
}
void MoTaGame::GameEnd()
{
}
void MoTaGame::GamePaint(HDC hdc)
{
}
void MoTaGame::GameKeyAction(int Action)
{
}
void MoTaGame::GameMouseAction(int x, int y, int Action)
{
}
MoTaGame::~MoTaGame()
{
}
