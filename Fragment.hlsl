Texture2D LightShadingTexture	: register(t0);
SamplerState sampAni;

struct VS_OUT {
	float4 Pos : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(VS_OUT input) : SV_Target{

	return float4(255, 0, 0, 1);
}