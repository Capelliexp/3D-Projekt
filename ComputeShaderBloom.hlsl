Texture2D LightShadingTexture	: register(t0);
//RWTexture2D<float> LightShadingTexture	: register(t0);		//kan vara fel, får jag skriva till denna? (nope)

[numthreads(20, 20, 1)]	//ger error

void CS_main(uint3 DispatchThreadID : SV_DispatchThreadID){
	int3 texturelocation = int3(0, 0, 0);
	texturelocation.x = DispatchThreadID.x;
	texturelocation.y = DispatchThreadID.y;



}