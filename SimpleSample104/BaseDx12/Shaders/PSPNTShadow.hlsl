//--------------------------------------------------------------------------------------
// File: PSPNTShadow.hlsl
//
//--------------------------------------------------------------------------------------

struct PSPNTInputShadow
{
	float4 position : SV_Position;
	float3 norm   : NORMAL0;
	float3 lightRay		: NORMAL1;
	float3 lightView : NORMAL2;
	float2 tex : TEXCOORD0;
	float4 lightSpacePos : POSITION1;
};

cbuffer ParametersBuffer : register(b0)
{
	float4x4 World	: packoffset(c0);
	float4x4 View	: packoffset(c4);
	float4x4 Projection	: packoffset(c8);
	float4 LightPos	: packoffset(c12);
	float4 EyePos	: packoffset(c13);
	float4x4 LightView	: packoffset(c14);
	float4x4 LightProjection	: packoffset(c18);
};

// 深度マップ
Texture2D g_DepthMap : register(t0);
Texture2D<float4> g_texture : register(t1);
SamplerComparisonState g_SamplerDepthMap : register(s0);
SamplerState g_sampler : register(s1);

float3 DplusS(float3 N, float3 L, float NdotL, float3 view);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PSPNTInputShadow input) : SV_TARGET
{
		//テクスチャとデフィーズからライティングを作成
	float4 RetColor = float4(1, 1, 1, 1);
	RetColor = g_texture.Sample(g_sampler, input.tex) * RetColor;
	RetColor = saturate(RetColor);

	//影の濃さ
	const float3 ambient = float3(0.7f, 0.7f, 0.7f);
	float3 N = normalize(input.norm);
	float3 L = normalize(input.lightRay);
	float NdotL = dot(N, L);

	float2 shadowTexCoords;
	shadowTexCoords.x = 0.5f + (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f);
	shadowTexCoords.y = 0.5f - (input.lightSpacePos.y / input.lightSpacePos.w * 0.5f);
	float pixelDepth = input.lightSpacePos.z / input.lightSpacePos.w;

	float lighting = 1;

	if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
		(saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
		(pixelDepth > 0))
	{

		float margin = acos(saturate(NdotL));
		float epsilon = 0.0001 / margin;

		epsilon = clamp(epsilon, 0, 0.1);

		lighting = float(g_DepthMap.SampleCmpLevelZero(
			g_SamplerDepthMap,
			shadowTexCoords,
			pixelDepth + epsilon
			)
			);
		if (lighting == 0.f)
		{
			//影の中
			return float4(RetColor.xyz * ambient, RetColor.w);
		}
		else if (lighting < 1.0f)
		{
			//影と影以外の境界線
			float3 light = lighting * (ambient + DplusS(N, L, NdotL, input.lightView));
			float3 shadow = (1.0f - lighting) * ambient;
			return float4(RetColor.xyz * (light + shadow), RetColor.w);
		}
	}
	return float4(RetColor.xyz * (ambient + DplusS(N, L, NdotL, input.lightView)), RetColor.w);
}


float3 DplusS(float3 N, float3 L, float NdotL, float3 view)
{
	const float3 Kdiffuse = float3(.5f, .5f, .4f);
	const float3 Kspecular = float3(.2f, .2f, .3f);
	const float exponent = 3.f;

	// Compute the diffuse coefficient.
	float diffuseConst = saturate(NdotL);

	// Compute the diffuse lighting value.
	float3 diffuse = Kdiffuse * diffuseConst;

	// Compute the specular highlight.
	float3 R = reflect(-L, N);
	float3 V = normalize(view);
	float3 RdotV = dot(R, V);
	float3 specular = Kspecular * pow(saturate(RdotV), exponent);

	return (diffuse + specular);
}
