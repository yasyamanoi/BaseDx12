
cbuffer ShadowmapConstantBuffer : register(b0)
{
	float4x4 World				: packoffset(c0);
	float4x4 View				: packoffset(c4);
	float4x4 Projection			: packoffset(c8);
};

struct VS_INPUT
{
	float4 vPosition    : POSITION;
};

float4 main(VS_INPUT Input) : SV_POSITION
{
	Input.vPosition.w = 1.0f;
	float4 pos;
	pos = mul(Input.vPosition, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	return pos;
}