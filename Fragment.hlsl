Texture2D LightShadingTexture	: register(t0);
Texture2D BloomTexture			: register(t1);
SamplerState sampAni;

struct VS_OUT {
	float4 Pos : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(VS_OUT input) : SV_Target{

	float3 color = LightShadingTexture.Sample(sampAni, input.TexCoord).rgb;

	return float4(color, 1);
}
