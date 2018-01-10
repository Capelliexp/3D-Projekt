Texture2D txDiffuse : register(t0);
SamplerState sampAni;

struct PSInput{
	float4 PositionCS	: SV_Position;
	float3 PositionWS	: POSITIONWS;
	float2 TexCoord		: TEXCOORD;
	float3 NormalCS		: NORMAL;
	float3 NormalWS		: NORMALWS;
};

struct PSOutput{
	float4 Normal			: SV_Target0;
	float4 DiffuseAlbedo	: SV_Target1;
	float4 SpecularAlbedo	: SV_Target2;
	float4 Position			: SV_Target3;
};

cbuffer Albido:register(b0){
	float3 Kd;	//diffuse reflectivity
	float3 Ka;	//ambient reflectivity
	float3 Ks;	//specular reflectivity
	float Ns;	//specular exponent (higher = concentrated highlight)
	int illum;	//illumination
};

PSOutput PS_main(in PSInput input) : SV_Target{
	PSOutput output;

	//Sample object texture
	float3 diffuseAlbedo = txDiffuse.Sample(sampAni, input.TexCoord).rgb;

	float3 normal = normalize(input.NormalWS.xyz);
	normal = (normal + 1) / 2;

	//Output our G-Buffer values
	output.Normal = float4(normal, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = float4(Ks, Ns);
	output.Position = float4(input.PositionWS, 1);

	return output;
}
