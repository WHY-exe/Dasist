#include "PSBase.hlsli"
#include "lighting.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = vso.viewNorm;
     [flatten]
    if (enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    if (dot(ViewNormal, vso.ViewPos.xyz) > 0.0f)
        ViewNormal = -ViewNormal;
    LightComponent result = { { 0, 0, 0 }, { 0, 0, 0 }, { 0.3, 0.3, 0.3 }};
    float4 matAmbient = ambient;
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc);
    float4 matDiffuse = dmap.Sample(splr, vso.tc);
    float4 matSpec = spec_color;
    SetLightingPixelResult(result, specular_pow, specular_intensity, vso.ViewPos, ViewNormal);
    return float4(
        saturate(result.Diffuse) * matDiffuse.rgb +
        saturate(result.Ambient) * matAmbient.rgb +
        saturate(result.Specular) * matSpec.rgb, 
        max(matDiffuse.a, matAmbient.a));
}