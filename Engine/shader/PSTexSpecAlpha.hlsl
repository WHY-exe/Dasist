#include "PSBase.hlsli"
#include "lighting.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    float SpecularPower = specular_pow;
    [flatten]
    if(hasGloss)
    {
        SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    }
    float3 ViewNormal = vso.viewNorm;
    LightComponent result = { { 0, 0, 0 }, { 0, 0, 0 }, { 0.3, 0.3, 0.3 }};
    float4 matAmbient = ambient;
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc);
    float4 matDiffuse = dmap.Sample(splr, vso.tc);
    float4 matSpec = spec_color;
    SetLightingPixelResult(result, SpecularPower, specular_intensity, vso.ViewPos, ViewNormal);
    return float4(
       saturate(result.Diffuse) * matDiffuse.rgb +
       saturate(result.Ambient) * matAmbient.rgb +
       saturate(result.Specular) * SpecularReflectionColor, 
       max(matDiffuse.a, matAmbient.a));
}