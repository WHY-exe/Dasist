Texture2D tex;

SamplerState splr;

cbuffer pointLightCBuf
{
    float3 plightPos;
    float3 pDiffuseColor;
    float pDiffuseIntensity;
    float pAttConst;
    float pAttLinear;
    float pAttQuad;
}

cbuffer OBJCBuf
{
    float3 ambient;
    float specular_intensity;
    float specular_pow;
};

cbuffer GlobalLightCBuf
{
    float3 glightPos;
    float3 gDiffuseColor;
    float gDiffuseIntensity;
}

float4 main(float2 tc : Texure2D, float3 worldPos : Position3D, float3 camPos : CamPos, float3 fn : Normal) : SV_Target
{
    // vector of the obj to the light source
    const float3 vToGLight = glightPos - worldPos;
	// distance between the obj to the light source
    const float distToGLight = length(vToGLight);
	// Norm vector of the obj to the light source
    const float3 dirToGLight = vToGLight / distToGLight;
	// diffuse intensity
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 gDiffuse = gDiffuseColor * gDiffuseIntensity * max(0.0f, dot(dirToGLight, fn));
    const float3 gr = 2.0f * fn * dot(vToGLight, fn) - vToGLight;
    const float3 gSpecular = (gDiffuse * gDiffuseIntensity) * specular_intensity * pow(max(0.0f, dot(normalize(-gr), normalize(camPos))), specular_pow);
    
	// vector of the obj to the light source
	const float3 vToPLight = plightPos - worldPos;
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
    const float3 pDiffuse = pDiffuseColor * pDiffuseIntensity * att * max(0.0f, dot(dirToPLight, fn));
    const float3 pr = 2.0f * fn * dot(vToPLight, fn) - vToPLight;
    const float3 pSpecular = (pDiffuse * pDiffuseIntensity) * specular_intensity * pow(max(0.0f, dot(normalize(-pr), normalize(camPos))), specular_pow);
    return tex.Sample(splr, tc) * float4(saturate(pDiffuse + gDiffuse + ambient + pSpecular + gSpecular), 1.0f);
}