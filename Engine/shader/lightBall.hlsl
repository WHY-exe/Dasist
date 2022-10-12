cbuffer pointLightCBuf
{
    float3 plightPos;
    float3 pDiffuseColor;
}



float4 main() : SV_TARGET
{
	return float4(pDiffuseColor, 1.0f);
}