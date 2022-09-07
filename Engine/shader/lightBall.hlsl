cbuffer LightCBuf
{
    float3 lightPos;
    float3 diffuseColor;
}



float4 main() : SV_TARGET
{
	return float4(diffuseColor, 1.0f);
}