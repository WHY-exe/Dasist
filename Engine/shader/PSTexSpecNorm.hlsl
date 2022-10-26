Texture2D tex : register(t0);
Texture2D spec : register(t1);
Texture2D nmap : register(t2);
SamplerState splr;

cbuffer pointLightCBuf : register(b0)
{
    float3 plightPos;
    float3 pDiffuseColor;
    float pDiffuseIntensity;
    float pAttConst;
    float pAttLinear;
    float pAttQuad;
}


cbuffer GlobalLightCBuf : register(b1)
{
    float3 glightPos;
    float3 gDiffuseColor;
    float gDiffuseIntensity;
}

cbuffer OBJCBuf : register(b2)
{
    float3 ambient;
    float specular_intensity;
    float specular_pow;
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
    const float3 vToGLight = glightPos - ViewPos;
	// distance between the obj to the light source
    const float distToGLight = length(vToGLight);
	// Norm vector of the obj to the light source
    const float3 dirToGLight = vToGLight / distToGLight;
    const float4 SpecularSample = spec.Sample(splr, tc);
    const float3 SpecularReflectionColor = SpecularSample.rgb;
    const float SpecularPower = pow(2.0f, SpecularSample.a * 13.0f);
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 gDiffuse = gDiffuseColor * gDiffuseIntensity * max(0.0f, dot(dirToGLight, ViewNormal));
    const float3 gr = 2.0f * ViewNormal * dot(vToGLight, ViewNormal) - vToGLight;
    const float3 gSpecular = (gDiffuse * SpecularReflectionColor) * pow(max(0.0f, dot(normalize(-gr), normalize(ViewPos))), SpecularPower);
    
	// vector of the obj to the light source
    const float3 vToPLight = plightPos - ViewPos;
	// distance between the obj to the light source
    const float distToPLight = length(vToPLight);
	// Norm vector of the obj to the light source
    const float3 dirToPLight = vToPLight / distToPLight;
	// caculate the light intensity attenuation
	// by the way the "*" operation in hlsl seems to be hadmard product
    const float att = 1.0f / (pAttConst + pAttLinear * distToPLight + pAttQuad * (distToPLight * distToPLight));
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 pDiffuse = pDiffuseColor * pDiffuseIntensity * att * max(0.0f, dot(dirToPLight, ViewNormal));
    const float3 pr = 2.0f * ViewNormal * dot(vToPLight, ViewNormal) - vToPLight;
    const float3 pSpecular = (pDiffuse * SpecularReflectionColor) * pow(max(0.0f, dot(normalize(-pr), normalize(ViewPos))), SpecularPower);
    return float4(tex.Sample(splr, tc).rgb * saturate(pDiffuse + gDiffuse + ambient) + (pSpecular + gSpecular) * SpecularReflectionColor, 1.0f);
}