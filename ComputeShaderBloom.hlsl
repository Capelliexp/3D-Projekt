Texture2D LightShadingTexture		: register(t0);
RWTexture2D<float4> OutputTexture	: register(u0);

[numthreads(32, 18, 1)]
void CS_main(uint3 dispatchThreadID : SV_DispatchThreadID){
	uint x = dispatchThreadID.x;
	uint y = dispatchThreadID.y;
	uint z = dispatchThreadID.z;
	float x_color = x * (1/200);
	//float y_color = x/18;

	//OutputTexture[uint2(x, y)] = float4(x_color, 0.0f, 0.0f, 1.0f);	//red
	OutputTexture[uint2(x, y)] = LightShadingTexture[uint2(x, y)];
}