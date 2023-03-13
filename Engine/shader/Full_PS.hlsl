Texture2D tex;
SamplerState splr;


float4 main(float2 uv : Texcoord) : SV_Target
{
    // negitive effect
    // return 1.0f - tex.Sample(splr, uv).rgba;
    const int r = 7;
    const float divisor = (2 * r + 1) * (2 * r + 1);
    float width, height;
    tex.GetDimensions(width, height);
    float accAlpha = 0.0f;
    float3 maxColor = float3(0.0f, 0.0f, 0.0f);
    for (int x = -r; x <= r; x++)
    {
        for (int y = -r; y <= r; y++)
        {
            const float2 tc = uv + float2(x / width , y / height);
            const float4 s = tex.Sample(splr, tc).rgba;
            accAlpha += s.a;
            maxColor = max(s.rgb, maxColor);
        }

    }
    return float4(maxColor, accAlpha / divisor);
}