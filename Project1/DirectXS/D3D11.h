#pragma once

#include "CommonEnv.h"
#include "Window.h"

#pragma comment(lib, "d3d11.lib")

class D3DX
{
public:
	D3DX ();
	~D3DX();

	HRESULT InitD3D ();//Direct3DÇÃèâä˙âª
	void DestroyD3D ();

	ID3D11Device* d_pDevice;
	IDXGISwapChain* d_pSwapChain;
	ID3D11DeviceContext* d_pDeviceContext;

	ID3D11RenderTargetView* d_pRenderTargetView;
	ID3D11DepthStencilView* d_pDepthStencilView;
	ID3D11Texture2D* d_pDepthStencil;
	ID3D11RasterizerState* d_pRasterizerState;

};