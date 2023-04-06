SamplerComparisonState ssplr : register(s1);
Texture2D smap : register(t4);
#define PCF_RANGE 2
float PCFShadowed(const in float4 shadow_pos)
{
    const float3 spos = shadow_pos.xyz / shadow_pos.w;
    if (spos.z > 1.0f || spos.z < 0.0f)
        return 1.0f;
    float shadow_weight = 0.0f;
    float zBias = spos.z - 0.00005f;
    [unroll]
    for (int x = -PCF_RANGE; x <= PCF_RANGE; x++)
    {
        [unroll]
        for (int y = -PCF_RANGE; y <= PCF_RANGE; y++)
        {
            shadow_weight += smap.SampleCmpLevelZero(ssplr, spos.xy, zBias, int2(x, y));
        }
    }
    shadow_weight /= pow(PCF_RANGE * 2 + 1, 2);
   return shadow_weight;
}