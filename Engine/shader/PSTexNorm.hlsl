#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal, 
         gDiffuseColor, gDiffuseIntensity,
        specular_intensity,  specular_pow
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal, 
        pDiffuseColor, pDiffuseIntensity, 
        specular_intensity, specular_pow,  
        false, true, pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient * (pDiffuseColor + gDiffuseColor);
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + Ambient) * dmap.Sample(splr, vso.tc).rgb + (pLight.Specular + gLight.Specular), 1.0f);
}