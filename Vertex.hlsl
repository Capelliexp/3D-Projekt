struct VS_OUT {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};

VS_OUT VS_main(uint VertexID: SV_VertexID) {	//skapar en fullscreen quad
	VS_OUT output;

	output.texcoord = float2((VertexID << 1) & 2, VertexID & 2);

	if (VertexID == 0)
		output.position = float4(0.0, 0.5, 0.5, 1.0);
	else if (VertexID == 2)
		output.position = float4(0.5, -0.5, 0.5, 1.0);
	else if (VertexID == 1)
		output.position = float4(-0.5, -0.5, 0.5, 1.0);

	return output;
}