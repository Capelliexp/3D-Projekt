struct VS_IN {
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

struct VS_OUT {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

cbuffer float_four_matrix:register(b0) {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

VS_OUT VS_main(VS_IN input) {

	VS_OUT output = (VS_OUT)0;

	output.Pos = float4(input.Pos, 1);
	output.Tex = input.Tex;
	output.Norm = input.Norm;

	return output;
}