//Texture2D LightShadingTexture		: register(t0);
//Texture2D BloomTexture1			: register(t0);	//<--- Kan inte läsa RWTexture2D som Texture2D?
//RWTexture2D<float4> BloomTexture2	: register(u0);

Texture2D<float4> InputTex : register(t0);
SamplerState sampAni;

struct VS_OUT {
	float4 Pos : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(VS_OUT input) : SV_Target{

	/*float4 color1 = LightShadingTexture.Sample(sampAni, input.TexCoord).rgba;
	return float4(color1);*/

	//float4 color2 = BloomTexture1.Sample(sampAni, input.TexCoord).rgba;
	//return float4(color2);

	//float4 color3 = BloomTexture2[input.TexCoord].rgba;
	//return float4(color3);

	//float4 color = InputTex.Sample(sampAni, input.TexCoord).rgba;
	float4 color = InputTex.Load(int3(input.TexCoord, 1));
	return float4(color);
}