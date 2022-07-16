
cbuffer CBuf {
	matrix transform;
};

float4 main(float3 pos : Position, float3 pos_norm : Norm) : SV_POSITION
{
	return mul(float4(pos_norm, 1.0f), transform);
}