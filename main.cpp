#include <windows.h>

#include <iostream>
#include <crtdbg.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <string>

#include "bth_image.h"
#include "PlayerControl.h"
#include "OBJReader.h"
#include "MTLReader.h"

#include "DirectXTK/DirectXHelpers.h"
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/Model.h"
#include "DirectXTK/SimpleMath.h"
#include "DirectXTK/SimpleMath.inl"
#include "DirectXTK/WICTextureLoader.h"

#include <Objbase.h>

#define _T(a)  L ## a

using namespace DirectX;

//https://msdn.microsoft.com/en-us/library/windows/desktop/ee415571(v=vs.85).aspx	DirectXMath Tutorial
//http://mws.bykim.se/4-VertexBuffers.php	ByKim
//http://www.rastertek.com/dx11tut04.html	enkla begrepp
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476896%28v=vs.85%29.aspx	hur man skapar en "constant buffer"
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476491(v=vs.85).aspx		VSSetConstantBuffers()
//https://msdn.microsoft.com/en-us/library/windows/desktop/dn508285%28v=vs.85%29.aspx	"dynamic resources" / map - unmap
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476486%28v=vs.85%29.aspx	UpdateSubresource()
//HRESULT hr = ...
//Ctrl + M + O
//https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074%28v=vs.85%29.aspx	depth stencil
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476506%28v=vs.85%29.aspx	skapa depth stencil
//http://www.gamedev.net/topic/624213-directx11-depth-buffer-problem-solved/			depth stencil forum
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476507(v=vs.85).aspx		CreateDepthStencilView (Måste användas)
//https://msdn.microsoft.com/en-us/library/windows/desktop/bb205120(v=vs.85).aspx		depth test explanation

//http://www.gamedev.net/topic/639136-d3d11-simple-textured-triangle-wont-draw/			textur forum
//http://www.rastertek.com/dx11tut05.html												textur rastertek
//https://msdn.microsoft.com/en-us/library/windows/desktop/bb509694(v=vs.85).aspx		load()
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476220(v=vs.85).aspx		Subresource
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476253(v=vs.85).aspx		2D-texture struct
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476521(v=vs.85).aspx		CreateTexture2D()
//C:\Users\Capelli\Dropbox\Skola\Programmering\3D Föreläsningar\Powerpoints\7			texturing.pdf

//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476904(v=vs.85).aspx	Hur man läser in textur
//https://msdn.microsoft.com/en-us/library/windows/desktop/ff476286(v=vs.85).aspx	CreateTextureFromFile()

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HRESULT CreateDirect3DContext(HWND wndHandle);

IDXGISwapChain* gSwapChain = nullptr;

ID3D11Device* gDevice = nullptr;
ID3D11DeviceContext* gDeviceContext = nullptr;

//-----------------------	OBJ/MTL

ID3D11Buffer* gVertexBuffer_Floor = nullptr;
ID3D11Buffer* gVertexBuffer_Light = nullptr;
ID3D11Buffer* gVertexBuffer_Box = nullptr;
ID3D11Buffer* gVertexBuffer_Boll = nullptr;
ID3D11Buffer* gVertexBuffer_Drake = nullptr;

ID3D11Buffer* MTLBuffer_Box = nullptr;
ID3D11Buffer* gConstantBuffer_Boll = nullptr;
ID3D11Buffer* MTLBuffer_Drake = nullptr;

//-----------------------	Object textures

ID3D11ShaderResourceView* gTextureView_BTH;					//texturerna
ID3D11ShaderResourceView* gTextureView_Box;

ID3D11ShaderResourceView* gFirstPass_Normal;
ID3D11ShaderResourceView* gFirstPass_DiffuseAlbedo;
ID3D11ShaderResourceView* gFirstPass_SpecularAlbedo;
ID3D11ShaderResourceView* gFirstPass_Position;

ID3D11ShaderResourceView* gShadowView;	//för shadow mapping

//-----------------------	Buffers

ID3D11Buffer* MatriserBuffer = nullptr;
ID3D11Buffer* CamPosBuffer = nullptr;
ID3D11Buffer* LightBuffer_1 = nullptr;

//-----------------------	Shaders

ID3D11InputLayout* gVertexLayoutFinal = nullptr;
ID3D11InputLayout* gVertexLayoutFirstPass = nullptr;
ID3D11InputLayout* gVertexLayoutLightShadingPass = nullptr;
ID3D11VertexShader* gVertexShaderFinal = nullptr;
ID3D11VertexShader* gVertexShaderFirstPass = nullptr;
ID3D11VertexShader* gVertexShaderLightShadingPass = nullptr;

ID3D11GeometryShader* gGeometryShaderFirstPass = nullptr;
ID3D11GeometryShader* gGeometryShaderBasic = nullptr;
ID3D11GeometryShader* gGeometryShaderBackface = nullptr;

ID3D11PixelShader* gPixelShaderFinal = nullptr;
ID3D11PixelShader* gPixelShaderFirstPass = nullptr;
ID3D11PixelShader* gPixelShaderLightShadingPass = nullptr;

//-----------------------	RTV, DSV och SRV

ID3D11RenderTargetView* gBackbufferRTV = nullptr;	//screen output
ID3D11DepthStencilView* gDepthStencilView;			//depth test

ID3D11RenderTargetView* gFirstPassRTV[4];	//shader output
ID3D11ShaderResourceView* gFirstPassSRV[4];	//shader input
ID3D11Texture2D* FirstPassTex[4];			//texture for RTV and SRV

ID3D11RenderTargetView* gLightShadingPassRTV;	//skapa dessa...
ID3D11ShaderResourceView* gLightShadingPassSRV;
ID3D11Texture2D* LightShadingTex;

//-----------------------

ID3D11RasterizerState* RastState;	//till rasterizer

LARGE_INTEGER TotalFrames = { 0 };
__int64 FPS = { 0 };

POINT mouseInfoNew;	//musen rörelse under framen

XMVECTOR CurrentCamPos;	//kamerans nuvarande position

HRESULT hr;	//errorblob

//-----------------------	ESC

bool MouseMoveESC = false; 
bool LastFrameESC = false;

//-----------------------

float ViewPortWidth;
float ViewPortHeight;

//-----------------------	Funktioner

void Update();
void Keyboard();
void Mouse();
void Clock();
void checkErrorBlob(HRESULT hr, std::string pos);

void Render_pre();
void RenderFirstPass(ID3D11Buffer* OBJ, ID3D11Buffer* MTL, int draws, ID3D11ShaderResourceView* texure);
void RenderLightShadingPass();
void RenderFINAL();

//-----------------------	Light-structen

struct LightStruct{		//multiple of 16!
	XMMATRIX ViewMatrixLight;	//64-byte
	XMVECTOR LightRange;		//16-byte
	XMVECTOR PosLight;			//16-byte
	XMVECTOR ViewLight;			//16-byte
	XMVECTOR SpotlightAngles;	//16-byte
	XMVECTOR LightColor;		//16-byte
	unsigned int LightType;		//4-byte	//1 = pointlight, 2 = directional light, 3 = spotlight
};

LightStruct LightObject1;

//-----------------------	WVP Matrisen

struct WVPI_Matriser {
	XMMATRIX WorldMatrix;		//global position
	XMMATRIX ViewMatrix;		//camera
	XMMATRIX ProjectionMatrix;	//FOV, aspect, near/far plane
};

WVPI_Matriser MatrixObject;

//-------

void CreateMatrixObjects(){
	MatrixObject.WorldMatrix = XMMatrixIdentity();
	MatrixObject.WorldMatrix = XMMatrixTranspose(MatrixObject.WorldMatrix);

	MatrixObject.ViewMatrix = XMMatrixLookAtLH( { 2.5,1,1 }, { 2.5,0,2.5 }, { 0,1,0 } );	//pos, look, up
	MatrixObject.ViewMatrix = XMMatrixTranspose(MatrixObject.ViewMatrix);

	MatrixObject.ProjectionMatrix = XMMatrixPerspectiveFovLH(1.4f, 1.777f, 0.1f, 100.0f);	//FovAngleY, AspectRatio, NearZ, FarZ
	MatrixObject.ProjectionMatrix = XMMatrixTranspose(MatrixObject.ProjectionMatrix);

	//----------------------------------------------------------

	D3D11_BUFFER_DESC MatrixBufferDesc;
	memset(&MatrixBufferDesc, 0, sizeof(MatrixBufferDesc));
	MatrixBufferDesc.ByteWidth = sizeof(MatrixObject);
	MatrixBufferDesc.BindFlags =		D3D11_BIND_CONSTANT_BUFFER;
	MatrixBufferDesc.Usage =			D3D11_USAGE_DYNAMIC;
	MatrixBufferDesc.CPUAccessFlags =	D3D11_CPU_ACCESS_WRITE;
	MatrixBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA WVPI;
	WVPI.pSysMem = &MatrixObject;
	WVPI.SysMemPitch = 0;
	WVPI.SysMemSlicePitch = 0;

	gDevice->CreateBuffer(&MatrixBufferDesc, &WVPI, &MatriserBuffer);
}

void CreateLightObjects() {

	D3D11_BUFFER_DESC LightBufferDesc;
	memset(&LightBufferDesc, 0, sizeof(LightBufferDesc));
	LightBufferDesc.ByteWidth = sizeof(LightObject1);
	LightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	LightBufferDesc.MiscFlags = 0;

	//---

	LightObject1.ViewMatrixLight = XMMatrixLookAtLH({ 2.5,3.0,2.5 }, { 2.5,0,2.5 }, { 0,1,0 });		//pos, look, up
	LightObject1.LightRange = {8.0, 0.0, 0.0, 0.0};			//1
	LightObject1.PosLight = { 2.5, 3.0, 2.5 , 0.0};				//3	//behövs ej, finns i matrisen
	LightObject1.ViewLight = { 2.5, 0.0, 2.5, 0.0 };			//2	//behövs ej, finns i matrisen
	LightObject1.SpotlightAngles = { 0.2f, 0.8f, 0.0f, 0.0f };	//2
	LightObject1.LightColor = { 1.0, 1.0 , 1.0, 0.0 };			//3
	LightObject1.LightType = 1;									//1	//1 = pointlight, 2 = directional light, 3 = spotlight

	D3D11_SUBRESOURCE_DATA Light1;
	Light1.pSysMem = &LightObject1;
	Light1.SysMemPitch = 0;
	Light1.SysMemSlicePitch = 0;

	gDevice->CreateBuffer(&LightBufferDesc, &Light1, &LightBuffer_1);

	//---
}

void CreateOtherBuffers(){
	D3D11_BUFFER_DESC CamPosDesc;
	memset(&CamPosDesc, 0, sizeof(CamPosDesc));
	CamPosDesc.ByteWidth = sizeof(CurrentCamPos);
	CamPosDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CamPosDesc.Usage = D3D11_USAGE_DYNAMIC;
	CamPosDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	CamPosDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA CamPos_data;
	CamPos_data.pSysMem = &CurrentCamPos;
	CamPos_data.SysMemPitch = 0;
	CamPos_data.SysMemSlicePitch = 0;

	gDevice->CreateBuffer(&CamPosDesc, &CamPos_data, &CamPosBuffer);
}

void CreateRastarizer() {
	D3D11_RASTERIZER_DESC Rast;		//maybe use D3D11_RASTERIZER_DESC1 ???
	Rast.FillMode = D3D11_FILL_SOLID;
	Rast.CullMode = D3D11_CULL_NONE;	//NONE / BACK / FRONT
	Rast.FrontCounterClockwise = FALSE;
	Rast.DepthBias = 0;
	Rast.DepthBiasClamp = 0.0f;
	Rast.SlopeScaledDepthBias = 0.0f;
	Rast.DepthClipEnable = TRUE;
	Rast.ScissorEnable = FALSE;
	Rast.MultisampleEnable = FALSE;
	Rast.AntialiasedLineEnable = FALSE;	//om MultisampleEnable är TRUE måste denna vara FALSE, olika algoritmer for AA.

	gDevice->CreateRasterizerState(&Rast, &RastState);	//denna fungerar ej (gör den inte ???)

	gDeviceContext->RSSetState(RastState);
}

void CreateDepth() {
	// resource texture
	ID3D11Texture2D* depthTex1 = nullptr;

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = (INT)ViewPortWidth;
	descDepth.Height = (INT)ViewPortHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//DXGI_FORMAT_D24_UNORM_S8_UINT / DXGI_FORMAT_R32_TYPELESS / DXGI_FORMAT_R24_UNORM_X8_TYPELESS / DXGI_FORMAT_R24G8_TYPELESS
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL/* | D3D11_BIND_SHADER_RESOURCE*/;	//D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;                         // the window to be used

	//D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	//descDSV.Format = DXGI_FORMAT_R32_TYPELESS;	//DXGI_FORMAT_D32_FLOAT / DXGI_FORMAT_D24_UNORM_S8_UINT
	//descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//descDSV.Texture2D.MipSlice = 0;

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;	//DXGI_FORMAT_R32_FLOAT / DXGI_FORMAT_R24_UNORM_X8_TYPELESS
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = descDepth.MipLevels;
	//srvDesc.Texture2D.MostDetailedMip = 0;

	hr = gDevice->CreateTexture2D(&descDepth, NULL, &depthTex1);
	hr = gDevice->CreateDepthStencilView(depthTex1, /*&descDSV*/ NULL, &gDepthStencilView);
	//hr = gDevice->CreateShaderResourceView(depthTex1, &srvDesc, &gShadowView);		//textur så vi kan se resultatet, shadow mapping
	
	// release the texture, because having a reference to the View of it is enough!
	depthTex1->Release();
}

void CreateShaders(){

	//CREATE VERTEX SHADER FOR FIRST PASS
	ID3DBlob* pVS3 = nullptr;
	D3DCompileFromFile(L"VertexFirstPass.hlsl", nullptr, nullptr, "VS_main", "vs_4_0", 0, 0, &pVS3, nullptr);

	gDevice->CreateVertexShader(pVS3->GetBufferPointer(), pVS3->GetBufferSize(), nullptr, &gVertexShaderFirstPass);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc3[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	gDevice->CreateInputLayout(inputDesc3, ARRAYSIZE(inputDesc3), pVS3->GetBufferPointer(), pVS3->GetBufferSize(), &gVertexLayoutFirstPass);
	// we do not need this COM object anymore, so we release it.
	pVS3->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE VERTEX SHADER FOR LIGHT SHADING PASS
	ID3DBlob* pVS2 = nullptr;
	D3DCompileFromFile(L"VertexLightShadingPass.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", 0, 0, &pVS2, nullptr);	//OBS vs_5_0

	gDevice->CreateVertexShader(pVS2->GetBufferPointer(), pVS2->GetBufferSize(), nullptr, &gVertexShaderLightShadingPass);

	//create input layout (verified using vertex shader)
	D3D11_INPUT_ELEMENT_DESC inputDesc2[] = {				//Måste vi använda denna om vi skapar en fullscreen quad?
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	gDevice->CreateInputLayout(inputDesc2, ARRAYSIZE(inputDesc2), pVS2->GetBufferPointer(), pVS2->GetBufferSize(), &gVertexLayoutLightShadingPass);
	// we do not need this COM object anymore, so we release it.
	pVS2->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE VERTEX SHADER FOR FINAL PASS
	ID3DBlob* pVS1 = nullptr;
	D3DCompileFromFile( L"Vertex.hlsl", nullptr, nullptr, "VS_main", "vs_5_0", 0, 0, &pVS1, nullptr);	//OBS vs_5_0

	gDevice->CreateVertexShader(pVS1->GetBufferPointer(), pVS1->GetBufferSize(), nullptr, &gVertexShaderFinal);
	
	D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pVS1->GetBufferPointer(), pVS1->GetBufferSize(), &gVertexLayoutFinal);
	
	pVS1->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE GEOMETRY SHADER FOR FIRST PASS
	ID3DBlob* pGS0 = nullptr;
	ID3DBlob* errorBlob0 = nullptr;
	hr = D3DCompileFromFile(L"GeometryFirstPass.hlsl", nullptr, nullptr, "GS_main", "gs_4_0", 0, 0, &pGS0, &errorBlob0);

	if (FAILED(hr)) {
		if (errorBlob0) {
			OutputDebugStringA((char*)errorBlob0->GetBufferPointer());
			errorBlob0->Release();
		}
	}

	gDevice->CreateGeometryShader(pGS0->GetBufferPointer(), pGS0->GetBufferSize(), nullptr, &gGeometryShaderFirstPass);

	pGS0->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE PIXEL SHADER FOR FIRST PASS
	ID3DBlob* pPS3 = nullptr;
	D3DCompileFromFile(L"FragmentFirstPass.hlsl", nullptr, nullptr, "PS_main", "ps_4_0", 0, 0, &pPS3, nullptr);

	gDevice->CreatePixelShader(pPS3->GetBufferPointer(), pPS3->GetBufferSize(), nullptr, &gPixelShaderFirstPass);

	pPS3->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE PIXEL SHADER FOR LIGHT SHADING PASS
	ID3DBlob* pPS2 = nullptr;
	D3DCompileFromFile(L"FragmentLightShadingPass.hlsl", nullptr, nullptr, "PS_main", "ps_4_0", 0, 0, &pPS2, nullptr);

	gDevice->CreatePixelShader(pPS2->GetBufferPointer(), pPS2->GetBufferSize(), nullptr, &gPixelShaderLightShadingPass);

	pPS2->Release();

	//-------------------------------------------------------------------------------------------------------------------------------

	//CREATE PIXEL SHADER FOR FINAL PASS
	ID3DBlob* pPS1 = nullptr;
	D3DCompileFromFile(L"Fragment.hlsl", nullptr, nullptr, "PS_main", "ps_4_0", 0, 0, &pPS1, nullptr);

	gDevice->CreatePixelShader(pPS1->GetBufferPointer(), pPS1->GetBufferSize(), nullptr, &gPixelShaderFinal);
	
	pPS1->Release();
}

void CreateTexturesAndViews(){

	CoInitialize(NULL); //only call this once in the program

	//-------------	BTH IMAGE

	D3D11_TEXTURE2D_DESC bthTexDesc;
	ZeroMemory(&bthTexDesc, sizeof(bthTexDesc));
	bthTexDesc.Width = BTH_IMAGE_WIDTH;
	bthTexDesc.Height = BTH_IMAGE_HEIGHT;
	bthTexDesc.MipLevels = bthTexDesc.ArraySize = 1;
	bthTexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bthTexDesc.SampleDesc.Count = 1;
	bthTexDesc.SampleDesc.Quality = 0;
	bthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	bthTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bthTexDesc.MiscFlags = 0;
	bthTexDesc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture_BTH = NULL;

	D3D11_SUBRESOURCE_DATA data_bth;
	ZeroMemory(&data_bth, sizeof(data_bth));
	data_bth.pSysMem = (void*)BTH_IMAGE_DATA;
	data_bth.SysMemPitch = BTH_IMAGE_WIDTH * 4 * sizeof(char);
	hr = gDevice->CreateTexture2D(&bthTexDesc, &data_bth, &pTexture_BTH);

	D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
	ZeroMemory(&resViewDesc, sizeof(resViewDesc));
	resViewDesc.Format = bthTexDesc.Format;
	resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resViewDesc.Texture2D.MipLevels = bthTexDesc.MipLevels;
	resViewDesc.Texture2D.MostDetailedMip = 0;

	hr = gDevice->CreateShaderResourceView(pTexture_BTH, &resViewDesc, &gTextureView_BTH);

	pTexture_BTH->Release();

	//------------- Box texture

	CreateWICTextureFromFile(gDevice, gDeviceContext, L"objs & texturs/Box & Boll/brick_16.jpg", NULL, &gTextureView_Box, 0);

	//-------------	Create Textures

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = 1280;
	textureDesc.Height = 720;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	gDevice->CreateTexture2D(&textureDesc, NULL, &FirstPassTex[0]);
	gDevice->CreateTexture2D(&textureDesc, NULL, &FirstPassTex[1]);
	gDevice->CreateTexture2D(&textureDesc, NULL, &FirstPassTex[2]);
	gDevice->CreateTexture2D(&textureDesc, NULL, &FirstPassTex[3]);

	gDevice->CreateTexture2D(&textureDesc, NULL, &LightShadingTex);

	//-------------	Create RTVs

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	gDevice->CreateRenderTargetView(FirstPassTex[0], &renderTargetViewDesc, &gFirstPassRTV[0]);
	gDevice->CreateRenderTargetView(FirstPassTex[1], &renderTargetViewDesc, &gFirstPassRTV[1]);
	gDevice->CreateRenderTargetView(FirstPassTex[2], &renderTargetViewDesc, &gFirstPassRTV[2]);
	gDevice->CreateRenderTargetView(FirstPassTex[3], &renderTargetViewDesc, &gFirstPassRTV[3]);

	gDevice->CreateRenderTargetView(LightShadingTex, &renderTargetViewDesc, &gLightShadingPassRTV);

	//-------------	Create SRVs

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewFromFirstPass;
	ZeroMemory(&shaderResourceViewFromFirstPass, sizeof(shaderResourceViewFromFirstPass));
	shaderResourceViewFromFirstPass.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewFromFirstPass.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewFromFirstPass.Texture2D.MostDetailedMip = 0;
	shaderResourceViewFromFirstPass.Texture2D.MipLevels = 1;

	gDevice->CreateShaderResourceView(FirstPassTex[0], &shaderResourceViewFromFirstPass, &gFirstPassSRV[0]);
	gDevice->CreateShaderResourceView(FirstPassTex[1], &shaderResourceViewFromFirstPass, &gFirstPassSRV[1]);
	gDevice->CreateShaderResourceView(FirstPassTex[2], &shaderResourceViewFromFirstPass, &gFirstPassSRV[2]);
	gDevice->CreateShaderResourceView(FirstPassTex[3], &shaderResourceViewFromFirstPass, &gFirstPassSRV[3]);

	gDevice->CreateShaderResourceView(LightShadingTex, &shaderResourceViewFromFirstPass, &gLightShadingPassSRV);
}

void CreateTriangleData(){
	OBJFormat Floor[6] = {
		-5.0f, 0.0f, 5.0f,		//v0 pos	-	top/vänster	-	1
		0.0f, 0.0f,				//v0 tex
		0.0f, 1.0f, 0.0f,		//v0 norm

		5.0f, 0.0f, -5.0f,		//v1		-	bot/höger	-	2
		1.0f, 1.0f,				//v1 tex
		0.0f, 1.0f, 0.0f,		//v1 norm

		-5.0f, 0.0f, -5.0f,	 	//v2		-	bot/vänster	-	3
		0.0f, 1.0f,				//v2 tex
		0.0f, 1.0f, 0.0f,		//v2 norm

		-5.0f, 0.0f, 5.0f,		//v3 pos	-	top/vänster	-	1
		0.0f, 0.0f,				//v3 tex
		0.0f, 1.0f, 0.0f,		//v3 norm

		5.0f, 0.0f, 5.0f,		//v4		-	top/höger	-	4
		1.0f, 0.0f,				//v4 tex
		0.0f, 1.0f, 0.0f,		//v4 norm

		5.0f, 0.0f, -5.0f,		//v5		-	bot/höger	-	2
		1.0f, 1.0f,				//v5 tex
		0.0f, 1.0f, 0.0f,		//v5 norm
	};

	OBJFormat light[6] = {
		2.4f, 3.1f, 2.7f,		//v0 pos	-	top/vänster	-	1
		0.0f, 0.0f,				//v0 tex
		0.0f, 0.0f, -1.0f,

		2.6f, 2.9f, 2.7f,		//v1		-	bot/höger	-	2
		1.0f, 1.0f,				//v1 tex
		0.0f, 0.0f, -1.0f,

		2.4f, 2.9f, 2.7f,	 	//v2		-	bot/vänster	-	3
		0.0f, 1.0f,				//v2 tex
		0.0f, 0.0f, -1.0f,

		2.4f, 3.1f, 2.7f,		//v3 pos	-	top/vänster	-	1
		0.0f, 0.0f,				//v3 tex
		0.0f, 0.0f, -1.0f,

		2.6f, 3.1f, 2.7f,		//v4		-	top/höger	-	4
		1.0f, 0.0f,				//v4 tex
		0.0f, 0.0f, -1.0f,

		2.6f, 2.9f, 2.7f,		//v5		-	bot/höger	-	2
		1.0f, 1.0f,				//v5 tex
		0.0f, 0.0f, -1.0f,
	};

	//-----------------------------------------------

	D3D11_BUFFER_DESC TriangleBufferDescNormal_MTL;
	memset(&TriangleBufferDescNormal_MTL, 0, sizeof(TriangleBufferDescNormal_MTL));
	TriangleBufferDescNormal_MTL.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	TriangleBufferDescNormal_MTL.Usage = D3D11_USAGE_DEFAULT;
	TriangleBufferDescNormal_MTL.ByteWidth = sizeof(MTLFormat) * 1;

	//-----------------------------------------------	Box

	OBJFormat* Box = new OBJFormat[36];
	MTLFormat* BoxAlbedo = new MTLFormat[1];

	readOBJ(Box, "OBJs & Texturs/Box & Boll/box.obj", 1.0f, 0.0f, 0.5f, 0.0f);
	readMTL(BoxAlbedo, "OBJs & Texturs/Box & Boll/box.mtl");

	D3D11_BUFFER_DESC TriangleBufferDescSmall_OBJ;
	memset(&TriangleBufferDescSmall_OBJ, 0, sizeof(TriangleBufferDescSmall_OBJ));
	TriangleBufferDescSmall_OBJ.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	TriangleBufferDescSmall_OBJ.Usage = D3D11_USAGE_DEFAULT;
	TriangleBufferDescSmall_OBJ.ByteWidth = sizeof(OBJFormat) * 36;

	D3D11_SUBRESOURCE_DATA BoxData;
	BoxData.pSysMem = Box;
	gDevice->CreateBuffer(&TriangleBufferDescSmall_OBJ, &BoxData, &gVertexBuffer_Box);

	D3D11_SUBRESOURCE_DATA BoxData_MTL;
	BoxData_MTL.pSysMem = BoxAlbedo;
	gDevice->CreateBuffer(&TriangleBufferDescNormal_MTL, &BoxData_MTL, &MTLBuffer_Box);

	//-----------------------------------------------	Floor, Light

	D3D11_SUBRESOURCE_DATA FloorData;
	FloorData.pSysMem = Floor;
	gDevice->CreateBuffer(&TriangleBufferDescSmall_OBJ, &FloorData, &gVertexBuffer_Floor);

	D3D11_SUBRESOURCE_DATA LightData;
	LightData.pSysMem = light;
	gDevice->CreateBuffer(&TriangleBufferDescSmall_OBJ, &LightData, &gVertexBuffer_Light);

	//-----------------------------------------------	Boll

	OBJFormat* Boll = new OBJFormat[2280];
	MTLFormat* BollAlbedo = new MTLFormat[1];

	readOBJ(Boll, "OBJs & Texturs/Box & Boll/sphere1.obj", 1.0f, 0.0f, 2.0f, 0.0f);
	readMTL(BollAlbedo, "OBJs & Texturs/Box & Boll/sphere1.mtl");

	D3D11_BUFFER_DESC TriangleBufferDescBig_OBJ;
	memset(&TriangleBufferDescBig_OBJ, 0, sizeof(TriangleBufferDescBig_OBJ));
	TriangleBufferDescBig_OBJ.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	TriangleBufferDescBig_OBJ.Usage = D3D11_USAGE_DEFAULT;
	TriangleBufferDescBig_OBJ.ByteWidth = sizeof(OBJFormat) * 2280;

	D3D11_SUBRESOURCE_DATA BollData;
	BollData.pSysMem = Boll;
	gDevice->CreateBuffer(&TriangleBufferDescBig_OBJ, &BollData, &gVertexBuffer_Boll);

	D3D11_SUBRESOURCE_DATA BollData_MTL;
	BollData_MTL.pSysMem = BollAlbedo;
	gDevice->CreateBuffer(&TriangleBufferDescNormal_MTL, &BollData_MTL, &gConstantBuffer_Boll);

	//-----------------------------------------------	Dragon

	OBJFormat* Drake = new OBJFormat[231288];
	MTLFormat* DrakeAlbedo = new MTLFormat[1];

	readOBJ(Drake, "OBJs & Texturs/drake/Figurine Dragon N170112.obj", 0.01f, 0.0f, 0.7f, 3.0f);
	readMTL(DrakeAlbedo, "OBJs & Texturs/drake/Figurine Dragon N170112.mtl");

	D3D11_BUFFER_DESC TriangleBufferDescDrake_OBJ;
	memset(&TriangleBufferDescDrake_OBJ, 0, sizeof(TriangleBufferDescDrake_OBJ));
	TriangleBufferDescDrake_OBJ.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	TriangleBufferDescDrake_OBJ.Usage = D3D11_USAGE_DEFAULT;
	TriangleBufferDescDrake_OBJ.ByteWidth = sizeof(OBJFormat) * 231288;

	D3D11_SUBRESOURCE_DATA DrakeData;
	DrakeData.pSysMem = Drake;
	gDevice->CreateBuffer(&TriangleBufferDescDrake_OBJ, &DrakeData, &gVertexBuffer_Drake);

	D3D11_SUBRESOURCE_DATA DrakeData_MTL;
	DrakeData_MTL.pSysMem = DrakeAlbedo;
	gDevice->CreateBuffer(&TriangleBufferDescNormal_MTL, &DrakeData_MTL, &MTLBuffer_Drake);
}

void SetViewport(){
	D3D11_VIEWPORT viewport[1];
	viewport[0].Width = ViewPortWidth = 1280.0f;
	viewport[0].Height = ViewPortHeight = 720.0f;
	viewport[0].MinDepth = 0.0f;
	viewport[0].MaxDepth = 1.0f;
	viewport[0].TopLeftX = 0;
	viewport[0].TopLeftY = 0;

	gDeviceContext->RSSetViewports(1, viewport);
}

void SetScissor() {
	D3D11_RECT rects[1];
	rects[0].left = 0;
	rects[0].right = (LONG)ViewPortWidth;
	rects[0].top = 0;
	rects[0].bottom = (LONG)ViewPortHeight;

	gDeviceContext->RSSetScissorRects(1, rects);
}

//-----

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance);	//Skapar fönstret

	if (wndHandle) {
		CreateDirect3DContext(wndHandle);	//Skapa och koppla SwapChain, Device och Device Context

		SetViewport();
		SetScissor();
		CreateMatrixObjects();
		CreateLightObjects();
		CreateOtherBuffers();
		CreateRastarizer();
		CreateDepth();
		CreateShaders();
		CreateTriangleData();
		CreateTexturesAndViews();

		ShowWindow(wndHandle, nCmdShow);

		SetCursorPos(960, 540);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Clock();
				Mouse();
				Keyboard();
				Update();

				Render_pre();

				RenderFirstPass(gVertexBuffer_Floor, nullptr, 6, gTextureView_Box);
				RenderFirstPass(gVertexBuffer_Light, nullptr, 6, gTextureView_BTH);
				RenderFirstPass(gVertexBuffer_Box, MTLBuffer_Box, 36, gTextureView_Box);
				RenderFirstPass(gVertexBuffer_Boll, gConstantBuffer_Boll, 2280, gTextureView_BTH);
				RenderFirstPass(gVertexBuffer_Drake, MTLBuffer_Drake, 231288, gTextureView_BTH);

				RenderLightShadingPass();

				RenderFINAL();

				gSwapChain->Present(0, 0);	//Växla front- och back-buffer
			}
		}

		gVertexBuffer_Floor->Release();		//LÄGG TILL FLER OBJ:S HÄR
		gVertexBuffer_Light->Release();
		gVertexBuffer_Box->Release();
		gVertexBuffer_Boll->Release();
		gVertexBuffer_Drake->Release();

		MTLBuffer_Box->Release();		//LÄGG TILL FLER MTL:ER HÄR
		gConstantBuffer_Boll->Release();
		MTLBuffer_Drake->Release();

		//-----
		gVertexLayoutFinal->Release();
		gVertexLayoutFirstPass->Release();

		gVertexShaderFirstPass->Release();
		gVertexShaderLightShadingPass->Release();
		gVertexShaderFinal->Release();

		gGeometryShaderFirstPass->Release();

		gPixelShaderFirstPass->Release();
		gPixelShaderLightShadingPass->Release();
		gPixelShaderFinal->Release();

		//-----

		gBackbufferRTV->Release();
		gSwapChain->Release();
		gDevice->Release();
		gDeviceContext->Release();
		DestroyWindow(wndHandle);
	}

	return (int)msg.wParam;
}

void Render_pre(){

	float clearColor[] = { 0, 0, 0, 1 };

	gDeviceContext->ClearRenderTargetView(gFirstPassRTV[0], clearColor);
	gDeviceContext->ClearRenderTargetView(gFirstPassRTV[1], clearColor);
	gDeviceContext->ClearRenderTargetView(gFirstPassRTV[2], clearColor);
	gDeviceContext->ClearRenderTargetView(gFirstPassRTV[3], clearColor);
	gDeviceContext->ClearRenderTargetView(gLightShadingPassRTV, clearColor);
	gDeviceContext->ClearRenderTargetView(gBackbufferRTV, clearColor);

	gDeviceContext->ClearDepthStencilView(gDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderFirstPass(ID3D11Buffer* OBJ, ID3D11Buffer* MTL, int draws, ID3D11ShaderResourceView* texure) {
	
	UINT32 vertexSize = sizeof(OBJFormat);
	UINT32 offset = 0;

	gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gDeviceContext->IASetInputLayout(gVertexLayoutFirstPass);

	gDeviceContext->OMSetRenderTargets(4, gFirstPassRTV, gDepthStencilView);

	//--------------------

	gDeviceContext->VSSetShader(gVertexShaderFirstPass,	 nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(gGeometryShaderFirstPass, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderFirstPass, nullptr, 0);

	//--------------------

	gDeviceContext->IASetVertexBuffers(0, 1, &OBJ, &vertexSize, &offset);	//what object we are drawing

	gDeviceContext->VSSetConstantBuffers(0, 1, &MatriserBuffer);

	gDeviceContext->GSSetConstantBuffers(0, 1, &MatriserBuffer);
	gDeviceContext->GSSetConstantBuffers(1, 1, &CamPosBuffer);

	gDeviceContext->PSSetShaderResources(0, 1, &texure);
	gDeviceContext->PSSetConstantBuffers(0, 1, &MTL);

	//--------------------

	gDeviceContext->Draw(draws, 0);
}

void RenderLightShadingPass(){

	gDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//kan byta till LIST nen då krävs 6 drawcalls

	gDeviceContext->IASetInputLayout(NULL);

	gDeviceContext->OMSetRenderTargets(1, &gLightShadingPassRTV, nullptr);
	//gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);		//används för test

	//--------------------

	gDeviceContext->VSSetShader(gVertexShaderLightShadingPass, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderLightShadingPass, nullptr, 0);

	//--------------------

	gDeviceContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);

	gDeviceContext->PSSetConstantBuffers(0, 1, &LightBuffer_1);	//ljus
	gDeviceContext->PSSetConstantBuffers(1, 1, &CamPosBuffer);

	gDeviceContext->PSSetShaderResources(0, 1, &gFirstPassSRV[0]);
	gDeviceContext->PSSetShaderResources(1, 1, &gFirstPassSRV[1]);
	gDeviceContext->PSSetShaderResources(2, 1, &gFirstPassSRV[2]);
	gDeviceContext->PSSetShaderResources(3, 1, &gFirstPassSRV[3]);

	//--------------------

	gDeviceContext->Draw(3, 0);
}

void RenderFINAL() {

	gDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	gDeviceContext->IASetInputLayout(gVertexLayoutFinal);

	gDeviceContext->OMSetRenderTargets(1, &gBackbufferRTV, nullptr);
	
	//--------------------
	
	gDeviceContext->VSSetShader(gVertexShaderFinal, nullptr, 0);
	gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	gDeviceContext->PSSetShader(gPixelShaderFinal, nullptr, 0);

	gDeviceContext->PSSetShaderResources(0, 1, &gLightShadingPassSRV);

	//--------------------

	gDeviceContext->Draw(3, 0);
}

//-----

//skapa fönstret
HWND InitWindow(HINSTANCE hInstance){
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = WndProc;
	wcex.hInstance      = hInstance;
	wcex.lpszClassName = L"D3D_PROJECT";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"D3D_PROJECT",
		L"Direct3D Project",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ){
	switch (message) 
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;		
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT CreateDirect3DContext(HWND wndHandle){
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;									// one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// how swap chain is to be used
	scd.OutputWindow = wndHandle;							// the window to be used
	scd.SampleDesc.Count = 1;								// how many multisamples
	scd.Windowed = TRUE;									// windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&gSwapChain,
		&gDevice,
		NULL,
		&gDeviceContext);

	if (SUCCEEDED(hr))
	{
		// get the address of the back buffer
		ID3D11Texture2D* pBackBuffer = nullptr;
		gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		// use the back buffer address to create the render target
		gDevice->CreateRenderTargetView(pBackBuffer, NULL, &gBackbufferRTV);
		pBackBuffer->Release();
	}
	return hr;
}

//-------

void Clock(){
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx

	LARGE_INTEGER OldClock = TotalFrames;

	QueryPerformanceCounter(&TotalFrames);

	FPS = TotalFrames.QuadPart - OldClock.QuadPart;	//fungerar ej ???
}

void Update(){
	//mapped_subresource
	D3D11_MAPPED_SUBRESOURCE mappedResource1;
	D3D11_MAPPED_SUBRESOURCE mappedResource2;
	ZeroMemory(&mappedResource1, sizeof(D3D11_MAPPED_SUBRESOURCE));
	ZeroMemory(&mappedResource2, sizeof(D3D11_MAPPED_SUBRESOURCE));

	//Map
	gDeviceContext->Map(MatriserBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource1);
	gDeviceContext->Map(CamPosBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource2);

	//Update
	memcpy(mappedResource1.pData, &MatrixObject, sizeof(MatrixObject));
	memcpy(mappedResource2.pData, &CurrentCamPos, sizeof(CurrentCamPos));

	//Unmap
	gDeviceContext->Unmap(MatriserBuffer, 0);
	gDeviceContext->Unmap(CamPosBuffer, 0);
}

void Keyboard(){

	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms646293(v=vs.85).aspx

	if (GetAsyncKeyState('W') != 0) {			//W
		Forward(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState('A') != 0) {			//A
		Left(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState('S') != 0) {			//S
		Backwards(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState('D') != 0) {			//D
		Right(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState(VK_SPACE) != 0) {		//Space
		Jump(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState(VK_LBUTTON) != 0) {	//Left Click
		LeftMouse(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState(VK_RBUTTON) != 0) {	//Right Click
		RightMouse(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}
	if (GetAsyncKeyState('E') != 0) {			//E
		Use(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}

	if (GetAsyncKeyState(VK_LSHIFT) != 0) {		//Left Shift
		Down(FPS, &MatrixObject.ViewMatrix, &CurrentCamPos);
	}

}

void Mouse(){
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms648394(v=vs.85).aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms648390(v=vs.85).aspx
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd162952(v=vs.85).aspx
	//http://www.gamedev.net/topic/651600-camera-rotation-in-dx11/ww


	if ((GetAsyncKeyState(VK_ESCAPE) != 0 && MouseMoveESC == false && LastFrameESC == false) ||
		(GetAsyncKeyState(VK_ESCAPE) != 0 && MouseMoveESC == true && LastFrameESC == true)) {			//E
		MouseMoveESC = true;
		LastFrameESC = true;
	}

	else if ((GetAsyncKeyState(VK_ESCAPE) != 0 && MouseMoveESC == true && LastFrameESC == false) ||
		(GetAsyncKeyState(VK_ESCAPE) != 0 && MouseMoveESC == false && LastFrameESC == true)) {			//E
		MouseMoveESC = false;
		LastFrameESC = true;
		SetCursorPos(960, 540);
	}

	if (MouseMoveESC == false)
	{
		GetCursorPos(&mouseInfoNew);
		mouseMovement(mouseInfoNew, &MatrixObject.ViewMatrix);
		SetCursorPos(960, 540);
	}
	
	if (GetAsyncKeyState(VK_ESCAPE) != 0) {
		LastFrameESC = true;
	}
	else {
		LastFrameESC = false;
	}

}

void checkErrorBlob(HRESULT hr, std::string pos) {
	if (hr == E_INVALIDARG) {
		MessageBox(NULL,L"Blob ERROR: An invalid parameter was passed to the returning function",L"Error", MB_OK);
		return;
	}
	else if (hr == E_OUTOFMEMORY) {
		MessageBox(NULL, L"Blob ERROR: Out of memory ", L"Error", MB_OK);
		return;
	}
	else if (FAILED(hr)) {
		MessageBox(NULL, L"Blob ERROR: An unknown error occured ", L"Error", MB_OK);
		return;
	}
}