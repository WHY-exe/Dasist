#include "PSBase.hlsli"
#include "lighting.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = vso.viewNorm;
    LightComponent result = { { 0, 0, 0 }, { 0, 0, 0 } };
    float3 lightAmbient = { 0.3, 0.3, 0.3 };
    float4 matAmbient = ambient;
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc);
    float4 matDiffuse = dmap.Sample(splr, vso.tc);
    float4 matSpec = spec_color;
    if (gEnable)
    {
        LightComponent gLight = GetLight(
             gLightViewPos, vso.ViewPos, ViewNormal,
             gDiffuseColor, gDiffuseIntensity,
             specular_intensity, specular_pow
        );
        result.Diffuse += gLight.Diffuse;
        result.Specular += gLight.Specular;
        lightAmbient += gAmbientColor;
    }
    if (pEnable)
    {
        LightComponent pLight = GetLight(
            pLightViewPos, vso.ViewPos, ViewNormal,
            pDiffuseColor, pDiffuseIntensity,
            specular_intensity, specular_pow,
            true, pAttConst, pAttLinear, pAttQuad
        );
        result.Diffuse += pLight.Diffuse;
        result.Specular += pLight.Specular;
        lightAmbient += pAmbientColor;
    }

    return float4(
        saturate(result.Diffuse) * matDiffuse.rgb +
        saturate(lightAmbient) * matAmbient.rgb +
        saturate(result.Specular) * matSpec.rgb,
        1.0f);
}