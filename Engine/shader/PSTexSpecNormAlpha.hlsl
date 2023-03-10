#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    const float4 SpecularSample = spec.Sample(splr, vso.tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
    float3 ViewNormal = vso.viewNorm;
    if(enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
    [flatten]
    if (dot(ViewNormal, vso.ViewPos.xyz) > 0.0f)
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
    float4 matSpecColor = spec_color;
    return float4(saturate(gLight.Diffuse + pLight.Diffuse) * matDiffuse.rgb +
                saturate(gAmbientColor + pAmbientColor) * matAmbient.rgb +
                saturate(pLight.Specular + gLight.Specular) * SpecularReflectionColor, max(matDiffuse.a, matAmbient.a));
}