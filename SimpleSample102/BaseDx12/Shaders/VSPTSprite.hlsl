struct VSPTInput
{
	float4 position : POSITION;
	float2 tex : TEXCOORD;
};

struct PSPTInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer SceneConstantBuffer : register(b0)
{
	float4 offset;
};



PSPTInput main(VSPTInput input)
{
	PSPTInput result;

	result.position = input.position + offset;
	result.tex = input.tex;

	return result;
}
