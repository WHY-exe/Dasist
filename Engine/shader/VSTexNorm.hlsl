#include "VSBase.hlsli"

VSOut main(float3 pos : Position3D, float3 fn : Normal, float2 tc : Tex2D, float3 tan : Tangent)
{
    VSOut vso;
    vso.tc = tc;
    vso.ViewPos = (float3) mul(float4(pos, 1.0f), modelCamView);
    // ignore the translation of the camera and the obj
    vso.viewNorm = mul(fn, (float3x3) modelCamView);
    vso.tan = mul(tan, (float3x3) modelCamView);
    vso.pos = mul(float4(pos, 1.0f), modelProjView);
    
    return vso;
}