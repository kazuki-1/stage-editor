// This shader is basically used as a base, 
// with the pixel shader only returning the base colour
// I used this to test if model data was properly loaded
// The difference between this and BaseShader3D.fx is that this does not 
// cast the vertex properties by the bone influences
// Light is unused here, but I left it here 
// incase there are error in byte size of the constant buffer
// This is also used on Debug primitives 


// Vertex data
struct VS_IN
{
    float3 position : POSITION;

};

// VertexShader output
struct VS_OUT
{
    float4 position : SV_POSITION;
};

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
    float3 temp;
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


// Scene Constant Buffer (Light, camera etc)
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

// Mesh Constant Buffer
cbuffer CBUFFER_M : register(b1)
{
    row_major float4x4 world;
    float4 colour;
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