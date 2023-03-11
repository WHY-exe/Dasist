Texture2D tex;
SamplerState splr;


float4 main(float2 uv : Texcoord) : SV_Target
{
    // negitive effect
    // return 1.0f - tex.Sample(splr, uv).rgba;
    const int r = 7;
    float width, height;
    tex.GetDimensions(width, height);
    float acc = 0.0f;
    float3 maxColor = float3(0.0f, 0.0f, 0.0f);
    for (int x = 0; x < r; x++)
    {
        for (int y = 0; y < r; y++)
        {
            const float2 tc = uv + float2(x / width , y / height);
            const float3 s = tex.Sample(splr, tc).rgb;
            acc += tex.Sample(splr, tc).a;
            maxColor = max(s, maxColor);
        }

    }
    return float4(maxColor, acc / 9);
}