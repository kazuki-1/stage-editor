// This is the phongLambert shader
// This is the most used shader in the entire project
// All models are by default loaded with a phongShader






// Calculates the lambert diffuse value of the coordinates based on the position of the vertex and the light sources
float3 CalculateLambertDiffuse(float3 normal, float3 lightDir, float3 lightColour, float3 kd)
{
    float d = max(0, dot(lightDir, normal));
    return d * lightColour * kd;
}

// Calculates the phong specular value of the coordinates based on the position of the vertex and the light sources
float3 CalculatePhongSpecular(float3 normal, float3 lightVector, float3 lightColour, float3 eyeVector, float shineFactor, float3 ks)
{
    float3 r = reflect(-lightVector, normal);
    float d = max(0, pow(max(0, dot(r, eyeVector)), shineFactor));
    return d * lightColour * ks;
}

// Slight difference from Lambert, the shading is lighter for this, unused but left in just in case
float3 CalculateHalfLambertDiffuse(float3 normal, float3 lightDir, float3 lightColour, float3 kd)
{
    float d = dot(lightDir, normal) * 0.5f + 0.5f;
    d = clamp(d, 0, 1);
    return d * lightColour * kd;
}

// Calculate back lighting on the vertex, unused but left in just in case
float3 CalculateRimLighting(float3 normal, float3 eyeVector, float3 lightVector, float3 lightColour, float rimPower = 3.0f)
{
    float rim = 1.0f - saturate(dot(eyeVector, normal));
    rim = pow(rim, rimPower);
    return lightColour * rim * saturate(dot(eyeVector, -lightVector));

}

// Gives the vertices a more toonish feel shading, unused but left in just in case
float3 CalculateToonDiffuse(Texture2D t2d, SamplerState tss, float3 normal, float3 lightVector, float3 lightColour, float3 kd)
{
    float u = clamp(dot(normal, lightVector), 0, 1);
    float c = t2d.Sample(tss, float2(u, 0.1f));
    return lightColour * c * kd;
}

float3 CalculateShadowUV(float3 world, matrix light)
{
    float4 wvp_position = mul(float4(world, 1), light);
    wvp_position /= wvp_position.w;
    wvp_position.x = (wvp_position.x + 1) * 0.5;
    wvp_position.y = (wvp_position.y * -1 + 1) * 0.5;

    return wvp_position.xyz;
}

float3 CalculateShadowColour(Texture2D tex, SamplerState samp, float3 uv, float3 colour, float bias)
{
    float2 texelSize;
    uint width, height;
    tex.GetDimensions(width, height);
    texelSize = float2(1.0f / width, 1.0f / height);


    float factor = 0;
    static const int PCFkernalSize = 5;
    for (int x = (-PCFkernalSize / 2); x <= PCFkernalSize / 2; ++x)
    {
        for (int y = (-PCFkernalSize / 2); y <= PCFkernalSize / 2; ++y)
        {

            float depth = tex.Sample(samp, uv.xy + texelSize * float2(x, y)).r;
            factor += step(uv.z - depth, bias);
        }
    }

    return lerp(colour, 1, factor / (PCFkernalSize * PCFkernalSize));
}



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

#pragma endregion

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
    float2 placeholder;
}

// Mesh Constant buffer
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
    // Cast the vertex based on their bone influences
    for (int ind = 0; ind < 4; ++ind)
    {
        n_Pos += vin.weights[ind] * mul(float4(vin.position.xyz, 1.0f), b_Transform[vin.bones[ind]]);
        n_Normal += vin.weights[ind] * mul(float4(vin.normal.xyz, 0.0f), b_Transform[vin.bones[ind]]);
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
Texture2D shadowM : register(t4);
SamplerState Sampler : register(s0);



float4 PS_MAIN(VS_OUT pin) : SV_TARGET
{

    // Perform normal calculation based on the normal map
    // If no normal map is provided, models will default to a purple plane texture
    // To see creation of this default normal, refer to Texture.h
    float4 diffuseColour = diffuseM.Sample(Sampler, pin.uv) * pin.colour;
    float3 normal = normalM.Sample(Sampler, pin.uv);
    normal.xyz = (normal.xyz - 0.5) * 2;
    float3x3 CM = { normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal) };
    float3 N = mul(normal, CM);
    float3 L = normalize(-directional.direction.xyz);
    float3 E = normalize(viewPosition.xyz - pin.world_position);
    N = normalize(N);


    float3 kd = { 1, 1, 1 };
    float3 ks = { 1, 1, 1 };
    float3 ka = { 1, 1, 1 };
    float shineFactor = 128;

    float3 DirectionalSpecular = CalculatePhongSpecular(N, L, directional.colour.rgb, E, shineFactor, ks);
    float3 DirectionalDiffuse = CalculateLambertDiffuse(N, L, directional.colour.rgb, kd);
    float3 ambient = ka * ambientLightColour.rgb;


    // Perform pointLight calculation
    float3 PointDiffuse = float3(0, 0, 0);
    float3 PointSpecular = float3(0, 0, 0);
    for (int a = 0; a < pLightCount; ++a)
    {
        float3 lightVector = pin.world_position.xyz - pointlights[a].position.xyz;
        lightVector *= -1;
        float lightLength = length(lightVector);
        if (lightLength > pointlights[a].range)
            continue;
        float weaken = max(0.0f, 1.0f - (lightLength / pointlights[a].range));
        weaken = clamp(weaken, 0.0f, 1.0f);
        weaken *= weaken;
        float3 pl_Norm = normalize(lightVector);
        PointDiffuse += CalculateLambertDiffuse(N, pl_Norm, pointlights[a].colour.rgb, kd.rgb) * weaken * pointlights[a].intensity;
        PointSpecular += CalculatePhongSpecular(N, pl_Norm, pointlights[a].colour.rgb, E, shineFactor, ks.rgb) * weaken * pointlights[a].intensity;
    }

    // Perform spotLight calculation
    float3 SpotDiffuse = { 0, 0, 0 };
    float3 SpotSpecular = { 0, 0, 0 };
    for (int b = 0; b < sLightCount; ++b)
    {
        float3 lightVector = pin.world_position.xyz - spotlights[b].position;
        float lightLength = length(lightVector);
        if (lightLength > spotlights[b].range)
            continue;
        float weaken = max(0.0f, 1.0f - (lightLength / spotlights[b].range));
        weaken = clamp(weaken, 0.0f, 1.0f);


        float3 normalizedLV = normalize(lightVector);
        float angle = dot(normalizedLV, spotlights[b].direction);
        float area = spotlights[b].innerCorner - spotlights[b].outerCorner;
        float weakenAngle = clamp(1.0f - ((spotlights[b].innerCorner - angle) / area), 0.0f, 1.0f);
        weaken *= weakenAngle;

        SpotDiffuse += CalculateLambertDiffuse(N, -normalizedLV, spotlights[b].colour.rgb, kd.rgb) * weaken;
        SpotSpecular += CalculatePhongSpecular(N, -normalizedLV, spotlights[b].colour.rgb, E, shineFactor, ks.rgb) * weaken;

    }

    float4 colour = float4(ambient, diffuseColour.a);
    colour.rgb += diffuseColour.rgb * (DirectionalDiffuse + PointDiffuse + SpotDiffuse);
    colour.rgb += DirectionalSpecular + PointSpecular + SpotSpecular;
    return colour;

}



