// A 2dShader that is default upon all 2dSprites

// Vertex data
struct VS_IN
{
	float4 position : POSITION;
	float4 color : COLOUR;
	float2 uv : UV;
};

// VertexShader output
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 colour : COLOUR;
	float2 uv : UV;
};

VS_OUT VS_MAIN(VS_IN vin)
{
	VS_OUT vout;
	vout.position = float4(vin.position.xyz, 1.0f);
	vout.colour = vin.color;
	vout.uv = vin.uv;
	return vin;
};


Texture2D difMap : register(t0);
SamplerState difMap_sampState : register(s0);
float4 PS_MAIN(VS_OUT pin) : SV_TARGET
{
	return difMap.Sample(difMap_sampState, pin.uv) * pin.colour;

};
