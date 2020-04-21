//--------------------------------------------------------------------------------------
// File: PSPVTStatic.hlsl
//
//--------------------------------------------------------------------------------------

struct PSInput
{
    float4 position    : SV_POSITION;
    float2 uv        : TEXCOORD0;
};

cbuffer cb0 : register(b0)
{
	float4x4 g_mWorldViewProj;
};


Texture2D        g_txDiffuse : register(t0);
SamplerState    g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{

    return g_txDiffuse.Sample(g_sampler, input.uv);
}
