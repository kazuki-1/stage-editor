// This shader is basically used as a base, 
// with the pixel shader only returning the base colour
// I used this to test if model data was properly loaded


// Directional Light data
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
    float3 temp;
};

// Spotlight data
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

// VertexData
struct VS_IN
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : UV;
    float4 weights : WEIGHTS;
    uint4 bones : BONES;
};

// VertexShader output
struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 world_position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 colour : COLOUR;
    float2 uv : UV;
};


// Scene Constant buffer (Light, camera etc)
cbuffer CBUFFER_S : register(b0)
{
    row_major float4x4 view_proj;
    float4 viewPosition;
    float4 ambientLightColour;
    DLIGHT_DATA directional;
}

// Mesh Constant Buffer
cbuffer CBUFFER_M : register(b1)
{
    row_major float4x4 world;
    row_major float4x4 b_Transform[256];
    float4 colour;

}


VS_OUT VS_MAIN(VS_IN vin)
{
    VS_OUT vout;

    float4 n_Pos = { 0,0, 0, 1.0f };
    float4 n_Normal = { 0, 0, 0, 0 };
    float4 n_Tangent = { 0, 0, 0, 0 };

    // Sets the parameters based on their bone influences
    for (int ind = 0; ind < 4; ++ind)
    {
        n_Pos += vin.weights[ind] * mul(float4(vin.position.xyz, 1.0f), b_Transform[vin.bones[ind]]);
        n_Normal += vin.weights[ind] * mul(float4(vin.normal, 0.0f), b_Transform[vin.bones[ind]]);
        n_Tangent += vin.weights[ind] * mul(float4(vin.tangent.xyz, 0.0f), b_Transform[vin.bones[ind]]);

    }
    vout.world_position = mul(float4(n_Pos.xyz, 1.0f), world);
    vout.position = mul(vout.world_position, view_proj);
    vout.tangent = mul(float4(n_Tangent.xyz, 0.0f), world).xyz;
    vout.normal = mul(float4(n_Normal.xyz, 0.0f), world).xyz;
    vout.tangent = normalize(vout.tangent).xyz;
    vout.normal = normalize(vout.normal).xyz;
    vout.uv = vin.uv;
    vout.colour = colour;
    vout.binormal = cross(vout.tangent, vout.normal);
    return vout;
}

Texture2D diffuseM : register(t0);
Texture2D normalM : register(t1);
SamplerState Sampler : register(s0);


float4 PS_MAIN(VS_OUT pin) : SV_TARGET
{

    float4 diffuseColour = diffuseM.Sample(Sampler, pin.uv) * pin.colour;
    return diffuseColour;
}



