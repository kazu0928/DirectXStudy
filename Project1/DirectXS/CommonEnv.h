#pragma once

#include <windows.h>
#include <d3d11.h>

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x;}