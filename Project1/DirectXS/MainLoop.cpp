#include "MainLoop.h"

Window* g_pWindow = NULL;
D3DX* g_pD3DX = NULL;
Main* g_pMain = NULL;

using namespace DirectX;

//�G���g���[�|�C���g
INT WINAPI WinMain(HINSTANCE _hIns, HINSTANCE _hIns2, LPSTR _lps, INT _int)
{
	g_pMain = new Main;
	g_pWindow = new Window;
	g_pD3DX = new D3DX;
	if (g_pWindow == NULL) return 0;
	if (SUCCEEDED(g_pWindow->InitWindow(_hIns, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME)))
	{
		if (SUCCEEDED (g_pD3DX->InitD3D ()))
		{
			//���_���`
			SIMPLE_VERTEX vertices[] =
			{
				XMFLOAT3(-0.5,-0.5,0),//���_1	
				XMFLOAT3(-0.5,0.5,0), //���_2
				XMFLOAT3(0.5,-0.5,0),  //���_3
				XMFLOAT3(0.5,0.5,0), //���_4	
			};
			if (SUCCEEDED(g_pD3DX->CreateVertexBuffer(vertices,sizeof(vertices)/sizeof(vertices[0]))))
			{
				void(*pApp)() = Main::App;
				g_pWindow->Loop(pApp);
			}
		}
	}
	delete g_pWindow;
	delete g_pD3DX;
	delete g_pMain;
	return 0;
}

//�A�v���P�[�V���������@�A�v���̃��C���֐�
void Main::App ()
{
	g_pMain->Render ();
}
void Main::Render ()
{
	//��ʂ�P�F�œh��Ԃ�
	float ClearColor[4] = { 0,0,0.3f,1 };
	g_pD3DX->RenderClearColor(ClearColor);
	
	//�r���[�g�����X�t�H�[��
	XMFLOAT4X4 mWorld, mView, mProj;
	//���[���h�g�����X�t�H�[��(���[���h���W)
	XMStoreFloat4x4(&mWorld, XMMatrixTranslation(0, 0, 0));
	//�r���[�g�����X�t�H�[��
	XMFLOAT3 vEyePt(0.0f, 1.0f, -2.0f);//�J�����̈ʒu
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//�����������
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//��ʒu
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat3(&vEyePt), XMLoadFloat3(&vLookatPt), XMLoadFloat3(&vUpVec)));
	//�v���W�F�N�V�����g�����X�t�H�[��
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(XM_PI/4/*FOV(���W�A��)*/, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT/*�A�X��*/, 0.1f/*�߃N���b�v�i�\���̈�܂ł̋����j*/, 110.0f/*���N���b�v�i�\���̈�j*/));
	//�V�F�[�_�̓o�^
	g_pD3DX->d_pDeviceContext->VSSetShader(g_pD3DX->d_pVertexShader,NULL,0);
	g_pD3DX->d_pDeviceContext->PSSetShader(g_pD3DX->d_pPixelShader, NULL, 0);
	//��ʍX�V
	g_pD3DX->UpdateDisplay();

}



