#include "D3D11.h"

extern Window* g_pWindow;//Window�N���X

//Direct3D������
HRESULT D3DX::InitD3D ()
{
#pragma region �f�B�X�v���C�A�_�v�^�[��\���f�o�C�X�ƃ����_�����O�Ɏg�p����X���b�v �`�F�[���̍쐬
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

	if (FAILED (D3D11CreateDeviceAndSwapChain (NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, pFeatureLevel, &m_pDeviceContext)))
	{
		return FALSE;
	}

#pragma endregion

#pragma region �r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;
	//�X���b�v�`�F�C������`���o�b�t�@�̎擾
	m_pSwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//�����_�[�^�[�Q�b�g�r���[�ƕ`���o�b�t�@�Ƃ̊֘A�t��
	m_pDevice->CreateRenderTargetView (pBackBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE (pBackBuffer);
	//�[�x�X�e���V���r���[�̍쐬
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
	m_pDevice->CreateTexture2D (&descDepth, NULL, &m_pDepthStencil);
	m_pDevice->CreateDepthStencilView (m_pDepthStencil, NULL, &m_pDepthStencilView);
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	m_pDeviceContext->OMSetRenderTargets (1, &m_pRenderTargetView, m_pDepthStencilView);


#pragma endregion

}