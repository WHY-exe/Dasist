#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal, 
         gDiffuseColor, gDiffuseIntensity,
         specular_pow, specular_intensity
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal, 
        pDiffuseColor, pDiffuseIntensity, 
        specular_pow, specular_intensity, 
        true, pAttConst, pAttLinear, pAttQuad
    );
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + ambient) * dmap.Sample(splr, vso.tc).rgb + (pLight.Specular + gLight.Specular), 1.0f);
}