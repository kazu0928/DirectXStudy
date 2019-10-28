#include "D3D11.h"

extern Window* g_pWindow;//Window�N���X
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
#pragma region ������
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

	if (FAILED (D3D11CreateDeviceAndSwapChain (NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &d_pSwapChain, &d_pDevice, pFeatureLevel, &d_pDeviceContext)))
	{
		return FALSE;
	}
#pragma endregion

#pragma region �r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;
	//�X���b�v�`�F�C������`���o�b�t�@�̎擾
	d_pSwapChain->GetBuffer (0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	//�����_�[�^�[�Q�b�g�r���[�ƕ`���o�b�t�@�Ƃ̊֘A�t��
	d_pDevice->CreateRenderTargetView (pBackBuffer, NULL, &d_pRenderTargetView);
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
	d_pDevice->CreateTexture2D (&descDepth, NULL, &d_pDepthStencil);
	d_pDevice->CreateDepthStencilView (d_pDepthStencil, NULL, &d_pDepthStencilView);
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h
	d_pDeviceContext->OMSetRenderTargets (1, &d_pRenderTargetView, d_pDepthStencilView);
#pragma endregion

	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d_pDeviceContext->RSSetViewports (1, &vp);
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory (&rdc, sizeof (rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;

	d_pDevice->CreateRasterizerState (&rdc, &d_pRasterizerState);
	d_pDeviceContext->RSSetState (d_pRasterizerState);
	//�V�F�[�_������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//�V�F�[�_�̏�����
HRESULT D3DX::InitShader()
{
	//���s���R���p�C���Ŏ���
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�R���p�C��
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, L"hlsl�R���p�C�����s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	//���_�V�F�[�_�쐬
	if (FAILED(d_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &d_pVertexShader )))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	//���̓f�[�^�^�̐����Z�o
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(d_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &d_pVertexLayout)))
	{
		return FALSE;
	}
	//�s�N�Z���V�F�[�_�쐬
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader,&pErrors)))
	{
		MessageBox(0, L"hlsl�R���p�C�����s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(d_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &d_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@(�V�F�[�_�ƃA�v���Ԃł̕ϐ����̂����p)
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

//�S�ẴC���^�[�t�F�C�X�������[�X
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
/*�����b�p�[�֐�*/
//

/// <summary>
/// ��ʂ̍X�V
/// </summary>
void D3DX::UpdateDisplay()
{
	d_pSwapChain->Present(0, 0);
}

/// <summary>
/// ��ʂ�P�F�N���A
/// </summary>
/// <param name="ClearColor">�F</param>
/// <param name="dc">DeviceContext</param>
/// <param name="rt">RenderTargetView</param>
/// <param name="ds">DepthStencilView</param>
void D3DX::RenderClearColor(float ClearColor[4])
{
	d_pDeviceContext->ClearRenderTargetView(d_pRenderTargetView, ClearColor);
	d_pDeviceContext->ClearDepthStencilView(d_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/// <summary>
/// �o�[�e�b�N�X�o�b�t�@�̍쐬
/// </summary>
/// <param name="Vertex">���_�z��</param>
/// <param name="Vertex_Size">���_�z��T�C�Y</param>
/// <returns></returns>
HRESULT D3DX::CreateVertexBuffer(SIMPLE_VERTEX Vertex[],UINT Vertex_Size)
{
	//�o�[�e�b�N�X�o�b�t�@�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SIMPLE_VERTEX) * Vertex_Size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	//�������p�f�[�^
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = Vertex;
	if (FAILED(d_pDevice->CreateBuffer(&bd, &InitData, &d_pVertexBuffer)))
	{
		return E_FAIL;
	}
}
