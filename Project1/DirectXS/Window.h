#pragma once
#include "CommonEnv.h"

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define APP_NAME L"DirectX"


class Window
{
public:
	//�E�B���h�E�̏�����
	HRESULT InitWindow(HINSTANCE, INT, INT, INT, INT, LPCWSTR);
	LRESULT MsgProc(HWND, UINT, WPARAM, LPARAM);
	HWND w_hWnd = NULL;//�E�B���h�E�̃n���h��
	void Loop(void(*)());
};

