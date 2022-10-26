cbuffer CBuf : register(b0)
{
    matrix modelWorldView;
    matrix modelCamView;
    matrix modelProjView;
};

struct VSOut
{
    float2 tc : Tex2D;
    float3 ViewPos : ViewPos;
    float3 viewNorm : ViewNormal;
    float3 tan : Tangent;
    float3 bitan : Bitangent;
    float4 pos : SV_Position;
};

VSOut main(float3 pos : Position3D, float3 fn : Normal, float2 tc : Tex2D, float3 tan : Tangent, float3 bitan : Bitangent)
{
    VSOut vso;
    vso.tc = tc;
    vso.ViewPos = (float3) mul(float4(pos, 1.0f), modelCamView);
    // ignore the translation of the camera and the obj
    vso.viewNorm = mul(fn, (float3x3) modelCamView);
    vso.tan = mul(tan, (float3x3) modelCamView);
    vso.bitan = mul(bitan, (float3x3) modelCamView);
    vso.pos = mul(float4(pos, 1.0f), modelProjView);
    
    return vso;
}