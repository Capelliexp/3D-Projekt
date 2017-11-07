Texture2D LightShadingTexture	: register(t0);
Texture2D<float4> BloomTexture	: register(t1);	//<--- FÅR INGEN INPUT!
SamplerState sampAni;

struct VS_OUT {
	float4 Pos : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(VS_OUT input) : SV_Target{

	float4 color1 = LightShadingTexture.Sample(sampAni, input.TexCoord).rgba;
	float4 color2 = BloomTexture.Sample(sampAni, input.TexCoord).rgba;

	/*if (color2.r == 0.0) {
		return float4(0.0f, 1.0f, 0.0f, 1.0f);
	}*/

	return float4(color1);
	//return float4(color2);
}