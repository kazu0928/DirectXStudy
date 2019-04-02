#include "MAIN.h"
//�O���[�o���ϐ�
MAIN* g_pMain=NULL;
//�֐��v���g�^�C�v�̐錾
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
//
//
//�A�v���P�[�V�����̃G���g���[�֐� 
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE,LPSTR,INT)
{
	g_pMain=new MAIN;
	if(g_pMain != NULL)
	{
		if(SUCCEEDED(g_pMain->InitWindow(hInstance,0,0,WINDOW_WIDTH,
			WINDOW_HEIGHT,APP_NAME)))
		{
			if(SUCCEEDED(g_pMain->InitD3D()))
			{
				g_pMain->Loop();
			}
		}
		//�A�v���I��
		g_pMain->DestroyD3D();
		delete g_pMain;
	}
	return 0;
}
//
//
//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����MAIN�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return g_pMain->MsgProc(hWnd,uMsg,wParam,lParam);
}
//
//
//�E�B���h�E�쐬
HRESULT MAIN::InitWindow(HINSTANCE hInstance,
		INT iX,INT iY,INT iWidth,INT iHeight,LPCSTR WindowName)
 {
	 // �E�B���h�E�̒�`
	WNDCLASSEX  wc;
	ZeroMemory(&wc,sizeof(wc));
	wc.cbSize = sizeof(wc);									/* �\���̂̑傫�� */
	wc.style = CS_HREDRAW | CS_VREDRAW;//�E�C���h�E�̃T�C�Y���ύX���ꂽ��ĕ`�悷��A�Ƃ����w��/* �X�^�C�� */
	wc.lpfnWndProc = WndProc;/* ���b�Z�[�W�����֐� */
	wc.hInstance = hInstance;/* �v���O�����̃n���h�� */
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);/* �A�C�R�� */
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);/*�J�[�\��*/
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);/*�u���V*/
	wc.lpszClassName = WindowName;/*�N���X��*/
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	RegisterClassEx(&wc);

	//�E�B���h�E�̍쐬
	m_hWnd=CreateWindow(WindowName,WindowName,WS_OVERLAPPEDWINDOW,
		0,0,iWidth,iHeight,0,0,hInstance,0);
	 if(!m_hWnd) 
	 {
		 return E_FAIL;
	 }
	 //�E�C���h�E�̕\��
	 ShowWindow(m_hWnd,SW_SHOW);    
	 UpdateWindow(m_hWnd) ;

	 return S_OK;
 }
//
//
//�E�B���h�E�v���V�[�W���[
 LRESULT MAIN::MsgProc(HWND hWnd,UINT iMsg,WPARAM wParam,LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_KEYDOWN:
		switch((char)wParam)
		{
			case VK_ESCAPE://ESC�L�[�ŏC��
			PostQuitMessage(0);
			break;
		}
		break;
		case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc (hWnd, iMsg, wParam, lParam);
 }
//
//
//���b�Z�[�W���[�v�ƃA�v���P�[�V���������̓����
 void MAIN::Loop()
 {
	 // ���b�Z�[�W���[�v
	 MSG msg={0};
	 ZeroMemory(&msg,sizeof(msg));
	 while(msg.message!=WM_QUIT)
	 {
		 if( PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 else
		 {
			 //�A�v���P�[�V�����̏����͂��������ԁB
			 App();
		 }
	 }
	 //�A�v���P�[�V�����̏I��
 }
 //
 //�A�v���P�[�V���������B�A�v���̃��C���֐��B
 void MAIN::App()
 {
	 Render();
 }

//DirectX������
HRESULT MAIN::InitD3D()
{
	//�Q�l http://marupeke296.com/DX10_No1_Init.html�@�A����
	//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�i�\���̍쐬�j
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.BufferCount = 1;		//�X���b�v�`�F�[���̃o�b�t�@�[����\���l�ł��B�t�����g �o�b�t�@�[���܂݂܂��B
	//�o�b�N �o�b�t�@�[�̕\�����[�h��\�� DXGI_MODE_DESC �\���̂ł��B
	sd.BufferDesc.Width=WINDOW_WIDTH;				//�𑜓x�̕���\���l
	sd.BufferDesc.Height=WINDOW_HEIGHT;				//�𑜓x�̍�����\���l
	sd.BufferDesc.Format=DXGI_FORMAT_R8G8B8A8_UNORM;//�\���t�H�[�}�b�g��\�� DXGI_FORMAT �\���� 4 �����A16 �r�b�g�����Ȃ����K�������t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator=60;			
	sd.BufferDesc.RefreshRate.Denominator=1;
	sd.BufferUsage=DXGI_USAGE_RENDER_TARGET_OUTPUT;//�o�b�N�o�b�t�@�̎g�p�@
	sd.OutputWindow=m_hWnd;	//�o�͐�̃E�B���h�E�n���h��
	sd.SampleDesc.Count=1;   //1xMSAA
	sd.SampleDesc.Quality=0;
	sd.Windowed=TRUE;	
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;	
	//�X���b�v�`�F�[���̍쐬
	if( FAILED( D3D11CreateDeviceAndSwapChain(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,
		0,&pFeatureLevels,1,D3D11_SDK_VERSION,&sd,&m_pSwapChain,&m_pDevice,
		pFeatureLevel,&m_pDeviceContext )) )
	{
		return FALSE;
	}

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	ID3D11Texture2D *pBackBuffer;//�o�b�N�o�b�t�@�̃|�C���^�錾
	m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),(LPVOID*)&pBackBuffer); //�X���b�v�`�F�[������o�b�t�@���󂯎��   
	m_pDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
	SAFE_RELEASE(pBackBuffer);	//�o�b�N�o�b�t�@�̃����[�X�i�Q�ƌ��炵�j
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
	m_pDevice->CreateTexture2D( &descDepth, NULL, &m_pDepthStencil );
	m_pDevice->CreateDepthStencilView( m_pDepthStencil, NULL, &m_pDepthStencilView );
	//�����_�[�^�[�Q�b�g�r���[�Ɛ[�x�X�e���V���r���[���p�C�v���C���Ƀo�C���h�i�֘A�t���j
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView,m_pDepthStencilView);
	//�r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;//�T�C�Y
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;//Z�l�̕�
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;//����
	vp.TopLeftY = 0;
	m_pDeviceContext->RSSetViewports( 1, &vp );
	//���X�^���C�Y�ݒ�
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc,sizeof(rdc));
	rdc.CullMode=D3D11_CULL_NONE;
	rdc.FillMode=D3D11_FILL_SOLID;
	ID3D11RasterizerState* pIr=NULL;
	m_pDevice->CreateRasterizerState(&rdc,&pIr);
	m_pDeviceContext->RSSetState(pIr);
	SAFE_RELEASE(pIr);

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDevice);
}
//
//�V�[������ʂɃ����_�����O
void MAIN::Render()
{
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	float ClearColor[4] = { 0,0,1,1 };// �N���A�F�쐬�@RGBA�̏�
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//��ʃN���A
	//m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	m_pSwapChain->Present(0, 0);//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj	
}