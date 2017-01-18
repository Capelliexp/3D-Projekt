Texture2D LightShadingTexture		: register(t0);
RWTexture2D<float4> OutputTexture	: register(u0);

[numthreads(32, 18, 1)]
void CS_main(uint3 dispatchThreadID : SV_DispatchThreadID){
	OutputTexture[dispatchThreadID.xy] = float4(1.0f, 0.0f, 0.0f, 1.0f);	//red
	//OutputTexture[dispatchThreadID.xy] = LightShadingTexture[dispatchThreadID.xy];
}