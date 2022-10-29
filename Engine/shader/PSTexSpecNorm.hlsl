#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    float3 ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal,
         gDiffuseColor, gDiffuseIntensity,
         1.0f, SpecularPower, true, false
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal,
        pDiffuseColor, pDiffuseIntensity,
        1.0f, SpecularPower,
        true, true, pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient * (pDiffuseColor + gDiffuseColor);
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + Ambient) *
                  dmap.Sample(splr, vso.tc).rgb +
                 (pLight.Specular + gLight.Specular) * SpecularReflectionColor, 1.0f);
}