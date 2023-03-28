#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    float SpecularPower = specular_pow;
    [flatten]
    if (hasGloss)
    {
        SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    }
    float3 ViewNormal = vso.viewNorm;
    [flatten]
    if (enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
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
        saturate(result.Specular) * SpecularReflectionColor,
        1.0f);
}