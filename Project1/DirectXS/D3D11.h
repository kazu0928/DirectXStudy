#pragma once

#include "CommonEnv.h"
#include "Window.h"
#include <d3dCompiler.h>
#include "DirectXMath.h"
#pragma comment(lib,"d3dCompiler.lib")
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体
struct CB_SIMPLE
{
	DirectX::XMFLOAT4X4 mWVP;//ワールド、ビュー、射影の合成変換行列
};
struct SIMPLE_VERTEX
{
	DirectX::XMFLOAT3 Pos;
};


class D3DX
{
public:
	D3DX ();
	~D3DX();
	//初期化
	HRESULT InitD3D ();//Direct3Dの初期化
	HRESULT InitShader();//shaderの初期化
	void DestroyD3D ();
	//デバイス
	ID3D11Device* d_pDevice;
	IDXGISwapChain* d_pSwapChain;
	ID3D11DeviceContext* d_pDeviceContext;
	//ビュー
	ID3D11RenderTargetView* d_pRenderTargetView;
	ID3D11DepthStencilView* d_pDepthStencilView;
	ID3D11Texture2D* d_pDepthStencil;
	ID3D11RasterizerState* d_pRasterizerState;

	//
	ID3D11InputLayout* d_pVertexLayout;
	ID3D11VertexShader* d_pVertexShader;
	ID3D11PixelShader* d_pPixelShader;
	ID3D11Buffer* d_pConstantBuffer;
	//モデルごとに一つ
	ID3D11Buffer* d_pVertexBuffer;

	//ラッパー関数
	//画面更新
	void UpdateDisplay();
	//画面を単色で塗りつぶす
	void RenderClearColor(float[4]);
	//ポリゴンの作成
	HRESULT CreateVertexBuffer(SIMPLE_VERTEX[],UINT);
};