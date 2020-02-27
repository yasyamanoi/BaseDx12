struct PSPTInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
};

cbuffer SceneConstantBuffer : register(b0)
{
	float4 offset;
};


Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSPTInput input) : SV_TARGET
{
	float4 Light = g_texture.Sample(g_sampler, input.tex);
	return saturate(Light);
}

