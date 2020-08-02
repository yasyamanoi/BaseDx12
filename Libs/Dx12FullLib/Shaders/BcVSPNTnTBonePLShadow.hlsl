
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: pixel lighting + texture. + tangent
// VSInputNmTxTangentWeights
VSOutputPixelLightingTxTangentShadow main(VSInputNmTxTangentWeights vin)
{
	VSOutputPixelLightingTxTangentShadow vout;

	float3 normal = SkinWithTangent(vin, vin.Normal, 4);

	CommonVSOutputPixelLighting cout = ComputeCommonVSOutputPixelLighting(vin.Position, normal);
	SetCommonVSOutputParamsPixelLighting;

	vout.Diffuse = float4(1, 1, 1, DiffuseColor.a);
	vout.TexCoord = vin.TexCoord;

	// For normal mapping, we need tangent to form tangent space transform
	vout.TangentWS = normalize(mul(vin.Tangent.xyz, WorldInverseTranspose));

	//�e�p
	vout.norm = mul(vin.Normal, (float3x3)World);
	vout.norm = normalize(vout.norm);
	float4 LightModelPos = float4(vin.Position.xyz, 1.0f);
	//���[���h�ϊ�
	LightModelPos = mul(LightModelPos, World);
	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	vout.lightSpacePos = LightSpacePos;
	// Light ray
	vout.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	vout.lightView = EyePos.xyz - LightModelPos.xyz;


	return vout;
}

