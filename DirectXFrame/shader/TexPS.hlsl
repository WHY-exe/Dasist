Texture2D tex;

SamplerState splr;

cbuffer CBuf
{
	float3 lightPos;
    float padding1;
    float3 ambient;
    float padding2;
    float3 diffuseColor;
    float padding3;
    float diffuseIntensity;
    float attConst;
    float attLinear;
    float attQuad;
}



float4 main(float2 tc : TexCoord, float3 worldPos : Position, float3 fn : FaceNorm) : SV_Target
{
	// vector of the obj to the light source
	const float3 vToLight = lightPos - worldPos;
	// distance between the obj to the light source
	const float distToLight = length(vToLight);
	// Norm vector of the obj to the light source
	const float3 dirToLight = vToLight / distToLight;
	
	// caculate the light intensity attenuation
	const float att = 1.0f / (attConst + attLinear * distToLight + attQuad * (distToLight * distToLight));
	// diffuse intensity
	const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToLight, fn));
	
	return tex.Sample( splr, tc) * float4(saturate(diffuse + ambient), 1.0f);
}