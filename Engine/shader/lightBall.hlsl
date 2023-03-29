#include "PSBase.hlsli" 
#include "lighting.hlsli"

float4 main() : SV_TARGET
{
	return float4(pDiffuseColor, 1.0f);
}