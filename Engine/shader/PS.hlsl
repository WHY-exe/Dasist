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
        true, pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient.rgb;

    return float4(saturate(gLight.Diffuse + pLight.Diffuse + ambient.rgb) + (pLight.Specular + gLight.Specular), 1.0f);
}