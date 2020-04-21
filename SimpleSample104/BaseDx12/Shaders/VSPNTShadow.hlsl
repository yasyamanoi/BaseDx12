//--------------------------------------------------------------------------------------
// File: VSPNTShadow.hlsl
//
//--------------------------------------------------------------------------------------



struct VSPNTInput
{
	float4 position : POSITION;
	float3 norm : NORMAL;
	float2 tex : TEXCOORD;
};

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


PSPNTInputShadow main(VSPNTInput input)
{
	PSPNTInputShadow result;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, World);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	//���C�e�B���O
	result.norm = mul(input.norm, (float3x3)World);
	result.norm = normalize(result.norm);
	//�e�N�X�`��UV
	result.tex = input.tex;
	//�e�̂��߂̕ϐ�
	float4 LightModelPos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	LightModelPos = mul(LightModelPos, World);

	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	result.lightSpacePos = LightSpacePos;

	// Light ray
	result.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	result.lightView = EyePos.xyz - LightModelPos.xyz;

	return result;
}


