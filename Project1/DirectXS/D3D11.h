#pragma once

#include "CommonEnv.h"
#include "Window.h"

class D3DX
{
public:
	HRESULT InitD3D ();//Direct3DÇÃèâä˙âª

	ID3D11Device* m_pDevice;
	IDXGISwapChain* m_pSwapChain;
	ID3D11DeviceContext* m_pDeviceContext;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
};