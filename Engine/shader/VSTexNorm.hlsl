cbuffer CBuf : register(b0)
{
    matrix modelWorldView;
    matrix modelWorldProjView;
    matrix modelCamView;
    matrix modelProjView;
};

struct VSOut
{
    float2 tex : Tex2D;
    float3 worldPos : Position3D;
    float3 camPos : CamPos;
    float3 fNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position3D, float3 fn : Normal, float2 tex : Tex2D, float3 tan : Tangent, float3 bitan : Bitangent)
{
    VSOut vso;
    vso.tex = tex;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), modelWorldView);
    vso.camPos = (float3) mul(float4(pos, 1.0f), modelCamView);
    vso.fNormal = mul(fn, (float3x3) modelWorldView);
    vso.tan = mul(tan, (float3x3) modelWorldView);
    vso.bitan = mul(bitan, (float3x3) modelWorldView);
    vso.pos = mul(float4(pos, 1.0f), modelProjView);
    
    return vso;
}