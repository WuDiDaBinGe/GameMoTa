#include "MoTaGame.h"

// WinMain����
int WINAPI WinMain(HINSTANCE h_instance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	LPCTSTR WinTitle = L"ħ��V1.0";  // ��Ϸ���ڱ�������������
	// ���Ҫ�趨���ں�exe�ļ���ͼ�꣬�뽫ͼ����Դ���뵽������
	// Ȼ��ͼ��ı�ʶ�滻���캯���е�����NULLֵ
	// ���磺���ͼ���ʶΪIDI_TANKNEW��������NULLֵ����ΪIDI_TANKNEW
	MoTaGame* mygame = new MoTaGame(h_instance, WIN_CLASS, WinTitle,
		NULL, NULL, WIN_WIDTH, WIN_HEIGHT);
	// ���뽫����ľ�ָ̬��ָ����õ�����
	T_Engine::pEngine = mygame;
	// ������Ϸ�����ˢ��Ƶ��
	mygame->SetFrame(70);

	// �������Ҫ��ȫ��Ļ��ʽ������������������д���
	// ���������������������ɾ����ע�͵��������д���
	//mygame->SetFullScreen(TRUE);
	// ������Ϸ���棨��ʾ��Ϸ���ڲ�������Ϸ��ѭ����
	mygame->StartEngine();

	return TRUE;
}