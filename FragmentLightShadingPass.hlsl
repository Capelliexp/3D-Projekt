Texture2D NormalTexture			: register(t0);
Texture2D DiffuseAlbedoTexture	: register(t1);
Texture2D SpecularAlbedoTexture : register(t2);
Texture2D PositionTexture		: register(t3);
Texture2D ShadowMapTexture		: register(t4);	//shadow map
SamplerState sampAni;

cbuffer light1:register(b0) {
	matrix ViewMatrixLight;
	matrix ProjectionMatrixLight;
	float4 LightRange;		//1 variable
	float4 PosLight;		//3 variables
	float4 ViewLight;		//3 variables
	float4 SpotlightAngles;	//2 variables
	float4 LightColor;		//3 variables
	uint LightType;			//1 variable
};

cbuffer CamPos:register(b1) {
	float3 CamPosition;
};

cbuffer Shadow:register(b2) {
	matrix InverseViewMatrix;
	matrix InverseProjectionMatrix;
};

cbuffer Matrices:register(b3) {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VS_OUT {
	float4 PositionQuad : SV_Position;	//position på pixel pga full screen quad
	float2 TexCoord : TexCoord;		//samma som ovan?
};

float3 CalcLighting(float LightRange, float3 PosLight, float3 ViewLight, float2 SpotlightAngles, float3 LightColor, uint LightType, float3 color, float3 specular_color, float3 specular_power, float3 normal, float3 position, float3 CamPosition) {

	float3 L;
	float attenuation = 1;

	if (LightType != 1 && LightType != 2 && LightType != 3) {
		return float3(0, 1, 0);	//green error
	}

	if (LightType == 1 || LightType == 3) {	//pointlight or spotlight
											// Base the the light vector on the light position
		L = PosLight.xyz - position;

		//Calculate attenuation based on distance from the light source
		float dist = length(L);
		attenuation = max(0, 1.0f - (dist / LightRange));
		L = L / dist;
	}
	else {	//directional light
		L = -ViewLight.xyz;
	}

	if (LightType == 3) {	//spotlight
							// Also add in the spotlight attenuation factor
		float rho = dot(-L, ViewLight.xyz);
		attenuation = attenuation * saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y));
	}

	float nDotL = dot(normal, L);	//(0.0, 1.0, 0.0), (0.5, 3.0, 0.5)
	nDotL = saturate(nDotL);
	float3 diffuse = LightColor * color.rgb * nDotL;

	// Calculate the specular term
	float3 V = CamPosition - position;
	float3 H = normalize(L + V);
	float3 specular = pow(saturate(dot(normal, H)), specular_power) * LightColor * specular_color.xyz * nDotL;

	// Final value is the sum of the albedo and diffuse with attenuation applied
	attenuation = (attenuation + 0.05) * (10 / 10.5);	//ambient
	float3 shading = (diffuse + specular) * (attenuation);
	return shading;
}

float readShadowMap(float3 pixelPos3D) {
	float3 camToPixel = pixelPos3D - CamPosition;
	float lightToPixelLength = length(pixelPos3D - PosLight);

	float4 newPos = mul(pixelPos3D, mul(ViewMatrixLight, ProjectionMatrixLight));
	newPos = newPos / newPos.w;

	float2 textureCoordinates = (newPos.xy * float2(0.5, 0.5)) + float2(0.5, 0.5);
	float depthValue = ShadowMapTexture.Sample(sampAni, textureCoordinates).r - 0.0001f;

	int returnValue = 0;
	if (depthValue*100 < lightToPixelLength) {	//shadow
		returnValue = 1;
	}

	depthValue = 1 - ((1 - depthValue) * 7);

	return depthValue;
	//return returnValue;	//returnerar alltid 0
}

float4 PS_main(in VS_OUT input) : SV_Target{

	float3 color = DiffuseAlbedoTexture.Sample(sampAni, input.TexCoord).rgb;
	float3 specular_color = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord).xyz;
	float specular_power = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord.xy).w;
	float3 normal = NormalTexture.Sample(sampAni, input.TexCoord).xyz;
	normal = (normal - 0.5) * 2;		//remove for demonstration purpose
	float3 position = PositionTexture.Sample(sampAni, input.TexCoord).xyz;
	//position = normalize(position);	//add for demonstration purposes
	float3 shadowMap = ShadowMapTexture.Sample(sampAni, input.TexCoord).r;

	float shadowValue = readShadowMap(position);

	float3 light1 = CalcLighting(LightRange.x, PosLight.xyz, ViewLight.xyz, SpotlightAngles.xy, LightColor.xyz, LightType, color, specular_color, specular_power, normal, position, CamPosition);

	//return float4(light1, 1.0f);
	//return float4(shadowMap, 1.0f);
	return float4(light1.x, light1.y, (1 - shadowValue), 1.0f);
	//return float4(shadowValue, shadowValue, shadowValue, 1.0f);
}