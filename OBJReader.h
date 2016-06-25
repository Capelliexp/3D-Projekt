#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <iostream>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

struct OBJFormat {
	float x, y, z;
	float u, v;
	float xn, yn, zn;
};

struct vertex {
	float x, y, z;
};

struct uv {
	float u, v;
};

struct normal {
	float x, y, z;
};

void readOBJ(OBJFormat faces[], char* FilePath, float scale, float MoveX, float MoveY, float MoveZ);
