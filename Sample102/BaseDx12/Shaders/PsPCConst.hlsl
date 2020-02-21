

cbuffer SceneConstantBuffer : register(b0)
{
    float4 offset;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    input.color.y = 1.0;
    return input.color;
}