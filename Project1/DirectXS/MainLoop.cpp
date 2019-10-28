#include "MainLoop.h"

Window* g_pWindow = NULL;
D3DX* g_pD3DX = NULL;
Main* g_pMain = NULL;

using namespace DirectX;

//エントリーポイント
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
			//頂点を定義
			SIMPLE_VERTEX vertices[] =
			{
				XMFLOAT3(-1.0f,1.0f,-1.0f),
				XMFLOAT3(1.0f,1.0f,-1.0f),
				XMFLOAT3(1.0f,-1.0f,-1.0f),
				XMFLOAT3(-1.0f,-1.0f,-1.0f),
				XMFLOAT3(-1.0f,1.0f,1.0f),
				XMFLOAT3(1.0f,1.0f,1.0f),
				XMFLOAT3(1.0f,-1.0f,1.0f),
				XMFLOAT3(-1.0f,-1.0f,1.0f),
			};
			if (SUCCEEDED(g_pD3DX->CreateVertexBuffer(vertices,(UINT)(sizeof(vertices)/sizeof(vertices[0])))))
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

//アプリケーション処理　アプリのメイン関数
void Main::App ()
{
	g_pMain->Render ();
}
void Main::Render ()
{
	//画面を単色で塗りつぶす
	float ClearColor[4] = { 0,0,0.3f,1 };
	g_pD3DX->RenderClearColor(ClearColor);
	/*描画パイプラインの構成、定数バッファへの書き込み*/
	//ビュートランスフォーム
	XMFLOAT4X4 mWorld, mView, mProj;
	//ワールドトランスフォーム(ワールド座標)
	XMStoreFloat4x4(&mWorld, XMMatrixTranslation(0, 0, 0));
	//ビュートランスフォーム
	XMFLOAT3 vEyePt(0.0f, 1.0f, -2.0f);//カメラの位置
	XMFLOAT3 vLookatPt(0.0f, 0.0f, 0.0f);//注視する方向
	XMFLOAT3 vUpVec(0.0f, 1.0f, 0.0f);//上位置
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(XMLoadFloat3(&vEyePt), XMLoadFloat3(&vLookatPt), XMLoadFloat3(&vUpVec)));
	//プロジェクショントランスフォーム
	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(XM_PI/4/*FOV(ラジアン)*/, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT/*アス比*/, 0.1f/*近クリップ（表示領域までの距離）*/, 110.0f/*遠クリップ（表示領域）*/));
	//シェーダの登録
	g_pD3DX->d_pDeviceContext->VSSetShader(g_pD3DX->d_pVertexShader,NULL,0);
	g_pD3DX->d_pDeviceContext->PSSetShader(g_pD3DX->d_pPixelShader, NULL, 0);
	//ラスタライザとかもここで設定してもよし

	D3D11_MAPPED_SUBRESOURCE pData;
	CB_SIMPLE cb;
	//データのポインタを取得してリソースのGPUアクセスの拒否
	if (SUCCEEDED(g_pD3DX->d_pDeviceContext->Map((g_pD3DX->d_pConstantBuffer), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//MVP行列を渡す
		XMFLOAT4X4 m;
		XMStoreFloat4x4(&m, XMLoadFloat4x4(&mWorld) *XMLoadFloat4x4(&mView)*XMLoadFloat4x4(&mProj));
		XMStoreFloat4x4(&m, XMMatrixTranspose(XMLoadFloat4x4(&m)));
		cb.mWVP = m;
		//書き込み
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		//GPUの再アクセス可能に
		g_pD3DX->d_pDeviceContext->Unmap(g_pD3DX->d_pConstantBuffer, 0);
	}
	//このコンスタントバッファーを、どのシェーダーで使うかを指定
	g_pD3DX -> d_pDeviceContext->VSSetConstantBuffers(0, 1, &(g_pD3DX -> d_pConstantBuffer));
	g_pD3DX->d_pDeviceContext->PSSetConstantBuffers(0, 1, &(g_pD3DX->d_pConstantBuffer));

	//バーテックスバッファーをセット
	UINT stride = sizeof(SIMPLE_VERTEX);
	UINT offset = 0;
	g_pD3DX->d_pDeviceContext->IASetVertexBuffers(0, 1, &g_pD3DX->d_pVertexBuffer, &stride, &offset);
	//頂点インプットレイアウトをセット
	g_pD3DX->d_pDeviceContext->IASetInputLayout(g_pD3DX->d_pVertexLayout);
	//プリミティブ・トポロジーをセット
	g_pD3DX->d_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//プリミティブをレンダリング
	g_pD3DX->d_pDeviceContext->Draw(8, 0);


	//画面更新
	g_pD3DX->UpdateDisplay();

}



