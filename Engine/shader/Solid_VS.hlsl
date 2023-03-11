#include "VSBase.hlsli"

float4 main(float3 pos : Position3D, float3 fn : Normal, float3 tan : Tangent, float2 tc : Tex2D) : SV_POSITION
{
    return mul(float4(pos, 1.0f), modelProjView);
}