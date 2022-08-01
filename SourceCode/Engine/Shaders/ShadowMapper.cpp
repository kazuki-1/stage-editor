#include "ShadowMapper.h"
#include "../Graphics.h"
#include "../MODEL_RESOURCE.h"
#include "../DepthStencilStateManager.h"
#include "../BlendMode.h"
#include "../RASTERIZER.h"
#include "../MODEL.h"
#define ShadowMapSize 2048U
#define ShadowMapDimensions 500.0f
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------ShadowMapper Class----------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------ShadowMapper Initialize()---------------------------------------------------*/

HRESULT ShadowMapper::Initialize()
{
    ID3D11Device* dv{ DirectX11::Instance()->Device() };



    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif
    hr = D3DCompileFromFile(L"Shaders/ShadowMapper.fx", 0, 0, "VS_MAIN", "vs_5_0", shaderFlags, 0, &vs, &error);
    if (hr != S_OK)
    {
        std::string test = (const char*)error->GetBufferPointer();
        assert(test.c_str());
    }

    assert(dv->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), 0, vertexShader.GetAddressOf()) == S_OK);


    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    assert(dv->CreateInputLayout(ied, ARRAYSIZE(ied), vs->GetBufferPointer(), vs->GetBufferSize(), inputLayout.GetAddressOf()) == S_OK);


    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(CBuffer_Scene);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    assert(dv->CreateBuffer(&cbd, 0, sceneConstantBuffer.GetAddressOf()) == S_OK);

    cbd.ByteWidth = sizeof(MODEL_RESOURCES::MESH_CONSTANT_BUFFER);
    assert(dv->CreateBuffer(&cbd, 0, meshConstantBuffer.GetAddressOf()) == S_OK);

    depthStencil = std::make_unique<DepthStencil>(ShadowMapSize, ShadowMapSize);
    renderBuffer = std::make_unique<RenderBuffer>(DirectX11::Instance()->GetScreenDimension().x, DirectX11::Instance()->GetScreenDimension().y, DXGI_FORMAT_R8G8B8A8_UNORM);

    // Create a temporary viewport that is based ont he depthStencil
    viewport.Width = (float)depthStencil->GetWidth();
    viewport.Height = (float)depthStencil->GetHeight();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;


    return S_OK;
}

/*----------------------------------------------------ShadowMapper SetShaders()--------------------------------------------------------------*/

void ShadowMapper::SetShaders()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(0, 0, 0);
    dc->IASetInputLayout(inputLayout.Get());

    
    BlendStateManager::Instance()->Set(BlendModes::Alpha);
    DepthStencilStateManager::Instance()->Set(DepthStencilStateTypes::ShadowMapper);
    RasterizerManager::Instance()->Set(RasterizerTypes::ShadowMapper);


}

/*----------------------------------------------------ShadowMapper UpdateConstantBuffers()---------------------------------------------------*/

void ShadowMapper::UpdateConstantBuffers(OBJECT* object)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    MODEL* model = (MODEL*)object;
    MODEL_RESOURCES* resource = model->Resource().get();


    dc->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &scene_data, 0, 0);
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &resource->data, 0, 0);

}

/*----------------------------------------------------ShadowMapper SetConstantBuffers()---------------------------------------------------*/

void ShadowMapper::SetConstantBuffers()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    ID3D11Buffer* buffers[] = { sceneConstantBuffer.Get(), meshConstantBuffer.Get() };
    dc->VSSetConstantBuffers(1, 2, buffers);
}

/*----------------------------------------------------ShadowMapper CleanupShaders()---------------------------------------------------*/

void ShadowMapper::CleanupShaders()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();
    dc->PSSetShader(0, 0, 0);
    dc->VSSetShader(0, 0, 0);
    dc->PSSetConstantBuffers(1, 0, 0);
    dc->VSSetConstantBuffers(1, 0, 0);

}


/*----------------------------------------------------ShadowMapper Render()---------------------------------------------------*/

void ShadowMapper::Render()
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };





    ID3D11RenderTargetView* rtv{  }, * former_rtv{ DirectX11::Instance()->GetRenderTargetView().Get() };
    ID3D11DepthStencilView* dsv{ depthStencil->GetDSV().Get() }, *former_dsv{ DirectX11::Instance()->GetDepthStencilView().Get()};

    //dc->OMGetRenderTargets(1, &former_rtv, &former_dsv);
    D3D11_VIEWPORT former_viewport{};

    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(0, &rtv, dsv);

    // Sets the temporary viewport
    dc->RSSetViewports(1, &viewport);
    
    
    
    
    

    // Create a scene constant buffer with a mode
    CBuffer_Scene data{};
    Vector4 light_pos = Graphics::Instance()->scene_data.directional.direction;
    light_pos.Load(XMVectorScale(light_pos.XMV(), -250.0f));

    XMMATRIX lookAt{ XMMatrixLookAtLH(light_pos.XMV(), {0, 0, 0, 0}, {0, 1, 0, 0}) };
    XMMATRIX projection{ XMMatrixOrthographicLH(ShadowMapDimensions, ShadowMapDimensions, 0.1f, 1000.0f) };
    XMStoreFloat4x4(&scene_data.view_projection, lookAt * projection);
    
    UINT stride{ sizeof(MODEL_RESOURCES::VERTEX) }, offset{};
    SetShaders();

    for (auto& object : objects)
    {
        MODEL* m = (MODEL*)object;
        UpdateConstantBuffers(m);
        SetConstantBuffers();
        std::shared_ptr<MODEL_RESOURCES> resource = m->Resource();
        for (auto& mesh : resource->Meshes)
        {
            dc->IASetVertexBuffers(0, 1, mesh.dxVertexBuffer.GetAddressOf(), &stride, &offset);
            dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



            for (auto& subset : mesh.Subsets)
            {

                dc->IASetIndexBuffer(subset.subsetIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
                dc->DrawIndexed((UINT)subset.indices.size(), 0, 0);
            }
        }
    }
    
    // Reset to default viewport
    //dc->RSSetViewports(1, &former_viewport);
    //dc->OMSetRenderTargets(0, &former_rtv, former_dsv);
    CleanupShaders();
}