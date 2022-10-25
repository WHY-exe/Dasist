cbuffer CBuf : register(b0)
{
    matrix modelWorldView;
    matrix modelCamView;
    matrix modelProjView;
};

struct VSOut
{
    float2 tc : Tex2D;
    float3 worPos : WorPos;
    float3 ViewPos : ViewPos;
    float3 fNormal : Normal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position3D, float3 fn : Normal, float2 tc : Tex2D, float3 tan : Tangent, float3 bitan : Bitangent)
{
    VSOut vso;
    vso.tc = tc;
    vso.worPos = (float3) mul(float4(pos, 1.0f), modelWorldView);
    vso.ViewPos = (float3) mul(float4(pos, 1.0f), modelCamView);
    //vso.fNormal = mul(fn, (float3x3) modelCamView);
    //vso.tan = mul(tan, (float3x3) modelCamView);
    //vso.bitan = mul(bitan, (float3x3) modelCamView);
    vso.fNormal = (float3) mul(float4(fn, 1.0f), modelWorldView);
    vso.tan = (float3) mul(float4(tan, 1.0f), modelWorldView);
    vso.bitan = (float3) mul(float4(tan, 1.0f), modelWorldView);
    vso.pos = mul(float4(pos, 1.0f), modelProjView);
    
    return vso;
}