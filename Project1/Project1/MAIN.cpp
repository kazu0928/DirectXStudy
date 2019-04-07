#include "MAIN.h"
using namespace DirectX;
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
//
MAIN::MAIN()
{
	ZeroMemory(this, sizeof(MAIN));
}
//
//
//
MAIN::~MAIN()
{
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
	 //�e(m_Shot)�ƓI(m_Model)�̓����蔻��
	 for (int i = 0; i < m_iNumShot; i++)
	 {
		 for (int k = 0; k < m_iNumModel; k++)
		 {
			 XMFLOAT3 in_out_data;

			 DirectX::XMStoreFloat3(&in_out_data, DirectX::XMVector3Length((DirectX::XMLoadFloat3(&m_Shot[i].vPos) - DirectX::XMLoadFloat3(&m_Model[k].vPos))));
			 //xyz�ǂ���������̂�����
			 if (in_out_data.x < 1.0f)
			 {
				 //��������
				 //�������Ă��郂�f���������ꍇ�A���̃��f���̃f�[�^�ɁA�z��Ō�̃��f���̃f�[�^���㏑��
				 MODEL tmp;
				 memcpy(&m_Shot[i], &m_Shot[m_iNumShot - 1], sizeof(MODEL));
				 m_iNumShot--;
				 memcpy(&m_Model[k], &m_Model[m_iNumModel - 1], sizeof(MODEL));
				 m_iNumModel--;
				 break;
			 }
		 }
	 }
	 static int DasisugiBousi = 0;//�e�@�o�������h�~�p�J�E���^�[ 400�t���[���ɂP���ł�悤�ɂ���
	 DasisugiBousi++;
	 if (DasisugiBousi > 400 && m_iNumShot < MAX_SHOT && GetKeyState(VK_SPACE) & 0x80)
	 {
		 DasisugiBousi = 0;
		 m_Shot[m_iNumShot].vPos = XMFLOAT3(0, 0, -2);
		 m_Shot[m_iNumShot].vColor = XMFLOAT4(1, 1, 1, 1);//�e�͔�
		 m_iNumShot++;
	 }
	 //�e�ړ�
	 for (int i = 0; i < m_iNumShot; i++)
	 {
		 m_Shot[i].vPos.z += 0.001f;
		 //�L�[�{�[�h���� ���L�[�Œe�𑀍�
		 if (GetKeyState(VK_LEFT) & 0x80)//���ړ�
		 {
			 m_Shot[i].vPos.x -= 0.001f;
		 }
		 if (GetKeyState(VK_RIGHT) & 0x80)//�E�ړ�
		 {
			 m_Shot[i].vPos.x += 0.001f;
		 }
		 if (GetKeyState(VK_UP) & 0x80)//���ړ�
		 {
			 m_Shot[i].vPos.y += 0.001f;
		 }
		 if (GetKeyState(VK_DOWN) & 0x80)//�E�ړ�
		 {
			 m_Shot[i].vPos.y -= 0.001f;
		 }
	 }
	 //�܂ƈړ�
	 for (int i = 0; i < m_iNumModel; i++)
	 {
		 m_Model[i].vPos.z -= 0.002f;
	 }

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
	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
void MAIN::DestroyD3D()
{
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencil);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}
//
//��
//�V�F�[�_�[���쐬�@���_���C�A�E�g���`
HRESULT MAIN::InitShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́B�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	//D3DCompileFile��D3DX���Ȃ����ߏ���������
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DCompileFromFile(L"Simple.hlsl", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pCompiledShader, &pErrors)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_pDevice->CreateBuffer(&cb, NULL, &m_pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}
//
//
//
HRESULT MAIN::InitPolygon()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[] =
	{
		XMFLOAT3(0.0f, 0.5f, 0.0f),
		XMFLOAT3(0.5f, -0.5f, 0.0f),
		XMFLOAT3(-0.5f, -0.5f, 0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_pDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		return E_FAIL;
	}
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	//�S�Ẵ��f���œ����|���S���B�����o�[�e�b�N�X�o�b�t�@�[���g���B���f�����ƂɈقȂ�̂́A���f���̈ʒu�ƐF�B	
	for (int i = 0; i < MAX_MODEL; i++)
	{
		m_Model[i].vPos = XMFLOAT3(float(rand()) / 5000.0f - 3.0f, float(rand()) / 5000.0f - 3.0f, float(rand()) / 5000.0f + 20.0f);//�����ʒu�̓����_��
		m_Model[i].vColor = XMFLOAT4(float(rand()) / 32767.0f, float(rand()) / 32767.0f, float(rand()) / 32767.0f, 1.0f);//�F�������_��
	}
	m_iNumModel = MAX_MODEL;

	return S_OK;
}
//
//
//�V�[������ʂɃ����_�����O
void MAIN::Render()
{
	//��ʃN���A�i���ۂ͒P�F�ŉ�ʂ�h��Ԃ������j
	FLOAT ClearColor[4] = { 0,0,1,1 };// �N���A�F�쐬�@RGBA�̏�
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);//��ʃN���A
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�N���A

	/*DirectXMath���C�u�����ł͉��Z�ɗp����ꍇ��XMMATRIX���A���ꕨ��XMFLOAT4X4���g�p*/
	XMFLOAT4X4 mWorld;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	//XMStoreFloat4x4(&mWorld, DirectX::XMMatrixRotationY(timeGetTime() / 100.0f));//XMMATRIX����XMFLOAT4X4�֒l���i�[,��]

	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	XMFLOAT3 vEyePt(0.0f, 0.0f, -2.0f); //�J�����i���_�j�ʒu
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	XMStoreFloat4x4(&mView, DirectX::XMMatrixLookAtLH(XMLoadFloat3(&vEyePt),XMLoadFloat3(&vLookatPt),XMLoadFloat3(&vUpVec)));
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	XMStoreFloat4x4(&mProj, DirectX::XMMatrixPerspectiveFovLH(XM_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 100.0f));

	//�g�p����V�F�[�_�[�̓o�^	
	m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);
	//���̃R���X�^���g�o�b�t�@�[���g���V�F�[�_�[�̓o�^
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pDeviceContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�v���~�e�B�u�������_�����O �@�����Ȃ̂ŁA���[���h�g�����X�t�H�[���Ƃ����n�����������[�v���ɂ����i���f�����Ƃɍs���j
	for (int i = 0; i < m_iNumShot; i++)
	{
		//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		XMFLOAT4X4 mTrans, mScale;
		XMStoreFloat4x4(&mScale, DirectX::XMMatrixScaling(0.5, 0.5, 0.5));
		XMStoreFloat4x4(&mTrans,DirectX::XMMatrixTranslation(m_Shot[i].vPos.x, m_Shot[i].vPos.y, m_Shot[i].vPos.z));
		mWorld = mTrans;
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//���[���h�A�J�����A�ˉe�s���n��
			XMFLOAT4X4 m;
			XMStoreFloat4x4(&m,XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProj));
			DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
			cb.mWVP = m;
			//�J���[��n��
			cb.vColor = m_Shot[i].vColor;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->Draw(3, 0);
	}
	//1000�I�����_�����O
	for (int i = 0; i < m_iNumModel; i++)
	{
		//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
		XMFLOAT4X4 mTrans, mScale;
		XMStoreFloat4x4(&mTrans, DirectX::XMMatrixTranslation(m_Model[i].vPos.x, m_Model[i].vPos.y, m_Model[i].vPos.z));
		mWorld = mTrans;
		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
		D3D11_MAPPED_SUBRESOURCE pData;
		SIMPLESHADER_CONSTANT_BUFFER cb;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			//���[���h�A�J�����A�ˉe�s���n��
			XMFLOAT4X4 m;
			XMStoreFloat4x4(&m, XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProj));
			DirectX::XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
			cb.mWVP = m;
			//�J���[��n��
			cb.vColor = m_Model[i].vColor;
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
			m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
		}
		m_pDeviceContext->Draw(3, 0);
	}

	m_pSwapChain->Present(0, 0);//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj	

	//////�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��	
	////D3D11_MAPPED_SUBRESOURCE pData;
	////SIMPLESHADER_CONSTANT_BUFFER cb;
	////if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	////{
	////	//���[���h�A�J�����A�ˉe�s���n��
	////	XMFLOAT4X4 m;
	////	XMStoreFloat4x4(&m, XMLoadFloat4x4(&mWorld) * XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProj));
	////	XMStoreFloat4x4(&m, DirectX::XMMatrixTranspose(XMLoadFloat4x4(&m)));
	////	cb.mWVP = m;
	////	//�J���[��n��
	////	XMFLOAT4 vColor(1, 0, 0, 1);
	////	cb.vColor = vColor;
	////	memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
	////	m_pDeviceContext->Unmap(m_pConstantBuffer, 0);
	////}
	//////���_�C���v�b�g���C�A�E�g���Z�b�g
	////m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
	//////�v���~�e�B�u�E�g�|���W�[���Z�b�g
	////m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//////�v���~�e�B�u�������_�����O
	////m_pDeviceContext->Draw(3, 0);

	////m_pSwapChain->Present(0, 0);//��ʍX�V�i�o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɂj	
}