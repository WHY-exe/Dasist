#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    float3 ViewNormal = vso.viewNorm;
    [flatten]
    if (enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal,
         gDiffuseColor, gDiffuseIntensity,
        specular_intensity, SpecularPower
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal,
        pDiffuseColor, pDiffuseIntensity,
        specular_intensity, SpecularPower,
        true, pAttConst, pAttLinear, pAttQuad
    );
    
    float3 matAmbient = ambient.rgb;
    [flatten]
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc).rgb;
    float3 matDiffuse = dmap.Sample(splr, vso.tc).rgb;
    float3 matSpecColor = spec_color.rgb;
    return float4(saturate( /*gLight.Diffuse + */pLight.Diffuse) * matDiffuse +
                saturate( /*gAmbientColor +*/pAmbientColor) * matAmbient +
                saturate(pLight.Specular/* + gLight.Specular*/) * SpecularReflectionColor, 1.0f);
}