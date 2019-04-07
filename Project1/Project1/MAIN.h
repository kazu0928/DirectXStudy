//�w�b�_�[�t�@�C���̃C���N���[�h
#define _WIN32_DCOM

#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include "DirectXMath.h"
//��
#include <xaudio2.h>
#include <mmsystem.h>

//�K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//�萔��`
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����
#define MAX_MODEL 1000 //���f���ő吔
#define MAX_SHOT 400
#define MAX_WAV 100 //WAV�T�E���h�ő吔
#define APP_NAME L"�|���S�������\��"
//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//���_�̍\����
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos; //�ʒu
};
//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
struct SIMPLESHADER_CONSTANT_BUFFER
{
	DirectX::XMFLOAT4X4 mWVP;//���[���h�A�r���[�A�ˉe�̍����ϊ��s��
};
//
//
//MAIN�N���X�@��`
class MAIN
{
public:
	MAIN();
	~MAIN();
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Loop();
	void App();
	void Render();
	void DestroyD3D();
	//���A�v���ɂЂƂ�
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
	ID3D11RasterizerState* m_pRasterizerState;
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������
	ID3D11Buffer* m_pVertexBuffer;
};
