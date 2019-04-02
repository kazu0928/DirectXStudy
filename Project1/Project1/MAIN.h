//ヘッダーファイルのインクルード
#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
//必要なライブラリファイルのロード
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define APP_NAME "画面のクリア、背景塗りつぶし"
//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}

//
//
//
class MAIN
{
public:
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCSTR);
	HRESULT InitD3D();
	LRESULT MsgProc(HWND,UINT,WPARAM,LPARAM);
	void Loop();//アプリケーションループ（メッセージループ、アプリケーション処理）
	void App();//アプリケーション処理　親：Loop()
	void Render();//レンダリング
	void DestroyD3D();

	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
};
