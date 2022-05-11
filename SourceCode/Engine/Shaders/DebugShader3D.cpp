#include "DebugShader3D.h"
#include "../MODEL_RESOURCE.h"



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
    cbd.ByteWidth = sizeof(MODEL_RESOURCES::MESH_CONSTANT_BUFFER);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    return dv->CreateBuffer(&cbd, 0, meshConstantBuffer.GetAddressOf());


}

/*---------------------------------------------------DebugShader3D UpdateConstantBuffers()---------------------------------------*/

void DebugShader3D::UpdateConstantBuffer(ID3D11DeviceContext* dc, OBJECT* parent)
{
    
    MODEL_RESOURCES* model = (MODEL_RESOURCES*)parent;


    MODEL_RESOURCES::MESH_CONSTANT_BUFFER data = model->data;
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);

}

/*---------------------------------------------------DebugShader3D SetConstantBuffers()---------------------------------------*/

void DebugShader3D::SetConstantBuffers(ID3D11DeviceContext* dc)
{
    dc->VSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, meshConstantBuffer.GetAddressOf());

}
