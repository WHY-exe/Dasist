Texture2D dmap : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);
Texture2D amap : register(t3);

SamplerState splr;

struct VSOut
{
    float2 tc : Tex2D;
    float3 ViewPos : ViewPos;
    float3 viewNorm : ViewNormal;
    float3 tan : Tangent;
    float4 pos : SV_Position;
};
struct LightComponent
{
    float3 Diffuse : Diffuse;
    float3 Specular : Specular;
};
struct LightAttri
{
    float3 lightViewPos;
    float3 VertexPos;
    float3 VertexNormal;
    float3 DiffuseColor;
    float3 DiffuseIntensity;
    float SpecularPow;
    bool enableAtt;
    bool enableSpecMap;
    float SpecularIntensity;
    float AttConst;
    float AttLinear;
    float AttQuad;
};

//struct lightData
//{
//    float3 LightViewPos;
//    float3 AmbientColor;
//    float3 DiffuseColor;
//    float DiffuseIntensity;
//    bool enAtt;
//    bool Enable;
//    float AttConst;
//    float AttLinear;
//    float AttQuad;
//};



cbuffer pointLightCBuf : register(b0)
{
    float3 pLightViewPos;
    float3 pAmbientColor;
    float3 pDiffuseColor;
    float pDiffuseIntensity;
    float pAttConst;
    float pAttLinear;
    float pAttQuad;
    bool pEnable;
};


cbuffer GlobalLightCBuf : register(b1)
{
    float3 gLightViewPos;
    float3 gAmbientColor;
    float3 gDiffuseColor;
    float gDiffuseIntensity;
    bool gEnable;
};

cbuffer OBJCBuf : register(b2)
{
    float4 ambient;
    float4 spec_color;
    float specular_intensity;
    float specular_pow;
    bool hasAmbient;
    bool hasGloss;
    bool enNormal;
};
cbuffer SolidColorCBuf : register(b3)
{
    float4 solid_color;
}

float3 GenNormal(
    float3 SampleNormal,
    float3 unitNormal,
    float3 tangent
)
{
    float3 normalT = normalize(SampleNormal * 2.0f - 1.0f);
    tangent = normalize(unitNormal - dot(tangent, unitNormal) * unitNormal);
    float3 bitangent = cross(unitNormal, tangent);
    float3x3 TBN = float3x3(tangent, bitangent, unitNormal);
    return mul(normalT, TBN);
};

LightComponent GetLight(
    float3 lightViewPos,
    float3 VertexPos,
    float3 VertexNormal,
    float3 DiffuseColor,
    float3 DiffuseIntensity,
    float SpecularIntensity,
    float SpecularPow,    
    bool enableAtt = false,
    float AttConst = 1.0f,
    float AttLinear = 0.0f,
    float AttQuad = 0.0f
)
{
    LightComponent lc;
    // vector of the obj to the light source
    const float3 vToLight = lightViewPos - VertexPos;
	// distance between the obj to the light source
    const float distToLight = length(vToLight);
	// Norm vector of the obj to the light source
    const float3 dirToLight = vToLight / distToLight;
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    float3 Diffuse = DiffuseColor * DiffuseIntensity * max(0.0f, dot(dirToLight, VertexNormal));

    // the reflection vector
    const float3 r = 2.0f * VertexNormal * dot(vToLight, VertexNormal) - vToLight;
    float3 Specular = Diffuse * SpecularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(VertexPos))), SpecularPow);
    if (enableAtt)
    {
        const float att = 1.0f / (AttConst + AttLinear * distToLight + AttQuad * (distToLight * distToLight));
        Diffuse *= att;
        Specular *= att;
    }
    lc.Diffuse = Diffuse;
    lc.Specular = Specular;
    return lc;
};

LightComponent GetLight( LightAttri la)
{
    if (! la.enableAtt )
        return GetLight(la.lightViewPos, la.VertexPos, la.VertexNormal,
    la.DiffuseColor, la.DiffuseIntensity, la.SpecularIntensity, la.SpecularPow,
    la.enableAtt,  la.AttConst, la.AttLinear, la.AttQuad);
    else
        return GetLight(la.lightViewPos, la.VertexPos, la.VertexNormal,
    la.DiffuseColor, la.DiffuseIntensity, la.SpecularIntensity, la.SpecularPow, la.enableSpecMap);
};