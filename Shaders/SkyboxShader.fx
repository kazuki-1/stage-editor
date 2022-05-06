struct VS_IN
{
    float4 position : POSITION;
    float4 colour : COLOUR;
    float2 uv : UV;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 colour : COLOUR;
    float3 world_position : POSITION;
};


cbuffer CONST_BUFFER_SKYBOX :register (b4)
{
    float4 viewPosition;
    row_major float4x4 inverseView;
    row_major float4x4 inverseProjection;
};

VS_OUT VS_MAIN(VS_IN vin)
{
    VS_OUT vout;
    vout.position = vin.position;
    vout.colour = vin.colour;
    float4 viewSpace = mul(vin.position, inverseProjection);
    viewSpace /= viewSpace.w;

    vout.world_position = mul(viewSpace, inverseView).xyz;
    return vout;
}

Texture2D tex0 : register(t0);
SamplerState samp0 : register(s0);

float4 PS_MAIN(VS_OUT pin) : SV_TARGET
{
    float3 E = normalize(pin.world_position.xyz - viewPosition.xyz);
    float2 uv;
    static float PI = 3.14159f;
    uv = float2(atan2(E.z, E.x) / 2, -atan2(E.y, length(E.xz)));
    uv = uv / PI + 0.5f;
    uv.x = 1 - uv.x;
    return tex0.Sample(samp0, uv) * pin.colour;
    
}