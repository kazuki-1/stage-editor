struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 colour : COLOR;
	float2 uv : UV;
};

VS_OUT VS_MAIN(VS_OUT vout)
{
	vout.position.w = 1.0f;
	return vout;
};
cbuffer CONSTANT_SCENE_BUFFER
{	
	float4x4 view;
	float4x4 projection;
	float4x4 light_dir;
	
};

cbuffer CONSTANT_OBJECT_BUFFER
{
	float4x4 world;

};

Texture2D difMap : register(t0);
SamplerState difMap_sampState : register(s0);
float4 PS_MAIN(VS_OUT pin) : SV_TARGET
{
	return difMap.Sample(difMap_sampState, pin.uv) * pin.colour;
	//return pin.colour;
};
