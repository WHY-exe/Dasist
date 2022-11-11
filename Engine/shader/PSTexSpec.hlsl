#include "PSBase.hlsli"
float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    float SpecularPower = specular_pow;
    [flatten]
    if(hasGloss)
    {
        SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    }
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, vso.viewNorm,
         gDiffuseColor, gDiffuseIntensity,
         1.0f, SpecularPower, false, true
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, vso.viewNorm,
        pDiffuseColor, pDiffuseIntensity,
        1.0f, SpecularPower, 
        true,pAttConst, pAttLinear, pAttQuad
    );
    float3 Ambient = ambient.rgb * (pDiffuseColor + gDiffuseColor);
    return float4(saturate(gLight.Diffuse + pLight.Diffuse + Ambient) *
                  dmap.Sample(splr, vso.tc).rgb + 
                 (pLight.Specular + gLight.Specular) * SpecularReflectionColor, dmap.Sample(splr, vso.tc).a);
}