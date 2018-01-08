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
	float2 TexCoord : TexCoord;
};

float3 CalcLighting(float LightRange, float3 PosLight, float3 ViewLight, float2 SpotlightAngles, float3 LightColor, uint LightType, float3 color, float3 specular_color, float3 specular_power, float3 normal, float3 position, float3 CamPosition, float shadowValue) {

	float3 lightVector;
	float attenuation = 1;

	if (LightType == 1 || LightType == 3) {	//pointlight or spotlight - base the the light vector on the light position
		lightVector = PosLight.xyz - position;

		//Calculate attenuation based on distance from the light source
		float dist = length(lightVector);
		attenuation = max(0, 1.0f - (dist / LightRange));
		lightVector = lightVector / dist;
	}
	else {	//directional light - lightVector is the direction of the light
		lightVector = -ViewLight.xyz;
	}

	if (LightType == 3) {	//spotlight - add in the spotlight attenuation factor
		float rho = dot(-lightVector, ViewLight.xyz);
		attenuation = attenuation * saturate((rho - SpotlightAngles.y) / (SpotlightAngles.x - SpotlightAngles.y));
	}

	//calculate diffuse factor for the surface-to-light angle
	float nDotL = dot(normal, lightVector);
	nDotL = saturate(nDotL);
	float3 diffuse = LightColor * color.rgb * nDotL;

	// Calculate the specular term
	float3 camToPointVector = position - CamPosition;
	float3 reflectionVectorNormalized = normalize(reflect(camToPointVector, normal));

	float3 v = PosLight - position;
	float3 d = dot(v, reflectionVectorNormalized);
	float3 closestPointToLight = position + reflectionVectorNormalized * d;
	float dist = distance(closestPointToLight, PosLight);
	float3 specular = (LightColor * saturate(1 - dist));

	//Ambient
	float3 ambient = color * 0.02;

	//Final
	float3 shading = ambient + (diffuse + specular) * shadowValue * attenuation;

	return shading;
}

float readShadowMap(float3 pixelPos3D) {
	float3 camToPixel = pixelPos3D - CamPosition;
	float lightToPixelLength = length(pixelPos3D - PosLight);

	float4 newPos = mul(float4(pixelPos3D, 1.0f), mul(ViewMatrixLight, ProjectionMatrixLight));
	newPos.xyz /= newPos.w;

	float2 textureCoordinates = float2((newPos.x / 2.0) + 0.5, (newPos.y / -2.0) + 0.5);

	float depthValue = ShadowMapTexture.Sample(sampAni, textureCoordinates).r + 0.0001f;

	if (depthValue < newPos.z)
		return 0.0;
	else
		return 1.0;	//light strength

	return depthValue;
}

float4 PS_main(in VS_OUT input) : SV_Target{

	float3 color = DiffuseAlbedoTexture.Sample(sampAni, input.TexCoord).rgb;
	float3 position = PositionTexture.Sample(sampAni, input.TexCoord).xyz;
	float3 specular_color = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord).xyz;
	float specular_power = SpecularAlbedoTexture.Sample(sampAni, input.TexCoord.xy).w;
	float3 normal = NormalTexture.Sample(sampAni, input.TexCoord).xyz;
	normal = (normal - 0.5) * 2;

	float shadowValue = readShadowMap(position);

	float3 light1 = CalcLighting(LightRange.x, PosLight.xyz, ViewLight.xyz, SpotlightAngles.xy, LightColor.xyz, LightType, color, specular_color, specular_power, normal, position, CamPosition, shadowValue);

	return float4(light1, 1.0f);
	//return float4(shadowMap, 1.0f);
	//return float4(light1.x, light1.y, shadowValue, 1.0f);
	//return float4(shadowValue, shadowValue, shadowValue, 1.0f);
}