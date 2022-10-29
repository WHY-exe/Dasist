#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, vso.viewNorm,
         gDiffuseColor, gDiffuseIntensity,
         specular_intensity, specular_pow
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, vso.viewNorm,
        pDiffuseColor, pDiffuseIntensity,
        specular_intensity, specular_pow, 
        false, true, pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient * (pDiffuseColor + gDiffuseColor);
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + Ambient) * dmap.Sample(splr, vso.tc).rgb + (pLight.Specular + gLight.Specular), 1.0f);
}