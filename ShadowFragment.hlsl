Texture2D txDiffuse : register(t0);
SamplerState sampAni;

struct GS_OUT {
	float4 Pos : SV_POSITION;
	float4 PosWorld : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

//https://msdn.microsoft.com/en-us/library/windows/desktop/bb509632(v=vs.85).aspx	Packing Constant buffers
//http://gamedev.stackexchange.com/questions/55317/constant-buffer-content-packing

cbuffer Albido:register(b0) {
	float3 Kd;
	float3 Ka;
	float3 Ks;
	float Ns;
	int illum;
};

float4 PS_main(GS_OUT input) : SV_Target{
	float3 s = txDiffuse.Sample(sampAni, input.Tex).xyz;

	float3 lightsource = float3(2.5, 3, 2.5);
	float3 Pos_to_Light = lightsource - input.PosWorld.xyz;

	Pos_to_Light = normalize(Pos_to_Light);
	input.Norm = normalize(input.Norm);

	float angle = dot(Pos_to_Light, input.Norm);
	saturate(angle);

	return float4(s*angle, 1.0f);
};