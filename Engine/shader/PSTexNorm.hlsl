#include "PSBase.hlsli"

float4 main(VSOut vso) : SV_Target
{
    float3 ViewNormal = vso.viewNorm;
    [flatten]
    if(enNormal)
        ViewNormal = GenNormal(nmap.Sample(splr, vso.tc).xyz, normalize(vso.viewNorm), vso.tan);
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
    
    float3 matAmbient = ambient.rgb;
    [flatten]
    if (hasAmbient)
        matAmbient = amap.Sample(splr, vso.tc).rgb;
    float3 matDiffuse = dmap.Sample(splr, vso.tc).rgb;
    float3 matSpecColor = spec_color.rgb;
    return float4(saturate(gLight.Diffuse + pLight.Diffuse) * matDiffuse +
                saturate(gAmbientColor + pAmbientColor) * matAmbient +
                saturate(pLight.Specular + gLight.Specular), 1.0f);
}