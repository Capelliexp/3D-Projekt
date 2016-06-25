struct VS_OUT {
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
	float3 Normal : NORMAL;
};

struct GS_OUT {
	float4 PositionCS	: SV_Position;
	float3 PositionWS	: POSITION;
	float2 TexCoord		: TEXCOORD;
	float3 NormalCS		: NORMAL;
	float3 NormalWS		: NORMALWS;

	uint view : SV_ViewportArrayIndex;
};

cbuffer float_four_matrix:register(b0) {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
}

cbuffer Camera:register(b1) {
	float3 CamPos;
}

//https://en.wikipedia.org/wiki/Back-face_culling

[maxvertexcount(3)]
void GS_main(triangle VS_OUT input[3], inout TriangleStream<GS_OUT> OutputStream) {

	GS_OUT output = (GS_OUT)0;

	for (int i = 0; i < 3; i++) {

		output.PositionWS = mul(input[i].Pos, WorldMatrix).xyz;		//varför .xyz ???
		output.NormalWS = normalize(mul(input[i].Normal, (float3x3)WorldMatrix));	//varför endast 3x3 ???

		output.view = (int)0;		//Här kan man byta Viewport

		//----	Backface culling
		if (i == 0) {
			float3 cameraToTriangle = (float3)output.PositionWS - CamPos;
			float dotproduct = dot(cameraToTriangle, output.NormalWS);

			if (dotproduct > 0.0f) {	//cull
				OutputStream.RestartStrip();
				break;
			}
		}
		//----

		output.NormalCS = mul(output.NormalWS, (float3x3)ViewMatrix);
		output.NormalCS = normalize(mul(output.NormalCS, (float3x3)ProjectionMatrix));

		output.PositionCS = mul(input[i].Pos, WorldMatrix);
		output.PositionCS = mul(output.PositionCS, ViewMatrix);
		output.PositionCS = mul(output.PositionCS, ProjectionMatrix);

		output.TexCoord = input[i].Tex;

		

		OutputStream.Append(output);
	}
	OutputStream.RestartStrip();
}
