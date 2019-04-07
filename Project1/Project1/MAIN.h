//ヘッダーファイルのインクルード
#define _WIN32_DCOM

#include <windows.h>
#include <d3d11.h>
#include <d3dCompiler.h>
#include "DirectXMath.h"
//音
#include <xaudio2.h>
#include <mmsystem.h>

//必要なライブラリファイルのロード
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dCompiler.lib")

//定数定義
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ
#define MAX_MODEL 1000 //モデル最大数
#define MAX_SHOT 400
#define MAX_WAV 100 //WAVサウンド最大数
#define APP_NAME L"ポリゴン複数表示"
//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x;}

//頂点の構造体
struct SimpleVertex
{
	DirectX::XMFLOAT3 Pos; //位置
};
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体
struct SIMPLESHADER_CONSTANT_BUFFER
{
	DirectX::XMFLOAT4X4 mWVP;
	DirectX::XMFLOAT4 vColor;
};
//物体の構造体
struct MODEL//このサンプルでは単なる三角ポリゴン
{
	DirectX::XMFLOAT3 vPos;
	DirectX::XMFLOAT4 vColor;
};

//
//
//
class MAIN
{
public:
	MAIN();~MAIN();
	HRESULT InitWindow(HINSTANCE,INT,INT,INT,INT,LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();

	void Loop();  //アプリケーションループ（メッセージループ、アプリケーション処理）
	void App();   //アプリケーション処理　親：Loop()
	void Render();//レンダリング
	void DestroyD3D();
	//音
	HRESULT InitXAudio();
	HRESULT LoadSound(LPSTR, DWORD);
	HRESULT PlaySound(DWORD);

	//アプリにひとつ
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Texture2D* m_pDepthStencil;
	IXAudio2* m_pXAudio2;
	IXAudio2MasteringVoice* m_pMasteringVoice;
	//モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout; //頂点レイアウト　座標＋法線、テクスチャ座標
	ID3D11VertexShader* m_pVertexShader;//
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//モデルごと	
	ID3D11Buffer* m_pVertexBuffer;//頂点を保存しておくメモリ領域
	//モデルのインスタンス配列
	MODEL m_Model[MAX_MODEL];
	int m_iNumModel;
	MODEL m_Shot[MAX_SHOT];
	int m_iNumShot;
	//サウンドファイルごと
	IXAudio2SourceVoice* m_pSourceVoice[MAX_WAV];
	BYTE* m_pWavBuffer[MAX_WAV];//波形データ（フォーマット等を含まない、純粋に波形データのみ）
	DWORD m_dwWavSize[MAX_WAV];//波形データのサイズ

};
