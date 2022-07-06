struct VSOut
{
	float4 color : Color;
	float4 pos : SV_POSITION;
};



VSOut main(float2 pos : Position, float3 color : Color)
{
	VSOut vso;
	vso.pos = float4( pos.x, pos.y , 0.0f, 1.0f );
	vso.color = float4( color, 1.0f );
	return vso;
}