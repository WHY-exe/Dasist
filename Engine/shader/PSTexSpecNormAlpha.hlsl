#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    float3 ViewNormal = vso.viewNorm;
    [flatten]
    if (dot(ViewNormal, vso.ViewPos.rgb) >= 0.0f)
        ViewNormal = -ViewNormal;
    LightComponent gLight = GetLight(
         gLightViewPos, vso.ViewPos, ViewNormal,
         gDiffuseColor, gDiffuseIntensity,
        specular_intensity, SpecularPower
    );
    LightComponent pLight = GetLight(
        pLightViewPos, vso.ViewPos, ViewNormal,
        pDiffuseColor, pDiffuseIntensity,
        specular_intensity, SpecularPower,
        true, pAttConst, pAttLinear, pAttQuad
    );
    
    float4 matAmbient = float4(ambient.rgb, 1.0f);
    [flatten]
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc);
    float4 matDiffuse = dmap.Sample(splr, vso.tc);
    // clip the pixel when the alpha value is less than 0.1
    //clip(matDiffuse.a - 0.1f);
    //clip(matAmbient.a - 0.1f);
    float4 matSpecColor = spec_color;
    return float4(saturate(gLight.Diffuse + pLight.Diffuse) * matDiffuse.rgb +
                saturate(gAmbientColor + pAmbientColor) * matAmbient.rgb +
                saturate(pLight.Specular + gLight.Specular) * SpecularReflectionColor, max(matDiffuse.a, matAmbient.a));
}