#pragma once
#include "Window.h"

extern Window* g_pWindow;//Window�N���X
//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//�E�B���h�E�쐬
HRESULT Window::InitWindow(HINSTANCE hInstance, INT iX, INT iY, INT iWidth, INT iHeight, LPCWSTR WindowName)
{
	// �E�B���h�E�̒�`
	WNDCLASSEX  wc;//�E�B���h�E�N���X
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX); // WNDCLASSEX�\���̂̃T�C�Y
	wc.style = CS_HREDRAW/*���������̃T�C�Y�ύX����������S�̂�`������*/ | CS_VREDRAW/*���������̃T�C�Y�ύX�őS�̂�`������*/;
	wc.lpfnWndProc = WndProc;//�E�B���h�v���V�[�W��
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = WindowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);
	//�E�B���h�E�̍쐬
	w_hWnd = CreateWindow(WindowName, WindowName, WS_OVERLAPPEDWINDOW,
		0, 0, iWidth, iHeight, 0, 0, hInstance, 0);
	if (!w_hWnd)
	{
		return E_FAIL;
	}
	//�E�C���h�E�̕\��
	ShowWindow(w_hWnd, SW_SHOW);
	//�E�B���h�E�A�b�v�f�[�g
	UpdateWindow(w_hWnd);

	return S_OK;
}
//�E�B���h�E�v���V�[�W���[
LRESULT Window::MsgProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE://ESC�L�[�ŏI��
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY://�E�B���h�E��������Ƃ��̏����i�K�{�j
		//�v���O�����̏I��
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);//���̑��̏���
}
//OS���猩���E�B���h�E�v���V�[�W���[�i���ۂ̏�����Window�N���X�̃v���V�[�W���[�ŏ����j
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pWindow->MsgProc(hWnd, uMsg, wParam, lParam);
}

void Window::Loop(void (*f)())
{
	// ���b�Z�[�W���[�v
	MSG msg = { 0 };//MSG�\���̂̒�`
	ZeroMemory(&msg, sizeof(msg));//�O�̂��ߏ�����
	//long time = timeGetTime();//FPS����
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W�̎擾
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//���z�L�[�R�[�h�����ۂ̕����ɕϊ�
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			(*f)();
			//FPS����
			/*
			while (timeGetTime() - time < 1000 / 60)
			{
				Sleep(1);	//!< 1�~���bSleep����
			}
			time = timeGetTime();
			*/
		}
	}
}


