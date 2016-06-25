struct VS_OUT {
	float4 position : SV_Position;
	float2 texcoord: TexCoord;
};

VS_OUT VS_main(uint VertexID: SV_VertexID){	//skapar en fullscreen quad ???
	VS_OUT output;

	output.texcoord = float2((VertexID << 1) & 2, VertexID & 2);
	output.position = float4(output.texcoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);

	return output;
}