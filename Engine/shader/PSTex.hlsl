#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, vso.viewNorm,
         gDiffuseColor, gDiffuseIntensity,
         specular_pow, specular_intensity
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, vso.viewNorm,
        pDiffuseColor, pDiffuseIntensity,
        specular_pow, specular_intensity,
        true, pAttConst, pAttLinear, pAttQuad
    );
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + ambient) * dmap.Sample(splr, vso.tc).rgb + (pLight.Specular + gLight.Specular), 1.0f);
}