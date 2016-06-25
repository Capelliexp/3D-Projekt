#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

void Forward(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Left(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Backwards(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Right(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Jump(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Down(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void LeftMouse(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void RightMouse(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void Use(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos);
void mouseMovement(POINT mouseInfoNew, XMMATRIX* ViewMatrix);

#endif