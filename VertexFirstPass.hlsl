struct VSInput{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

struct VSOutput{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Norm : NORMAL;
};

cbuffer float_four_matrix:register(b0) {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

VSOutput VS_main(in VSInput input){
	VSOutput output = (VSOutput)0;

	output.Pos = float4(input.Pos, 1);
	output.Tex = input.Tex;
	output.Norm = input.Norm;

	return output;
}
