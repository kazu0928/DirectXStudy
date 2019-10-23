#pragma once
#include "Window.h"

extern Window* g_pWindow;//Windowクラス
//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//ウィンドウ作成
HRESULT Window::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// ウィンドウの定義
	WNDCLASSEX  wc;//ウィンドウクラス
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX); // WNDCLASSEX構造体のサイズ
	wc.style = CS_HREDRAW/*水平方向のサイズ変更があったら全体を描き直す*/ | CS_VREDRAW/*垂直方向のサイズ変更で全体を描き直す*/;
	wc.lpfnWndProc = WndProc;//ウィンドプロシージャ
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	//ウィンドウの作成
	w_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!w_hWnd)
	{
		return E_FAIL;
	}
	//ウインドウの表示
	ShowWindow(w_hWnd, SW_SHOW);
	//ウィンドウアップデート
	UpdateWindow(w_hWnd);

	return S_OK;
}
//ウィンドウプロシージャー
LRESULT Window::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESCキーで終了
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY://ウィンドウが閉じられるときの処理（必須）
		//プログラムの終了
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);//その他の処理
}
//OSから見たウィンドウプロシージャー（実際の処理はWindowクラスのプロシージャーで処理）
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pWindow->MsgProc(hWnd, uMsg, wParam, lParam);
}

void Window::Loop(void (*f)())
{
	// メッセージループ
	MSG msg = { 0 };//MSG構造体の定義
	ZeroMemory(&msg, sizeof(msg));//念のため初期化
	//long time = timeGetTime();//FPS制御
	while (msg.message != WM_QUIT)
	{
		//メッセージの取得
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//仮想キーコードを実際の文字に変換
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			(*f)();
			//FPS制御
			/*
			while (timeGetTime() - time < 1000 / 60)
			{
				Sleep(1);	//!< 1ミリ秒Sleepする
			}
			time = timeGetTime();
			*/
		}
	}
}


