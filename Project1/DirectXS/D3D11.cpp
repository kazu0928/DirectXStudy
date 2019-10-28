#include "D3D11.h"

extern Window* g_pWindow;//Windowクラス
extern D3DX* g_pD3DX;
using namespace DirectX;

D3DX::D3DX ()
{
	ZeroMemory (this, sizeof (D3DX));
}
D3DX::~D3DX ()
{
	DestroyD3D ();
}
#pragma region 初期化
//Direct3D初期化
HRESULT D3DX::InitD3D ()
{
#pragma region ディスプレイアダプターを表すデバイスとレンダリングに使用するスワップ チェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory (&sd, sizeof (sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_pWindow->w_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	if (FAILED (D3D11CreateDeviceAndSwapChain (NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &d_pSwapChain, &d_pDevice, pFeatureLevel, &d_pDeviceContext)))
	{
		return FALSE;
	}
#pragma endregion

#pragma region ビューの作成
	ID3D11Texture2D *pBackBuffer;
	//スワップチェインから描画先バッファの取得
	d_pSwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//レンダーターゲットビューと描画先バッファとの関連付け
	d_pDevice->CreateRenderTargetView (pBackBuffer, NULL, &d_pRenderTargetView);
	SAFE_RELEASE (pBackBuffer);
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
	d_pDevice->CreateTexture2D (&descDepth, NULL, &d_pDepthStencil);
	d_pDevice->CreateDepthStencilView (d_pDepthStencil, NULL, &d_pDepthStencilView);
	//レンダーターゲットビューと深度ステンシルビューをパイプラインにバインド
	d_pDeviceContext->OMSetRenderTargets (1, &d_pRenderTargetView, d_pDepthStencilView);
#pragma endregion

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d_pDeviceContext->RSSetViewports (1, &vp);
	//ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory (&rdc, sizeof (rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;

	d_pDevice->CreateRasterizerState (&rdc, &d_pRasterizerState);
	d_pDeviceContext->RSSetState (d_pRasterizerState);
	//シェーダ初期化
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//シェーダの初期化
HRESULT D3DX::InitShader()
{
	//実行時コンパイルで実装
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//コンパイル
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, L"hlslコンパイル失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	//頂点シェーダ作成
	if (FAILED(d_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &d_pVertexShader )))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//インプットレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	//入力データ型の数を算出
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(d_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &d_pVertexLayout)))
	{
		return FALSE;
	}
	//ピクセルシェーダ作成
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader,&pErrors)))
	{
		MessageBox(0, L"hlslコンパイル失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(d_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &d_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファ(シェーダとアプリ間での変数等のやり取り用)
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(CB_SIMPLE);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if (FAILED(d_pDevice->CreateBuffer(&cb, NULL, &d_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}
#pragma endregion

//全てのインターフェイスをリリース
void D3DX::DestroyD3D ()
{
	SAFE_RELEASE (d_pRasterizerState);
	SAFE_RELEASE (d_pSwapChain);
	SAFE_RELEASE (d_pRenderTargetView);
	SAFE_RELEASE (d_pDepthStencilView);
	SAFE_RELEASE (d_pDepthStencil);
	SAFE_RELEASE (d_pDeviceContext);
	SAFE_RELEASE (d_pDevice);
}
//
/*↓ラッパー関数*/
//

/// <summary>
/// 画面の更新
/// </summary>
void D3DX::UpdateDisplay()
{
	d_pSwapChain->Present(0, 0);
}

/// <summary>
/// 画面を単色クリア
/// </summary>
/// <param name="ClearColor">色</param>
/// <param name="dc">DeviceContext</param>
/// <param name="rt">RenderTargetView</param>
/// <param name="ds">DepthStencilView</param>
void D3DX::RenderClearColor(float ClearColor[4])
{
	d_pDeviceContext->ClearRenderTargetView(d_pRenderTargetView, ClearColor);
	d_pDeviceContext->ClearDepthStencilView(d_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/// <summary>
/// バーテックスバッファの作成
/// </summary>
/// <param name="Vertex">頂点配列</param>
/// <param name="Vertex_Size">頂点配列サイズ</param>
/// <returns></returns>
HRESULT D3DX::CreateVertexBuffer(SIMPLE_VERTEX Vertex[],UINT Vertex_Size)
{
	//バーテックスバッファ作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SIMPLE_VERTEX) * Vertex_Size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	//初期化用データ
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = Vertex;
	if (FAILED(d_pDevice->CreateBuffer(&bd, &InitData, &d_pVertexBuffer)))
	{
		return E_FAIL;
	}
}
