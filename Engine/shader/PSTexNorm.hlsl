Texture2D dmap : register(t0);
Texture2D nmap : register(t2);
SamplerState splr;

cbuffer pointLightCBuf : register(b0)
{
    float3 pLightViewPos;
    float3 pDiffuseColor;
    float pDiffuseIntensity;
    float pAttConst;
    float pAttLinear;
    float pAttQuad;
}


cbuffer GlobalLightCBuf : register(b1)
{
    float3 gLightViewPos;
    float3 gDiffuseColor;
    float gDiffuseIntensity;
}

cbuffer OBJCBuf : register(b2)
{
    float3 ambient;
    float specular_intensity;
    float specular_pow;
    float padding[2];
};

float4 main(float2 tc : Texure2D, float3 ViewPos : ViewPos, float3 ViewNormal : Normal, float3 tan : Tangent, float3 bitan : Bitangent) : SV_Target
{
    const float3x3 tbn = float3x3(
       normalize(tan),
       normalize(bitan),
       normalize(ViewNormal)
    );
    
    const float3 normSample = nmap.Sample(splr, tc).xyz;
    ViewNormal = 2.0f * normSample - 1.0f;
    ViewNormal = normalize(mul(ViewNormal, tbn));

    // vector of the obj to the light source
    const float3 vToGLight = gLightViewPos - ViewPos;
	// distance between the obj to the light source
    const float distToGLight = length(vToGLight);
	// Norm vector of the obj to the light source
    const float3 dirToGLight = normalize(vToGLight);
	// diffuse intensity
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 gDiffuse = gDiffuseColor * gDiffuseIntensity * max(0.0f, dot(dirToGLight, ViewNormal));
    // the reflection vector
    const float3 gr = 2.0f * ViewNormal * dot(vToGLight, ViewNormal) - vToGLight;
    const float3 gSpecular = gDiffuse * specular_intensity * pow(max(0.0f, dot(normalize(-gr), normalize(ViewPos))), specular_pow);
    
	// vector of the obj to the light source
    const float3 vToPLight = pLightViewPos - ViewPos;
	// distance between the obj to the light source
    const float distToPLight = length(vToPLight);
	// Norm vector of the obj to the light source
    const float3 dirToPLight = vToPLight / distToPLight;
	// caculate the light intensity attenuation
	// by the way the "*" operation in hlsl seems to be hadmard product
    const float att = 1.0f / (pAttConst + pAttLinear * distToPLight + pAttQuad * (distToPLight * distToPLight));
    
	// diffuse intensity
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 pDiffuse = pDiffuseColor * pDiffuseIntensity * att * max(0.0f, dot(dirToPLight, ViewNormal));
    const float3 pr = 2.0f * ViewNormal * dot(vToPLight, ViewNormal) - vToPLight;
    const float3 pSpecular = pDiffuse * specular_intensity * pow(max(0.0f, dot(normalize(-pr), normalize(ViewPos))), specular_pow);
    return float4(saturate(pDiffuse + gDiffuse + ambient) * dmap.Sample(splr, tc).rgb + (pSpecular + gSpecular), 1.0f);
}