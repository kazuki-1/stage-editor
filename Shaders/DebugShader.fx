struct VS_IN
{
    float3 position : POSITION;

};

struct VS_OUT
{
    float4 position : SV_POSITION;
};


struct DLIGHT_DATA
{
    float4 direction;
    float4 colour;
};
struct PLIGHT_DATA
{
    float4 position;
    float4 colour;
    float range;
    float3 temp;
};
struct SLIGHT_DATA
{
    float4 position;
    float4 direction;
    float4 colour;
    float range;
    float innerCorner;
    float outerCorner;
    float temp;
};

static const int PLIGHT_MAX = 8;
static const int SLIGHT_MAX = 8;

cbuffer CBUFFER_S : register(b0)
{
    row_major float4x4 view_proj;
    float4 viewPosition;
    float4 ambientLightColour;
    DLIGHT_DATA directional;
    PLIGHT_DATA pointlights[PLIGHT_MAX];
    SLIGHT_DATA spotlights[SLIGHT_MAX];
    int pLightCount;
    int sLightCount;
    float2 temp;
}

cbuffer CBUFFER_M : register(b1)
{
    row_major float4x4 world;
    float4 colour;
}

cbuffer CBUFFER_OUTLINE : register(b2)
{
    float4 outline_colour;
    float outline_size;
    float3 placeholder;
}

 
VS_OUT VS_MAIN(VS_IN vin)
{
    VS_OUT vout;
    vout.position = mul(float4(vin.position, 1.0f), world);
    vout.position = mul(vout.position, view_proj);
    return vout;
}

float4 PS_MAIN(VS_OUT vout) : SV_TARGET
{
    return colour;
}