#pragma once

#include <windows.h>
#include <d3d11.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib, "d3d11.lib")

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x;}