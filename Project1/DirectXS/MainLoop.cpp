#include "MainLoop.h"

Window* g_pWindow = NULL;
D3DX* g_pD3DX = NULL;
Main* g_pMain = NULL;

using namespace DirectX;

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
			//頂点を定義
			SIMPLE_VERTEX vertices[] =
			{
				XMFLOAT3(-0.5,-0.5,0),//頂点1	
				XMFLOAT3(-0.5,0.5,0), //頂点2
				XMFLOAT3(0.5,-0.5,0),  //頂点3
				XMFLOAT3(0.5,0.5,0), //頂点4	
			};
			if (SUCCEEDED(g_pD3DX->CreateVertexBuffer(vertices,sizeof(vertices)/sizeof(vertices[0]))))
			{
				void(*pApp)() = Main::App;
				g_pWindow->Loop(pApp);
			}
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
	//画面を単色で塗りつぶす
	float ClearColor[4] = { 0,0,0.3f,1 };
	g_pD3DX->RenderClearColor(ClearColor);
	
	XMFLOAT4X4 mWorld, mView, mProj;


	//画面更新
	g_pD3DX->UpdateDisplay();

}



