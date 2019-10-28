#pragma once

#include "CommonEnv.h"
#include "Window.h"
#include <d3dCompiler.h>
#include "DirectXMath.h"
#pragma comment(lib,"d3dCompiler.lib")
//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\����
struct CB_SIMPLE
{
	DirectX::XMFLOAT4X4 mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
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
	//������
	HRESULT InitD3D ();//Direct3D�̏�����
	HRESULT InitShader();//shader�̏�����
	void DestroyD3D ();
	//�f�o�C�X
	ID3D11Device* d_pDevice;
	IDXGISwapChain* d_pSwapChain;
	ID3D11DeviceContext* d_pDeviceContext;
	//�r���[
	ID3D11RenderTargetView* d_pRenderTargetView;
	ID3D11DepthStencilView* d_pDepthStencilView;
	ID3D11Texture2D* d_pDepthStencil;
	ID3D11RasterizerState* d_pRasterizerState;

	//
	ID3D11InputLayout* d_pVertexLayout;
	ID3D11VertexShader* d_pVertexShader;
	ID3D11PixelShader* d_pPixelShader;
	ID3D11Buffer* d_pConstantBuffer;
	//���f�����ƂɈ��
	ID3D11Buffer* d_pVertexBuffer;

	//���b�p�[�֐�
	//��ʍX�V
	void UpdateDisplay();
	//��ʂ�P�F�œh��Ԃ�
	void RenderClearColor(float[4]);
	//�|���S���̍쐬
	HRESULT CreateVertexBuffer(SIMPLE_VERTEX[],UINT);
};