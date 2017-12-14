Texture2D LightShadingTexture		: register(t0);
RWTexture2D<float4> OutputTexture	: register(u0);

#define TRIGGERPOINT 1.0
#define BLEEDRATE 0.01
#define DECLINE 0.001
#define RANGE 5
#define AREA RANGE*RANGE*4

float TexelDist(const int2 orig, const int2 testTexel) {
	float a = abs(testTexel.x - orig.x);
	float b = abs(testTexel.y - orig.y);

	return sqrt(a*a + b*b);
}

[numthreads(32, 18, 1)]
void CS_main(uint3 dispatchThreadID : SV_DispatchThreadID){
	uint xCoord = dispatchThreadID.x;
	uint yCoord = dispatchThreadID.y;
	//uint z = dispatchThreadID.z;

	float4 startColor = LightShadingTexture[uint2(xCoord, yCoord)];
	float4 finalColor = startColor;

	float4 texelColorArray[RANGE*2][RANGE*2];

	int iter = 0;
	for (int x1 = 0; x1 < 2*RANGE; ++x1) {
		for (int y1 = 0; y1 < 2*RANGE; ++y1) {
			texelColorArray[x1][y1] = LightShadingTexture[uint2(xCoord+x1, yCoord+y1)];
			++iter;
		}
	}

	for (int x = 0; x < RANGE*2; ++x) {
		for (int y = 0; y < RANGE*2; ++y) {
			float constValues = TexelDist((int2)(xCoord, yCoord), (int2)(x, y)) * DECLINE * BLEEDRATE;
			if (texelColorArray[x][y].r > (startColor.r * TRIGGERPOINT)) finalColor.r += (texelColorArray[x][y].r * constValues);
			if (texelColorArray[x][y].g > (startColor.g * TRIGGERPOINT)) finalColor.g += (texelColorArray[x][y].g * constValues);
			if (texelColorArray[x][y].b > (startColor.b * TRIGGERPOINT)) finalColor.b += (texelColorArray[x][y].b * constValues);
		}
	}

	//OutputTexture[uint2(xCoord, yCoord)] = startColor;
	OutputTexture[uint2(xCoord, yCoord)] = finalColor;
}

