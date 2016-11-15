struct VSInput {
	float3 Pos : POSITION;
};

struct VSOutput {
	float4 Pos : SV_POSITION;
};

cbuffer float_four_matrix:register(b0) {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

cbuffer light1:register(b1) {
	matrix ViewMatrixLight;
	matrix ProjectionMatrixLight;
	float4 LightRange;		//1 variable
	float4 PosLight;		//3 variables
	float4 ViewLight;		//3 variables
	float4 SpotlightAngles;	//2 variables
	float4 LightColor;		//3 variables
	uint LightType;			//1 variable
};

VSOutput VS_main(in VSInput input) {
	VSOutput output = (VSOutput)0;

	matrix all = mul(WorldMatrix, ViewMatrixLight);
	all = mul(all, ProjectionMatrixLight);

	output.Pos = float4(input.Pos, 1);
	output.Pos = mul(output.Pos, all);

	//output.Pos = mul(output.Pos, WorldMatrix);
	//output.Pos = mul(output.Pos, ViewMatrixLight);
	//output.Pos = mul(output.Pos, ProjectionMatrixLight);

	return output;
}
