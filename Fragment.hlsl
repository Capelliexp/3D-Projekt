Texture2D InputTex : register(t0);	//ingen input från computeSRV
SamplerState sampAni;

struct VS_OUT {
	float4 Pos : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(VS_OUT input) : SV_Target{
	float4 color = InputTex.Sample(sampAni, input.TexCoord).rgba;
	return float4(color);
}