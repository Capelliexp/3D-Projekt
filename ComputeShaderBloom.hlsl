

Texture2D LightShadingTexture		: register(t0);
RWTexture2D<float4> OutputTexture	: register(u0);

[numthreads(32, 32, 1)]		//ger error
void CS_main(uint3 DispatchThreadID : SV_DispatchThreadID){
	int3 texturelocation = int3(0, 0, 0);
	texturelocation.x = DispatchThreadID.x;
	texturelocation.y = DispatchThreadID.y;
	
}