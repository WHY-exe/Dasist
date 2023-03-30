#include "PSBase.hlsli"
#include "lighting.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = vso.viewNorm;
    LightComponent result = { { 0, 0, 0 }, { 0, 0, 0 }, { 0.3, 0.3, 0.3 } };
    float4 matAmbient = ambient;
    float4 matSpec = spec_color;
    SetLightingPixelResult(result, specular_pow, specular_intensity, vso.ViewPos, ViewNormal);

    return float4(
        saturate(result.Diffuse) +
        saturate(result.Ambient) * matAmbient.rgb +
        saturate(result.Specular) * matSpec.rgb, 1.0f);
}