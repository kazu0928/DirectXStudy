#include "MainLoop.h"

Window* g_pWindow = NULL;

//�A�v���P�[�V���������@�A�v���̃��C���֐�
void App()
{

}
//�G���g���[�|�C���g
INT WINAPI WinMain(HINSTANCE _hIns, HINSTANCE _hIns2, LPSTR _lps, INT _int)
{
	g_pWindow = new Window;
	if (g_pWindow == NULL) return 0;
	if (SUCCEEDED(g_pWindow->InitWindow(_hIns, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
	{
		void(*pApp)() = App;
		g_pWindow->Loop(pApp);
	}
	delete g_pWindow;
	return 0;
}

