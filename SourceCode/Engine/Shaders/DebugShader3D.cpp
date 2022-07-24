#include "DebugShader3D.h"
#include "../MODEL_RESOURCE.h"
#include "../MODEL.h"
#include "../BlendMode.h"
#include "../RASTERIZER.h"
#include "../DEBUG_PRIMITIVE.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DebugShader3D Class----------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DebugShader3D Initialize()---------------------------------------*/

HRESULT DebugShader3D::Initialize()
{
    ID3D11Device* dv = DirectX11::Instance()->Device();

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},

    };
    int ied_size{ ARRAYSIZE(ied) };


    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    hr = D3DCompileFromFile(L"./Shaders/DebugShader.fx", NULL, NULL, "VS_MAIN", "vs_5_0", shaderFlags, NULL, &vs, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = D3DCompileFromFile(L"./Shaders/DebugShader.fx", NULL, NULL, "PS_MAIN", "ps_5_0", shaderFlags, NULL, &ps, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    dv->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
    if (FAILED(hr))
    {
        assert(!"Failed to create Vertex Shader");
    }
    dv->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
    if (FAILED(hr))
    {
        assert(!"Failed to create Pixel Shader");
    }
    hr = dv->CreateInputLayout(ied, ied_size, vs->GetBufferPointer(), vs->GetBufferSize(), inputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        assert(!"Failed to create Input Layout");
    }
    vs->Release();
    ps->Release();

    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(CBuffer_Mesh);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    return dv->CreateBuffer(&cbd, 0, meshConstantBuffer.GetAddressOf());


}

/*---------------------------------------------------DebugShader3D UpdateConstantBuffers()---------------------------------------*/

void DebugShader3D::UpdateConstantBuffers(OBJECT* parent)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    MODEL* model = ((MODEL*)parent);
    MODEL_RESOURCES* resource = model->Resource().get();


    XMMATRIX world = XMLoadFloat4x4(&resource->Axises.AxisCoords) * model->TransformMatrix() ;



    CBuffer_Mesh data;
    XMStoreFloat4x4(&data.world, world);
    data.colour = model->GetColours();
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);

}

/*---------------------------------------------------DebugShader3D SetConstantBuffers()---------------------------------------*/

void DebugShader3D::SetConstantBuffers()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    dc->VSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());

}

/*---------------------------------------------------DebugShader3D Render()---------------------------------------*/

void DebugShader3D::Render()
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    SetShaders();
    SetConstantBuffers();

    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    RasterizerManager::Instance()->Set(RasterizerTypes::Base_3D);
    BlendStateManager::Instance()->Set(BlendModes::Alpha);
    UINT stride{ sizeof(MODEL_RESOURCES::VERTEX) }, offset{ 0 };

    int ind{};      // Checks if object is still a living pointer and erases if dead
    for (auto& object : objects)
    {
        if (!object->GetRenderState())
            continue;
        if (!object)
        {
            objects.erase(objects.begin() + ind);
            continue;
        }

        MODEL* model = (MODEL*)object;
        MODEL_RESOURCES* resource = model->Resource().get();
        XMFLOAT4X4 object_world_transform = model->Transform();

        for (auto& mesh : resource->Meshes)
        {
            dc->IASetVertexBuffers(0, 1, mesh.dxVertexBuffer.GetAddressOf(), &stride, &offset);
            //XMMATRIX f_World{
            //XMLoadFloat4x4(&mesh.BaseTransform) * 
            //XMLoadFloat4x4(&resource->Axises.AxisCoords) *
            //XMLoadFloat4x4(&object_world_transform) };

            //XMStoreFloat4x4(&resource->data.world, f_World);

            




            for (auto& subset : mesh.Subsets)
            {

                dc->IASetIndexBuffer(subset.subsetIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
                UpdateConstantBuffers(object);
                dc->DrawIndexed((UINT)subset.indices.size(), 0, 0);
            }

        }
        resource->data.world = object_world_transform;
        ++ind;
    }
}