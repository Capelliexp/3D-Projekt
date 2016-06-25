Texture2D NormalTexture			: register(t0);
Texture2D DiffuseAlbedoTexture	: register(t1);
Texture2D SpecularAlbedoTexture : register(t2);
Texture2D PositionTexture		: register(t3);
SamplerState sampAni;

cbuffer light1:register(b0) {
	float4x4 ViewMatrixLight;
	float4 LightRange;			//fixa i CreateLightObjects()
	float3 PosLight;
	float3 ViewLight;
	int LightType;
};

cbuffer CamPos:register(b1) {
	float3 CamPosition;
};

struct VS_OUT {
	float4 position : SV_Position;
	float2 TexCoord: TexCoord;
};

float4 PS_main(in VS_OUT input) : SV_Target{

	float3 color = DiffuseAlbedoTexture.Sample(sampAni, input.TexCoord).rgb;
	float3 specular_color = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord).xyz;
	float specular_power = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord.xy).w;
	float3 normal = NormalTexture.Sample(sampAni, input.TexCoord).xyz;
	normal = (normal - 0.5) * 2;
	float3 position = PositionTexture.Sample(sampAni, input.TexCoord).xyz;
	
	//----- shading calc

	//float3 L;
	//float attenuation;

	//if (LightType == 1 || LightType == 3) {	//pointlight eller spotlight
	//	// Base the the light vector on the light position
	//	L = PosLight - position;

	//	// Calculate attenuation based on distance from the light source
	//	float dist = length(L);
	//	attenuation = max(0, 1.0f - (dist / LightRange.x));
	//	L /= dist;
	//}
	//else {	//directional light
	//	// Light direction is explicit for directional lights
	//	L = -ViewLight;	//va?!?
	//}

	//if (LightType == 3) {	//spotlight
	//	// Also add in the spotlight attenuation factor
	//	float3 L2 = ViewLight;
	//	float rho = dot(-L, L2);
	//	attenuation = attenuation * saturate((rho - SpotlightAngles.y)/(SpotlightAngles.x - SpotlightAngles.y));
	//}

	//float nDotL = saturate(dot(normal, L));
	//float3 diffuse =  (255, 255, 255) * color.rgb * nDotL;	//kan vara fel

	//// Calculate the specular term
	//float3 V = CamPosition - position;
	//float3 H = normalize(L + V);
	//float3 specular = pow(saturate(dot(normal, H)), specular_power) * (255,255,255) * specular_color.xyz * nDotL;	//denna rad kan vara fel
	//
	//// Final value is the sum of the albedo and diffuse with attenuation applied
	//float4 shading;
	//shading.x = (diffuse.x*attenuation) + (specular.x*attenuation);
	//shading.y = (diffuse.y*attenuation) + (specular.y*attenuation);
	//shading.z = (diffuse.z*attenuation) + (specular.z*attenuation);
	//shading.w = 1.0f;

	//return shading;
	return float4(color, 1.0f);
	//return float4(255, 0, 0, 1);	//hela skärmen blir röd pga fullscreen quad
}