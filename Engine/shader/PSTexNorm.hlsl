#include "PSBase.hlsli"
#include "lighting.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = vso.viewNorm;
     [flatten]
    if (enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    LightComponent result = { { 0, 0, 0 }, { 0, 0, 0 }, { 0.3, 0.3, 0.3 }};
    float4 matAmbient = ambient;
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc);
    float4 matDiffuse = dmap.Sample(splr, vso.tc);
    float4 matSpec = spec_color;
    result = SetLightingPixelResult(specular_pow, specular_intensity, vso.ViewPos, ViewNormal);
    return float4(
        saturate(result.Diffuse) * matDiffuse.rgb +
        saturate(result.Ambient) * matAmbient.rgb +
        saturate(result.Specular) * matSpec.rgb, 
        1.0f);
}