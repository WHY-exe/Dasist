#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = normalize(vso.viewNorm);
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal,
         gDiffuseColor, gDiffuseIntensity,
        specular_intensity, specular_pow
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal,
        pDiffuseColor, pDiffuseIntensity,
        specular_intensity, specular_pow,
        false, true, pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient * (pDiffuseColor + gDiffuseColor);
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + ambient) + (pLight.Specular + gLight.Specular), 1.0f);
}