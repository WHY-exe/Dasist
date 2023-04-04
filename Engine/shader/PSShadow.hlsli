SamplerState ssplr : register(s1);
Texture2D smap : register(t4);

bool IsNotShadowed(float4 shadow_pos)
{
    shadow_pos.xyz = shadow_pos.xyz / shadow_pos.w;
    return shadow_pos.z > 1.0f ? true : smap.Sample(ssplr, shadow_pos.xy).r > (shadow_pos.z - 0.005);
}