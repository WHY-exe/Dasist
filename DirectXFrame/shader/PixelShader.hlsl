cbuffer LightCBuf
{
    float3 lightPos;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLinear;
    float attQuad;
}

cbuffer OBJCBuf
{
    float3 ambient;
};

float4 main(float2 tc : TexCoord, float3 worldPos : Position, float3 fn : FaceNorm) : SV_Target
{
	// vector of the obj to the light source
    const float3 vToLight = lightPos - worldPos;
	// distance between the obj to the light source
    const float distToLight = length(vToLight);
	// Norm vector of the obj to the light source
    const float3 dirToLight = vToLight / distToLight;
	
	// caculate the light intensity attenuation
	// by the way the "*" operation in hlsl seems to be hadmard product
    const float att = 1.0f / (attConst + attLinear * distToLight + attQuad * (distToLight * distToLight));
	// diffuse intensity
	// the vector take part in the dot product caculation is the unity vector
	// so the result is the cos(theta) between the two vector
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToLight, fn));
	
    return float4(saturate(diffuse + ambient), 1.0f);
}