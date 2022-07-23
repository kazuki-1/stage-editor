// DirectionalLight data
struct DLIGHT_DATA
{
    float4 direction;
    float4 colour;
};

// PointLight data
struct PLIGHT_DATA
{
    float4 position;
    float4 colour;
    float range;
    float intensity;
    float2 temp;
};

// SpotLight data
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

// Vertex Data
struct VS_IN
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : UV;
    float4 weights : WEIGHTS;
    uint4 bones : BONES;
};

// Scene Constant Buffer (Light, camera etc)
cbuffer CBUFFER_S : register(b1)
{
    row_major float4x4 view_proj;
}

// Mesh Constant buffer
cbuffer CBUFFER_M : register(b2)
{
    row_major float4x4 world;
    row_major float4x4 b_Transform[256];
    float4 colour;

}


float4 VS_MAIN(VS_IN vin) : SV_POSITION
{
    float3 p = float3(0, 0, 0);
    for (int ind = 0; ind < 4; ++ind)
        p += (vin.weights[ind] * mul(vin.position, b_Transform[vin.bones[ind]])).xyz;

    return mul(float4(p, 1.0f), view_proj);
}