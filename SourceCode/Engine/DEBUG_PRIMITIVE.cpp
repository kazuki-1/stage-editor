#include "DEBUG_PRIMITIVE.h"
#include "MODEL.h"
#include "Shaders/Shader.h"
#include "Rasterizer.h"
#include "SamplerStateManager.h"
//#include "OBJECT.h"
#pragma region DEBUG_PRIMITIVE

/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_PRIMITIVE class---------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_PRIMITIVE Render()------------------------------------------------------*/

void DEBUG_PRIMITIVE::Render(Vector4 colour)
{
    model->Render(0.0f, colour.XMF4());
}

/*------------------------------------------DEBUG_PRIMITIVE Execute()------------------------------------------------------*/

void DEBUG_PRIMITIVE::Execute()
{
    model->SetTransformation(scale, rotation , position);
    model->UpdateTransform();
}

#pragma endregion
#pragma region DEBUG_CUBE
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_CUBE class--------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_CUBE Constructor--------------------------------------------------------*/


DEBUG_CUBE::DEBUG_CUBE()
{
    model = std::make_shared<MODEL>();
    model->Initialize("./Data/Model/Debug Primitives/cube.mrs");
    scale = { 1.0f, 1.0f, 1.0f };
    model->SetScale(scale);
    model->SetTake(0);
    
}

#pragma endregion
#pragma region DEBUG_CYLINDER


/*----------------------------------------------------------------------------------=====-------------------------------------*/
/*------------------------------------------DEBUG_CYLINDER class--------------------------------------------------------------*/
/*----------------------------------------------------------------------====--------------------------------------------------*/
/*------------------------------------------DEBUG_CYLINDER Constructor--------------------------------------------------------*/


DEBUG_CYLINDER::DEBUG_CYLINDER()
{
    model = std::make_shared<MODEL>();
    model->Initialize("./Data/Model/Debug Primitives/cylinder.mrs");
    scale = { 100.0f, 100.0f, 100.0f };

    model->SetScale(scale);
    model->SetTake(0);
}
DEBUG_CYLINDER::DEBUG_CYLINDER(float r) : radius(r)
{
    model = std::make_shared<MODEL>();
    model->Initialize("./Data/Model/Debug Primitives/cylinder.mrs");
    scale = { radius * 100.0f, radius * 100.0f, radius * 100.0f };

    model->SetScale(scale);
    model->SetTake(0);
}

#pragma endregion
#pragma region DEBUG_SPHERE

/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------Sphere_Debug class------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------Sphere_Debug Render()---------------------------------------------------------*/

Sphere_Debug::Sphere_Debug()
{
    model = std::make_shared<MODEL>();
    model->Initialize("./Data/Model/Debug Primitives/sphere.mrs");
    scale = { 100.0f, 100.0f, 100.0f };

    model->SetScale(scale);
    model->SetTake(0);
}
Sphere_Debug::Sphere_Debug(float r) : radius(r)
{
    model = std::make_shared<MODEL>();
    model->Initialize("./Data/Model/Debug Primitives/sphere.mrs");
    scale = { radius * 100.0f, radius * 100.0f, radius * 100.0f };

    model->SetScale(scale);
    model->SetTake(0);

}

/*------------------------------------------Sphere_Debug SetRadius()---------------------------------------------------------*/

void Sphere_Debug::SetRadius(float r)
{
    radius = r;
}

#pragma endregion
#pragma region DEBUG_ARROWS

/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_ARROWS class------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_ARROWS Initialize()-----------------------------------------------------*/

void DEBUG_ARROWS::Initialize()
{
    
    target = XMMatrixIdentity();

    xAxis = std::make_shared<MODEL>();
    yAxis = std::make_shared<MODEL>();
    zAxis = std::make_shared<MODEL>();

    Vector3 s{ 0.1f, 0.1f, 0.1f };

    xAxis->Initialize("./Data/Model/Debug Primitives/arrows.mrs");
    yAxis->Initialize("./Data/Model/Debug Primitives/arrows.mrs");
    zAxis->Initialize("./Data/Model/Debug Primitives/arrows.mrs");

    xAxis->SetTransformation(s, {0, 0, -90}, {});
    yAxis->SetTransformation(s, Vector3{}, {});
    zAxis->SetTransformation(s, {90, 0, 0}, {});

    xAxis->SetTake(0);
    yAxis->SetTake(0);
    zAxis->SetTake(0);


    xAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    yAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    zAxis->Resource()->InsertShader(ShaderTypes::Base_3D);

    xAxis->Resource()->RemoveShader(ShaderTypes::PhongShader);
    yAxis->Resource()->RemoveShader(ShaderTypes::PhongShader);
    zAxis->Resource()->RemoveShader(ShaderTypes::PhongShader);



}

/*------------------------------------------DEBUG_ARROWS Execute()-----------------------------------------------------*/

void DEBUG_ARROWS::Execute()
{

    XMMATRIX X, Y, Z;
    X = xAxis->TransformMatrix();
    Y = yAxis->TransformMatrix();
    Z = zAxis->TransformMatrix();


    Vector3 r{ ToRadians(rotation.x), ToRadians(rotation.y), ToRadians(rotation.z) };
    Vector4 q;
    q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));

    target = XMMatrixScaling(0, 0, 0) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(position.XMV());
    Vector3 x;
    x.Load({ XMVector3TransformCoord(xAxis->Translation().XMV(), target) });
    Vector3 y;
    y.Load({ XMVector3TransformCoord(yAxis->Translation().XMV(), target) });
    Vector3 z;
    z.Load({ XMVector3TransformCoord(zAxis->Translation().XMV(), target) });

    xAxis->SetTranslation(x);
    yAxis->SetTranslation(y);
    zAxis->SetTranslation(z);

    xAxis->UpdateTransform();
    yAxis->UpdateTransform();
    zAxis->UpdateTransform();


}

/*------------------------------------------DEBUG_ARROWS Render()-----------------------------------------------------*/

void DEBUG_ARROWS::Render(Vector4 colour)
{
    xAxis->Render(0.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
    yAxis->Render(0.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
    zAxis->Render(0.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
}

/*------------------------------------------DEBUG_ARROWS SetTarget()-----------------------------------------------------*/

void DEBUG_ARROWS::SetTarget(XMMATRIX t)
{
    target = t;
}

void DEBUG_ARROWS::SetTarget(Vector3 p, Vector3 r)
{
    position = p;
    rotation = r;
}

#pragma endregion
#pragma region DEBUG_SCALARS

/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_SCALARS class-----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_SCALARS Initialize()----------------------------------------------------*/

void DEBUG_SCALARS::Initialize()
{

    target = XMMatrixIdentity();

    xAxis = std::make_shared<MODEL>();
    yAxis = std::make_shared<MODEL>();
    zAxis = std::make_shared<MODEL>();

    Vector3 x, z;
    Vector3 s{ 0.1f, 0.1f, 0.1f };

    xAxis->Initialize("./Data/Model/Debug Primitives/scalar.mrs");
    yAxis->Initialize("./Data/Model/Debug Primitives/scalar.mrs");
    zAxis->Initialize("./Data/Model/Debug Primitives/scalar.mrs");

    xAxis->SetTransformation(s, { 0, 0, -90 }, {});
    yAxis->SetTransformation(s, Vector3{}, Vector3{});
    zAxis->SetTransformation(s, { 90, 0, 0 }, {});

    xAxis->SetTake(0);
    yAxis->SetTake(0);
    zAxis->SetTake(0);


    xAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    yAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    zAxis->Resource()->InsertShader(ShaderTypes::Base_3D);

}

/*------------------------------------------DEBUG_SCALARS Execute()----------------------------------------------------*/

void DEBUG_SCALARS::Execute()
{

    XMMATRIX X, Y, Z;
    X = xAxis->TransformMatrix();
    Y = yAxis->TransformMatrix();
    Z = zAxis->TransformMatrix();


    Vector3 r{ ToRadians(rotation.x), ToRadians(rotation.y), ToRadians(rotation.z) };
    Vector4 q;
    q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));

    target = XMMatrixScaling(0, 0, 0) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(position.XMV());

    Vector3 x;
    x.Load({ XMVector3TransformCoord(xAxis->Translation().XMV(), target) });
    Vector3 y;
    y.Load({ XMVector3TransformCoord(yAxis->Translation().XMV(), target) });
    Vector3 z;
    z.Load({ XMVector3TransformCoord(zAxis->Translation().XMV(), target) });

    xAxis->SetTranslation(x);
    yAxis->SetTranslation(y);
    zAxis->SetTranslation(z);

    xAxis->UpdateTransform();
    yAxis->UpdateTransform();
    zAxis->UpdateTransform();


}

/*------------------------------------------DEBUG_SCALARS Render()----------------------------------------------------*/

void DEBUG_SCALARS::Render(Vector4 colour)
{
    xAxis->Render(0.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
    yAxis->Render(0.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
    zAxis->Render(0.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
}

/*------------------------------------------DEBUG_SCALARS SetTarget()----------------------------------------------------*/

void DEBUG_SCALARS::SetTarget(XMMATRIX t)
{
    target = t;
}

void DEBUG_SCALARS::SetTarget(Vector3 p, Vector3 r)
{
    position = p;
    rotation = r;
}

#pragma endregion
#pragma region DEBUG_DISCS


/*----------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_DISCS class-----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DEBUG_DISCS Initialize()----------------------------------------------------*/

void DEBUG_DISCS::Initialize()
{

    target = XMMatrixIdentity();

    xAxis = std::make_shared<MODEL>();
    yAxis = std::make_shared<MODEL>();
    zAxis = std::make_shared<MODEL>();

    Vector3 x, z;
    Vector3 s{ 2.0f, 2.0f, 2.0f };

    xAxis->Initialize("./Data/Model/Debug Primitives/disc.mrs");
    yAxis->Initialize("./Data/Model/Debug Primitives/disc.mrs");
    zAxis->Initialize("./Data/Model/Debug Primitives/disc.mrs");

    xAxis->SetTransformation(s, Vector3{}, {});
    yAxis->SetTransformation(s, {90, 0, 0}, {});
    zAxis->SetTransformation(s, { 90, 90, 0 }, {});

    xAxis->SetTake(0);
    yAxis->SetTake(0);
    zAxis->SetTake(0);


    xAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    yAxis->Resource()->InsertShader(ShaderTypes::Base_3D);
    zAxis->Resource()->InsertShader(ShaderTypes::Base_3D);

}

/*------------------------------------------DEBUG_DISCS Execute()----------------------------------------------------*/

void DEBUG_DISCS::Execute()
{

    XMMATRIX X, Y, Z;
    X = xAxis->TransformMatrix();
    Y = yAxis->TransformMatrix();
    Z = zAxis->TransformMatrix();


    Vector3 r{ ToRadians(rotation.x), ToRadians(rotation.y), ToRadians(rotation.z) };
    Vector4 q;
    q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));

    target = XMMatrixScaling(0, 0, 0) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(position.XMV());

    Vector3 x;
    x.Load({ XMVector3TransformCoord(xAxis->Translation().XMV(), target) });
    Vector3 y;
    y.Load({ XMVector3TransformCoord(yAxis->Translation().XMV(), target) });
    Vector3 z;
    z.Load({ XMVector3TransformCoord(zAxis->Translation().XMV(), target) });

    xAxis->SetTranslation(x);
    yAxis->SetTranslation(y);
    zAxis->SetTranslation(z);

    xAxis->UpdateTransform();
    yAxis->UpdateTransform();
    zAxis->UpdateTransform();


}

/*------------------------------------------DEBUG_DISCS Render()----------------------------------------------------*/

void DEBUG_DISCS::Render(Vector4 colour)
{
    xAxis->Render(0.0f, { 1.0f, 0.0f, 0.0f, 1.0f });
    yAxis->Render(0.0f, { 0.0f, 1.0f, 0.0f, 1.0f });
    zAxis->Render(0.0f, { 0.0f, 0.0f, 1.0f, 1.0f });
}

/*------------------------------------------DEBUG_DISCS SetTarget()----------------------------------------------------*/

void DEBUG_DISCS::SetTarget(XMMATRIX t)
{
    target = t;
}


void DEBUG_DISCS::SetTarget(Vector3 p, Vector3 r)
{
    position = p;
    rotation = r;
}

#pragma endregion

#pragma region DYNAMIC_DEBUG_PRIMITIVE

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_DEBUG_PRIMITIVE class-----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_DEBUG_PRIMITIVE Initialize()----------------------------------------------------*/


HRESULT DYNAMIC_DEBUG_PRIMITIVE::Initialize()
{
    // Vertex buffer and constant buffer creation
    ID3D11Device* dv = DirectX11::Instance()->Device();
    D3D11_BUFFER_DESC vbd{};
    D3D11_SUBRESOURCE_DATA vd{};
    vbd.ByteWidth = (UINT)sizeof(VERTEX) * (UINT)vertices.size();
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd.pSysMem = vertices.data();
    HRESULT hr = dv->CreateBuffer(&vbd, &vd, vertexBuffer.GetAddressOf());
    assert(hr == S_OK);
    vbd.CPUAccessFlags = 0;
    vbd.Usage = D3D11_USAGE_DEFAULT;

    vbd.ByteWidth = sizeof(int) * (UINT)indices.size();
    vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    vd.pSysMem = indices.data();
    hr = dv->CreateBuffer(&vbd, &vd, indexBuffer.GetAddressOf());
    assert(hr == S_OK);
    vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    vbd.ByteWidth = sizeof(CBUFFER_M);
    hr = dv->CreateBuffer(&vbd, nullptr, meshConstantBuffer.GetAddressOf());
    shader = ShaderManager::Instance()->Retrieve(ShaderTypes::Debug_3D).get();

    return hr;

}

/*------------------------------------------DYNAMIC_DEBUG_PRIMITIVE Execute()----------------------------------------------------*/

void DYNAMIC_DEBUG_PRIMITIVE::Execute()
{
    // Pure virtual func
}

void DYNAMIC_DEBUG_PRIMITIVE::Execute(XMMATRIX target)
{
    // Transforms the vertices of the primitive to match the target
    for (auto& v : vertices)
        v.position.Load(XMVector3TransformCoord(v.position.XMV(), target));

}

/*------------------------------------------DYNAMIC_DEBUG_PRIMITIVE Render()----------------------------------------------------*/

void DYNAMIC_DEBUG_PRIMITIVE::Render(Vector4 colour)
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    shader->SetShaders(dc, nullptr);

    // Setup Dx required parameters and prepare to render
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    BlendStateManager::Instance()->Set(BlendModes::Alpha);
    CBUFFER_M data;
    data.colour = colour;
    Vector4 q{};
    q.Load(XMQuaternionRotationRollPitchYawFromVector(rotation.XMV()));
    XMStoreFloat4x4(&data.world, XMMatrixScaling(1, 1, 1) * XMMatrixRotationQuaternion(q.XMV()) *  XMMatrixTranslationFromVector(position.XMV()));

    UINT stride{ sizeof(VERTEX) }, offset{};

    dc->VSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);
    dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
    dc->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    dc->DrawIndexed((UINT)indices.size(), 0, 0);
}

/*------------------------------------------DYNAMIC_DEBUG_PRIMITIVE SetTarget()----------------------------------------------------*/

void DYNAMIC_DEBUG_PRIMITIVE::SetTarget(Vector3 t)
{
    position = t;
}

#pragma endregion
#pragma region DYNAMIC_CUBE

/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CUBE class------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CUBE Constructor------------------------------------------------------*/

DYNAMIC_CUBE::DYNAMIC_CUBE()
{
    // Vertice and index buffer  creation
    vertices.resize(8);
    int ind[24] =
    {
        0, 1,
        0, 2,
        1, 3,
        2, 3,
        2, 6,
        3, 7,
        0, 4,
        1, 5,
        4, 5,
        4, 6,
        5, 7,
        6, 7
    };
    for (int& i : ind)
        indices.push_back(i);
    ID3D11Device* dv = DirectX11::Instance()->Device();
    D3D11_BUFFER_DESC vbd{};
    D3D11_SUBRESOURCE_DATA vd{};
    vbd.ByteWidth = sizeof(VERTEX) * (UINT)vertices.size();
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd.pSysMem = vertices.data();
    HRESULT hr = dv->CreateBuffer(&vbd, &vd, vertexBuffer.GetAddressOf());
    assert(hr == S_OK);
    vbd.CPUAccessFlags = 0;
    vbd.Usage = D3D11_USAGE_DEFAULT;

    vbd.ByteWidth = sizeof(int) * 24;
    vbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    vd.pSysMem = indices.data();
    hr = dv->CreateBuffer(&vbd, &vd, indexBuffer.GetAddressOf());
    assert(hr == S_OK);
    vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    vbd.ByteWidth = sizeof(CBUFFER_M);
    hr = dv->CreateBuffer(&vbd, nullptr, meshConstantBuffer.GetAddressOf());

    // Uses a debug shader since this only renders lines
    shader = ShaderManager::Instance()->Retrieve(ShaderTypes::Debug_3D).get();



}

/*------------------------------------------DYNAMIC_CUBE UpdateVertices()------------------------------------------------------*/

void DYNAMIC_CUBE::UpdateVertices(std::vector<Vector3>pos)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    for (int ind = 0; ind < 8 /*cube has 8 vertices since no normal is used*/; ++ind)
    {
        vertices[ind].position = pos[ind];
    }
    D3D11_MAPPED_SUBRESOURCE res;
    dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
    memcpy(res.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
    dc->Unmap(vertexBuffer.Get(), 0);
}

#pragma endregion
#pragma region DYNAMIC_SPHERE


/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_SPHERE class------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_SPHERE Constructor------------------------------------------------------*/

DYNAMIC_SPHERE::DYNAMIC_SPHERE(int v_count, float rad) : vertices_count(v_count) , radius(rad)
{
    float angle_per_vertex{ 360.0f / (float)v_count };
    // Vertical circle
    for (int ind = 0; ind < vertices_count; ++ind)
    {
        VERTEX vert = { { 0.0f, cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertices_count - 1)
        {
            indices.push_back(ind);
            indices.push_back(0);
        }
        indices.push_back(ind);
        indices.push_back(ind - 1);
    }
    // Horizontal circle
    for (int ind = 0; ind < vertices_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, 0.0f , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertices_count - 1)
        {
            indices.push_back(vertices_count + ind);
            indices.push_back(vertices_count);
        }
        indices.push_back(vertices_count + ind);
        indices.push_back(vertices_count + ind - 1);
    }
    // Forward circle
    for (int ind = 0; ind < vertices_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius, 0.0f } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertices_count - 1)
        {
            indices.push_back(vertices_count * 2 + ind);
            indices.push_back(vertices_count * 2);
        }
        indices.push_back(vertices_count * 2 + ind);
        indices.push_back(vertices_count * 2 + ind - 1);
    }

    Initialize();
}

/*------------------------------------------DYNAMIC_SPHERE UpdateVertices()------------------------------------------------------*/

void DYNAMIC_SPHERE::UpdateVertices(float rad, XMMATRIX* target)
{
    float angle_per_vertex{ 360.0f / (float)vertices_count };
    radius = rad;

    // Vertical circle
    for (int ind = 0; ind < vertices_count; ++ind)
    {
        vertices[ind] = { { 0.0f, cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
    }

    // X axis Horizontal circle
    for (int ind = vertices_count * 1; ind < vertices_count * 2; ++ind)
    {
        vertices[ind] = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, 0.0f , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
    }

    // Z axis horizontal circle
    for (int ind = vertices_count * 2; ind < vertices_count * 3; ++ind)
    {
        vertices[ind] = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius, 0.0f } };
    }

    if (target != nullptr)
        Execute(*target);

    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    
    D3D11_MAPPED_SUBRESOURCE mrs;
    dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mrs);
    memcpy(mrs.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
    dc->Unmap(vertexBuffer.Get(), 0);


}

/*------------------------------------------DYNAMIC_SPHERE SetRadius()------------------------------------------------------*/

void DYNAMIC_SPHERE::SetRadius(float r)
{
    radius = r;
}

#pragma endregion
#pragma region DYNAMIC_CYLINDER

/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CYLINDER class------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CYLINDER Constructor------------------------------------------------------*/

DYNAMIC_CYLINDER::DYNAMIC_CYLINDER(float rad, int vert_count) : radius(rad), vertex_count(vert_count)
{
    float angle_per_vertex{ 360.0f / (float)vert_count };

    // Top circle vertex and index creation
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(ind);
            indices.push_back(0);
        }
        indices.push_back(ind);
        indices.push_back(ind - 1);
    }

    // Bottom circle vertex and index creation
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, -height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(vertex_count + ind);
            indices.push_back(vertex_count);
        }
        indices.push_back(vertex_count + ind);
        indices.push_back(vertex_count + ind - 1);
    }

    // the index of the lines between the top and bottom circles
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        indices.push_back(vertex_count + ind);
        indices.push_back(ind);
    }
    Initialize();
}

/*------------------------------------------DYNAMIC_CYLINDER UpdateVertices()------------------------------------------------------*/

void DYNAMIC_CYLINDER::UpdateVertices(float rad, float h)
{
    float angle_per_vertex{ 360.0f / (float)vertex_count };
    height = h;
    std::vector<VERTEX>vs;

    // Top circle
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vs.push_back(vert);
    }

    // Bottom circle
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, -height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vs.push_back(vert);
        if (ind == 0)
            continue;
    }
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    D3D11_MAPPED_SUBRESOURCE mrs;
    dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mrs);
    memcpy(mrs.pData, vs.data(), sizeof(VERTEX) * vs.size());
    dc->Unmap(vertexBuffer.Get(), 0);
}

#pragma endregion
#pragma region DYNAMIC_CAPSULE

/*---------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CAPSULE class------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------DYNAMIC_CAPSULE Constructor------------------------------------------------------*/

DYNAMIC_CAPSULE::DYNAMIC_CAPSULE(float rad , int vert_count, float ht) : radius(rad), vertex_count(vert_count), height(ht)
{

    float angle_per_vertex{ 360.0f / (float)vertex_count };

#pragma region HORIZONTAL CIRCLES 
     // Top horizontal circle
     for (int ind = 0; ind < vertex_count; ++ind)
     {
         VERTEX vert = { {cosf(ToRadians(angle_per_vertex) * ind) * radius , height / 2  , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
         vertices.push_back(vert);
         if (ind == 0)
             continue;
         if (ind == vertex_count - 1)
         {
             indices.push_back(ind);
             indices.push_back(0);
         }
         indices.push_back(ind);
         indices.push_back(ind - 1);
     }
 
 
    // Bottom horizontal circle
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, -height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(vertex_count + ind);
            indices.push_back(vertex_count);
        }
        indices.push_back(vertex_count + ind);
        indices.push_back(vertex_count + ind - 1);
    }

#pragma endregion

    // Lines between top and bottom circles
    for (int ind = 0; ind < 32; ++ind)
    {
        indices.push_back(vertex_count + ind);
        indices.push_back(ind);
    }
#pragma region VERTICAL CIRCLES

    // Top vertical circle
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f , height / 2 + cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count * 2 - 1)
        {
            indices.push_back(vertex_count  * 2 + ind);
            indices.push_back(vertex_count  * 2 + 0);
        }
        indices.push_back(vertex_count * 2 + ind);
        indices.push_back(vertex_count * 2 + ind - 1);
    }

    // Bottom vertical circle
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f , -height / 2 + cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(vertex_count * 3 +ind);
            indices.push_back(vertex_count * 3 + 0);
        }
        indices.push_back(vertex_count * 3 + ind);
        indices.push_back(vertex_count * 3 + ind - 1);
    }

#pragma endregion

    // Creating vertices for a z-Axis circle on the top area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f, height / 2 + cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(vertex_count * 4 + ind);
            indices.push_back(vertex_count * 4 + 0);
        }
        indices.push_back(vertex_count * 4 + ind);
        indices.push_back(vertex_count * 4 + ind - 1);

    }
    // Creating vertices for a z-Axis circle on the bottom area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f , -height / 2 + cosf( ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
        vertices.push_back(vert);
        if (ind == 0)
            continue;
        if (ind == vertex_count - 1)
        {
            indices.push_back(vertex_count * 5 + ind);
            indices.push_back(vertex_count * 5 + 0);
        }
        indices.push_back(vertex_count * 5 + ind);
        indices.push_back(vertex_count * 5 + ind - 1);

    }


    Initialize();

}

/*------------------------------------------DYNAMIC_CAPSULE UpdateVertices()------------------------------------------------------*/

void DYNAMIC_CAPSULE::UpdateVertices(float rad, float height, XMMATRIX* target)
{
    radius = rad;
    this->height = height;
    vertices.clear();
    float angle_per_vertex{ 360.0f / (float)vertex_count };
    // Updating vertices for the horizontol circle on the top area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {cosf(ToRadians(angle_per_vertex) * ind) * radius , height / 2  , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
        vertices.push_back(vert);
    }

    // Updating vertices for the horizontol circle on the bottom area
     for (int ind = 0; ind < vertex_count; ++ind)
     {
         VERTEX vert = { {cosf(ToRadians(angle_per_vertex) * ind) * radius , -height / 2 , sinf(ToRadians(angle_per_vertex) * ind) * radius } };
         vertices.push_back(vert);
     }
 
 
    // Updating a vertical circle on the top area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, height / 2 + sinf(ToRadians(angle_per_vertex) * ind) * radius , 0.0f} };
        vertices.push_back(vert);
    }

    // Updating vertices for the vertical circle on the top area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { { cosf(ToRadians(angle_per_vertex) * ind) * radius, -height / 2 + sinf(ToRadians(angle_per_vertex) * ind) * radius , 0.0f} };
        vertices.push_back(vert);
    }
    // Updating vertices for a z-Axis circle on the top area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f, height / 2 + cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
        vertices.push_back(vert);

    }
    // Updating vertices for a z-Axis circle on the bottom area
    for (int ind = 0; ind < vertex_count; ++ind)
    {
        VERTEX vert = { {0.0f , -height / 2 + cosf(ToRadians(angle_per_vertex) * ind) * radius, sinf(ToRadians(angle_per_vertex) * ind) * radius} };
        vertices.push_back(vert);

    }

    if(target)
        Execute(*target);

    //for (int ind = 0; ind < vertex_count; ++ind)

    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    D3D11_MAPPED_SUBRESOURCE mrs{};
    dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mrs);
    memcpy(mrs.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
    dc->Unmap(vertexBuffer.Get(), 0);

}

#pragma endregion