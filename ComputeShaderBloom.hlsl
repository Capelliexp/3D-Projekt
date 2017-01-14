Texture2D LightShadingTexture		: register(t0);
RWTexture2D<float4> OutputTexture	: register(u0);

[numthreads(32, 18, 1)]		//error - "unknown attribute numthreads, or attribute invalid for this statement"
void CS_main(uint3 dispatchThreadID : SV_DispatchThreadID){
	OutputTexture[dispatchThreadID.xy] = LightShadingTexture[dispatchThreadID.xy];	//error - "vs_2_0 does not support indexing resources"
}