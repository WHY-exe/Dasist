#include "PSBase.hlsli" 
#include "lighting.hlsli"
cbuffer light_index : register(b4)
{
    int light_index;
};
float4 main() : SV_TARGET
{
	return float4(pDiffuseColor[light_index], 1.0f);
}