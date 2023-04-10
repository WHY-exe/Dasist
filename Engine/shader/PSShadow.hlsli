SamplerComparisonState ssplr : register(s1);
TextureCube smap : register(t4);
static const float zf = 500.0f;
static const float zn = 0.5f;
static const float c1 = zf / (zf - zn);
static const float c0 = -zn * zf / (zf - zn);

float CalculateShadowDepth(const in float4 shadowPos)
{
    // get magnitudes for each basis component
    const float3 m = abs(shadowPos).xyz;
    // get the length in the dominant axis
    // (this correlates with shadow map face and derives comparison depth)
    const float major = max(m.x, max(m.y, m.z));
    // converting from distance in shadow light space to projected depth
    return (c1 * major + c0) / major;
}

float PCFShadowed(const in float4 shadow_pos)
{
    return smap.SampleCmpLevelZero(ssplr, shadow_pos.xyz, CalculateShadowDepth(shadow_pos));
}