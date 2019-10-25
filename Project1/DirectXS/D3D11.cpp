#include "D3D11.h"

extern Window* g_pWindow;//Windowクラス
extern D3DX* g_pD3DX;

D3DX::D3DX ()
{
	ZeroMemory (this, sizeof (D3DX));
}
D3DX::~D3DX ()
{
	DestroyD3D ();
}

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

	return S_OK;
}

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