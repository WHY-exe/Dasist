#include "VSBase.hlsli"
cbuffer ShadowBuffer : register(b1)
{
    uint cur_light_num;
    matrix shadowView[16];
};

float4 ToShadowScreenSpace(const in float4 shadow_world_pos, uniform matrix shadowView)
{
    float4 shadowViewPos = mul(shadow_world_pos, shadowView);
    // bring the position from NDC space into texture coordinate
    return shadowViewPos * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowViewPos.w);
}