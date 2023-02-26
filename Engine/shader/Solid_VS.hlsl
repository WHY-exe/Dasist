#include "VSBase.hlsli"

float4 main(float3 pos : Position3D, float3 fn : Normal, float3 tan : Tangent, float2 tc : Tex2D) : SV_POSITION
{
    float4 pos_out = mul(float4((pos - center_point), 1.0f), modelScalin) + float4(center_point, 0.0f);
    return mul(pos_out, modelProjView);
}