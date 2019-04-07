#include "MAIN.h"
using namespace DirectX;
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
//
MAIN::MAIN()
{
	ZeroMemory(this, sizeof(MAIN));
}
//
//
//
MAIN::~MAIN()
{
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
	 //弾(m_Shot)と的(m_Model)の当たり判定
	 for (int i = 0; i < m_iNumShot; i++)
	 {
		 for (int k = 0; k < m_iNumModel; k++)
		 {
			 XMFLOAT3 in_out_data;

			 DirectX::XMStoreFloat3(&in_out_data, DirectX::XMVector3Length((DirectX::XMLoadFloat3(&m_Shot[i].vPos) - DirectX::XMLoadFloat3(&m_Model[k].vPos))));
			 //xyzどれも同じものが入る
			 if (in_out_data.x < 1.0f)
			 {
				 //消去処理
				 //当たっているモデルを消す場合、そのモデルのデータに、配列最後のモデルのデータを上書き
				 MODEL tmp;
				 memcpy(&m_Shot[i], &m_Shot[m_iNumShot - 1], sizeof(MODEL));
				 m_iNumShot--;
				 memcpy(&m_Model[k], &m_Model[m_iNumModel - 1], sizeof(MODEL));
				 m_iNumModel--;
				 break;
			 }
		 }
	 }
	 static int DasisugiBousi = 0;//弾　出しすぎ防止用カウンター 400フレームに１発でるようにする
	 DasisugiBousi++;
	 if (DasisugiBousi > 400 && m_iNumShot < MAX_SHOT && GetKeyState(VK_SPACE) & 0x80)
	 {
		 DasisugiBousi = 0;
		 m_Shot[m_iNumShot].vPos = XMFLOAT3(0, 0, -2);
		 m_Shot[m_iNumShot].vColor = XMFLOAT4(1, 1, 1, 1);//弾は白
		 m_iNumShot++;
	 }
	 //弾移動
	 for (int i = 0; i < m_iNumShot; i++)
	 {
		 m_Shot[i].vPos.z += 0.001f;
		 //キーボード入力 矢印キーで弾を操作
		 if (GetKeyState(VK_LEFT) & 0x80)//左移動
		 {
			 m_Shot[i].vPos.x -= 0.001f;
		 }
		 if (GetKeyState(VK_RIGHT) & 0x80)//右移動
		 {
			 m_Shot[i].vPos.x += 0.001f;
		 }
		 if (GetKeyState(VK_UP) & 0x80)//左移動
		 {
			 m_Shot[i].vPos.y += 0.001f;
		 }
		 if (GetKeyState(VK_DOWN) & 0x80)//右移動
		 {
			 m_Shot[i].vPos.y -= 0.001f;
		 }
	 }
	 //まと移動
	 for (int i = 0; i < m_iNumModel; i++)
	 {
		 m_Model[i].vPos.z -= 0.002f;
	 }

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
	//シェーダー初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//ポリゴン作成
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//後
//シェーダーを作成　頂点レイアウトを定義
HRESULT MAIN::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。シェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	//D3DCompileFileはD3DXがないため書き直し後
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}
//
//
//
HRESULT MAIN::InitPolygon()
{
	//バーテックスバッファー作成
	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//バーテックスバッファーをセット
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//全てのモデルで同じポリゴン。同じバーテックスバッファーを使う。モデルごとに異なるのは、モデルの位置と色。	
	for (int i = 0; i < MAX_MODEL; i++)
	{
		m_Model[i].vPos = XMFLOAT3(float(rand()) / 5000.0f - 3.0f, float(rand()) / 5000.0f - 3.0f, float(rand()) / 5000.0f + 20.0f);//初期位置はランダム
		m_Model[i].vColor = XMFLOAT4(float(rand()) / 32767.0f, float(rand()) / 32767.0f, float(rand()) / 32767.0f, 1.0f);//色もランダム
	}
	m_iNumModel = MAX_MODEL;

	return S_OK;
}
//
//
//シーンを画面にレンダリング
void MAIN::Render()
{
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	FLOAT ClearColor[4] = { 0,0,1,1 };// クリア色作成　RGBAの順
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//画面クリア
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファクリア

	/*DirectXMathライブラリでは演算に用いる場合はXMMATRIXを、入れ物はXMFLOAT4X4を使用*/
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	//ワールドトランスフォーム（絶対座標変換）
	//XMStoreFloat4x4(&mWorld, DirectX::XMMatrixRotationY(timeGetTime() / 100.0f));//XMMATRIXからXMFLOAT4X4へ値を格納,回転

	// ビュートランスフォーム（視点座標変換）
	XMFLOAT3 vEyePt(0.0f, 0.0f, -2.0f); //カメラ（視点）位置
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	XMStoreFloat4x4(&mView, DirectX::XMMatrixLookAtLH(XMLoadFloat3(&vEyePt),XMLoadFloat3(&vLookatPt),XMLoadFloat3(&vUpVec)));
	// プロジェクショントランスフォーム（射影変換）
	XMStoreFloat4x4(&mProj, DirectX::XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f));

	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//プリミティブをレンダリング 　複数なので、ワールドトランスフォームとそれを渡す部分をループ内にいれる（モデルごとに行う）
	for (int i = 0; i < m_iNumShot; i++)
	{
		//ワールドトランスフォーム（絶対座標変換）
		XMFLOAT4X4 mTrans, mScale;
		XMStoreFloat4x4(&mScale, DirectX::XMMatrixScaling(0.5, 0.5, 0.5));
		XMStoreFloat4x4(&mTrans,DirectX::XMMatrixTranslation(m_Shot[i].vPos.x, m_Shot[i].vPos.y, m_Shot[i].vPos.z));
		mWorld = mTrans;
		//シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//ワールド、カメラ、射影行列を渡す
			XMFLOAT4X4 m;
			XMStoreFloat4x4(&m,XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProj));
			DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
			cb.mWVP = m;
			//カラーを渡す
			cb.vColor = m_Shot[i].vColor;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->Draw(3, 0);
	}
	//1000個的レンダリング
	for (int i = 0; i < m_iNumModel; i++)
	{
		//ワールドトランスフォーム（絶対座標変換）
		XMFLOAT4X4 mTrans, mScale;
		XMStoreFloat4x4(&mTrans, DirectX::XMMatrixTranslation(m_Model[i].vPos.x, m_Model[i].vPos.y, m_Model[i].vPos.z));
		mWorld = mTrans;
		//シェーダーのコンスタントバッファーに各種データを渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//ワールド、カメラ、射影行列を渡す
			XMFLOAT4X4 m;
			XMStoreFloat4x4(&m, XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProj));
			DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
			cb.mWVP = m;
			//カラーを渡す
			cb.vColor = m_Model[i].vColor;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->Draw(3, 0);
	}

	m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに）	

	//////シェーダーのコンスタントバッファーに各種データを渡す	
	////D3D11_MAPPED_SUBRESOURCE pData;
	////SIMPLESHADER_CONSTANT_BUFFER cb;
	////if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	////{
	////	//ワールド、カメラ、射影行列を渡す
	////	XMFLOAT4X4 m;
	////	XMStoreFloat4x4(&m, XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProj));
	////	XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
	////	cb.mWVP = m;
	////	//カラーを渡す
	////	XMFLOAT4 vColor(1, 0, 0, 1);
	////	cb.vColor = vColor;
	////	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	////	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	////}
	//////頂点インプットレイアウトをセット
	////m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//////プリミティブ・トポロジーをセット
	////m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//////プリミティブをレンダリング
	////m_pDeviceContext->Draw(3, 0);

	////m_pSwapChain->Present(0, 0);//画面更新（バックバッファをフロントバッファに）	
}