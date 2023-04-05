SamplerState ssplr : register(s1);
Texture2D smap : register(t4);

bool IsNotShadowed(const in float4 shadow_pos)
{
    const float3 spos = shadow_pos.xyz / shadow_pos.w;
    const float ssamp_depth = smap.Sample(ssplr, spos.xy).r;
    return spos.z > 1.0f ? true : ssamp_depth > (spos.z - 0.00005f );
}