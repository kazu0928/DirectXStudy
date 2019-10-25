#include "MainLoop.h"

Window* g_pWindow = NULL;
D3DX* g_pD3DX = NULL;
Main* g_pMain = NULL;

//エントリーポイント
INT WINAPI WinMain(HINSTANCE _hIns, HINSTANCE _hIns2, LPSTR _lps, INT _int)
{
	g_pMain = new Main;
	g_pWindow = new Window;
	g_pD3DX = new D3DX;
	if (g_pWindow == NULL) return 0;
	if (SUCCEEDED(g_pWindow->InitWindow(_hIns, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
	{
		if (SUCCEEDED (g_pD3DX->InitD3D ()))
		{
			void (*pApp)() = Main::App;
			g_pWindow->Loop (pApp);
		}
	}
	delete g_pWindow;
	delete g_pD3DX;
	delete g_pMain;
	return 0;
}

//アプリケーション処理　アプリのメイン関数
void Main::App ()
{
	g_pMain->Render ();
}
void Main::Render ()
{
	
}

