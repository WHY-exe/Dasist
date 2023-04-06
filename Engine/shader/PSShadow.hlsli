SamplerState ssplr : register(s1);
Texture2D smap : register(t4);

float PCFShadowed(const in float4 shadow_pos)
{
    const float3 spos = shadow_pos.xyz / shadow_pos.w;
    if (spos.z > 1.0f)
        return 1.0f;
    uint width, height;
    smap.GetDimensions(width, height);
    float dx = 0.5 / width;
    float dy = 0.5 / height;  
    float shadow_weight = 0.0f;        
    const float zBias = spos.z - 0.00005f;
    shadow_weight += smap.Sample(ssplr, spos.xy + float2( dx,  dy)).r > zBias ? 0.25f : 0.0f;
    shadow_weight += smap.Sample(ssplr, spos.xy + float2(-dx,  dy)).r > zBias ? 0.25f : 0.0f;
    shadow_weight += smap.Sample(ssplr, spos.xy + float2( dx, -dy)).r > zBias ? 0.25f : 0.0f;
    shadow_weight += smap.Sample(ssplr, spos.xy + float2(-dx, -dy)).r > zBias ? 0.25f : 0.0f;
    return shadow_weight;
}