cbuffer CBuf
{	
	matrix modelWorldView;
    matrix modelCamView;
	matrix modelProjView;
};

struct VSOut
{
	float2 tex : TexCoord;
	float3 worldPos : Position;
    float3 camPos : CamPos;
	float3 fNormal : FaceNorm;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position, float3 fn : FaceNorm, float2 tex : TexCoord)
{
	VSOut vso;
	vso.tex = tex;	
	vso.worldPos = (float3)mul(float4(pos, 1.0f), modelWorldView);
    vso.camPos = (float3)mul(float4(pos, 1.0f), modelCamView);
	vso.fNormal = mul(fn, (float3x3)modelWorldView);
	vso.pos = mul(float4(pos, 1.0f), modelProjView);
	return vso;
}