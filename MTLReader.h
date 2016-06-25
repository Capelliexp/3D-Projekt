#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

struct MTLFormat {
	float Kd_r = NULL, Kd_g = NULL, Kd_b = NULL, skrap1 = NULL;		//tot bytes: 12 + 4 = 16
	float Ka_r = NULL, Ka_g = NULL, Ka_b = NULL, skrap2 = NULL;		//tot bytes: 12 + 4 = 16
	float Ks_r = NULL, Ks_g = NULL, Ks_b = NULL, skrap3 = NULL;		//tot bytes: 12 + 4 = 16
	float Ns = NULL, skrap4 = NULL, skrap5 = NULL, skrap6 = NULL;	//tot bytes: 4 + 12 = 16
	unsigned short illum = NULL;									//tot bytes: 2
};

void readMTL(MTLFormat albedo[], char* FilePath);
