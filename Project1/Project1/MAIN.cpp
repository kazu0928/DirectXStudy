#include "MAIN.h"
//グローバル変数
MAIN* g_pMain=NULL;
//関数プロトタイプの宣言
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//アプリケーションのエントリー関数 
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if(SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}
		//アプリ終了
		g_pMain->DestroyD3D();
		delete g_pMain;
	}
	return 0;
}
//
//
//OSから見たウィンドウプロシージャー（実際の処理はMAINクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//ウィンドウ作成
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPCSTR WindowName)
 {
	 // ウィンドウの定義
	WNDCLASSEX  wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);									/* 構造体の大きさ */
	wc.style = CS_HREDRAW | CS_VREDRAW;//ウインドウのサイズが変更されたら再描画する、という指定/* スタイル */
	wc.lpfnWndProc = WndProc;/* メッセージ処理関数 */
	wc.hInstance = hInstance;/* プログラムのハンドル */
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);/* アイコン */
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);/*カーソル*/
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);/*ブラシ*/
	wc.lpszClassName = WindowName;/*クラス名*/
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&wc);

	//ウィンドウの作成
	m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
		0,0,iWidth,iHeight,0,0,hInstance,0);
	 if(!m_hWnd) 
	 {
		 return E_FAIL;
	 }
	 //ウインドウの表示
	 ShowWindow(m_hWnd,SW_SHOW);    
	 UpdateWindow(m_hWnd) ;

	 return S_OK;
 }
//
//
//ウィンドウプロシージャー
 LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESCキーで修了
			PostQuitMessage(0);
			break;
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
 }
//
//
//メッセージループとアプリケーション処理の入り口
 void MAIN::Loop()
 {
	 // メッセージループ
	 MSG msg={0};
	 ZeroMemory(&msg,sizeof(msg));
	 while(msg.message!=WM_QUIT)
	 {
		 if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 else
		 {
			 //アプリケーションの処理はここから飛ぶ。
			 App();
		 }
	 }
	 //アプリケーションの終了
 }
 //
 //アプリケーション処理。アプリのメイン関数。
 void MAIN::App()
 {
	 Render();
 }

//DirectX初期化
HRESULT MAIN::InitD3D()
{
	//参考 http://marupeke296.com/DX10_No1_Init.html　、書籍
	//デバイスとスワップチェーンの作成（構造体作成）
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;		//スワップチェーンのバッファー数を表す値です。フロント バッファーを含みます。
	//バック バッファーの表示モードを表す DXGI_MODE_DESC 構造体です。
	sd.BufferDesc.Width=WINDOW_WIDTH;				//解像度の幅を表す値
	sd.BufferDesc.Height=WINDOW_HEIGHT;				//解像度の高さを表す値
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;//表示フォーマットを表す DXGI_FORMAT 構造体 4 成分、16 ビット符号なし正規化整数フォーマット
	sd.BufferDesc.RefreshRate.Numerator=60;			
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;//バックバッファの使用法
	sd.OutputWindow=m_hWnd;	//出力先のウィンドウハンドル
	sd.SampleDesc.Count=1;   //1xMSAA
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;	
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;	
	//スワップチェーンの作成
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )) )
	{
		return FALSE;
	}

	//レンダーターゲットビューの作成
	ID3D11Texture2D *pBackBuffer;//バックバッファのポインタ宣言
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer); //スワップチェーンからバッファを受け取る   
	m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
	SAFE_RELEASE(pBackBuffer);	//バックバッファのリリース（参照減らし）
	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	m_pDevice->CreateDepthStencilView( m_pDepthStencil, NULL, &m_pDepthStencilView );
	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド（関連付け）
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView,m_pDepthStencilView);
	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;//サイズ
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;//Z値の幅
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;//左上
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &vp );
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr=NULL;
	m_pDevice->CreateRasterizerState(&rdc,&pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDevice);
}
//
//シーンを画面にレンダリング
void MAIN::Render()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//画面クリア
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに）	
}