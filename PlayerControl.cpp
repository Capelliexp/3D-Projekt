#include "PlayerControl.h"

//http://www.gamedev.net/topic/259231-how-to-extract-direction-vector-from-view-matrix/
//https://www.opengl.org/discussion_boards/showthread.php/178484-Extracting-camera-position-from-a-ModelView-Matrix
//http://www.calcul.com/show/calculator/matrix-multiplication_;1;3;3;3

//Mus:
//https://msdn.microsoft.com/en-us/library/windows/desktop/gg153550(v=vs.85).aspx
//https://msdn.microsoft.com/en-us/library/windows/desktop/ms648393(v=vs.85).aspx

float movementSpeed = 2.0f;
float cameraSpeed = 0.002f;

void Forward(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){

	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInvers = XMMatrixInverse(NULL, *ViewMatrix);
	
	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInvers);

	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(View4x4._31, 0, View4x4._33, 0);

	CamLook = XMVector3Normalize(CamLook);

	XMVECTOR newCamPos = (CamPos + (CamLook* (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, {0, 1, 0});
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
}

void Left(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){

	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInverse = XMMatrixInverse(NULL, *ViewMatrix);

	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInverse);

	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(View4x4._31, 0, View4x4._33, 0);
	CamLook = XMVector3Normalize(CamLook);

	XMVECTOR strafe = XMVectorSet(-View4x4._33, 0, View4x4._31, 0);

	XMVECTOR newCamPos = (CamPos + (strafe * (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
}

void Backwards(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){

	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInvers = XMMatrixInverse(NULL, *ViewMatrix);

	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInvers);

	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(View4x4._31, 0, View4x4._33, 0);

	CamLook = XMVector3Normalize(CamLook);

	XMVECTOR newCamPos = (CamPos - (CamLook* (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);

}

void Right(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){

	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInverse = XMMatrixInverse(NULL, *ViewMatrix);

	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInverse);

	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(View4x4._31, 0, View4x4._33, 0);
	CamLook = XMVector3Normalize(CamLook);

	XMVECTOR strafe = XMVectorSet(-View4x4._33, 0, View4x4._31, 0);

	XMVECTOR newCamPos = (CamPos + (-strafe * (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
}

void Jump(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos) {
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInvers = XMMatrixInverse(NULL, *ViewMatrix);
	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInvers);
	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(0, View4x4._32, 0, 0);
	CamLook = XMVector3Normalize(CamLook);
	XMVECTOR newCamPos = (CamPos + (XMVectorSet(0, 1, 0, 0) * (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
}

void Down(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos) {
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInvers = XMMatrixInverse(NULL, *ViewMatrix);
	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInvers);
	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	XMVECTOR CamLook = XMVectorSet(0, View4x4._32, 0, 0);
	CamLook = XMVector3Normalize(CamLook);
	XMVECTOR newCamPos = (CamPos - (XMVectorSet(0, 1, 0, 0) * (movementSpeed / FPS)));
	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32, View4x4._33, 0)) + newCamPos;

	*CurrentCamPos = newCamPos;

	*ViewMatrix = XMMatrixLookAtLH({ newCamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
}

void LeftMouse(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){
}

void RightMouse(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){
}

void Use(__int64 FPS, XMMATRIX* ViewMatrix, XMVECTOR* CurrentCamPos){
}

void mouseMovement(POINT mouseInfoNew, XMMATRIX* ViewMatrix){

	LONG x = mouseInfoNew.x;
	LONG y = mouseInfoNew.y;

	float xMovement = (float)960 - x;
	float yMovement = (float)540 - y;

	//---------------------------------------

	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);
	XMMATRIX ViewInverse = XMMatrixInverse(NULL, *ViewMatrix);

	XMFLOAT4X4 View4x4;
	XMStoreFloat4x4(&View4x4, ViewInverse);

	XMVECTOR CamPos = XMVectorSet(View4x4._41, View4x4._42, View4x4._43, 0);
	
	XMMATRIX RotationX = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), cameraSpeed*xMovement);

	XMVECTOR newCamLook = (XMVectorSet(View4x4._31, View4x4._32 + cameraSpeed*yMovement, View4x4._33, 0)) + CamPos;

	*ViewMatrix = XMMatrixLookAtLH({ CamPos }, { newCamLook }, { 0, 1, 0 });
	*ViewMatrix = *ViewMatrix * RotationX;
	*ViewMatrix = XMMatrixTranspose(*ViewMatrix);

}