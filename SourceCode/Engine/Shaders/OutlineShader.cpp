#include "OutlineShader.h"
#include "../MODEL_RESOURCE.h"
#include "../DepthStencilStateManager.h"
#include "../SamplerStateManager.h"

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------OutlineShader Class-----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------OutlineShader Initialize()----------------------------------------------------*/

HRESULT OutlineShader::Initialize()
{
    ID3D11Device* dv = DirectX11::Instance()->Device();

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV",          0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"WEIGHTS",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONES",     0, DXGI_FORMAT_R32G32B32A32_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


    };
    int ied_size{ ARRAYSIZE(ied) };


    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    hr = D3DCompileFromFile(L"./Shaders/OutlineShader.fx", NULL, NULL, "VS_MAIN", "vs_5_0", shaderFlags, NULL, &vs, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = D3DCompileFromFile(L"./Shaders/OutlineShader.fx", NULL, NULL, "PS_MAIN", "ps_5_0", shaderFlags, NULL, &ps, &error);
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

    assert (dv->CreateBuffer(&cbd, 0, meshConstantBuffer.GetAddressOf()) == S_OK);

    cbd.ByteWidth = sizeof(CBuffer_Outline);
    return dv->CreateBuffer(&cbd, 0, outlineConstantBuffer.GetAddressOf());

}

void OutlineShader::SetShaders()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(pixelShader.Get(), 0, 0);
    dc->IASetInputLayout(inputLayout.Get());
    DepthStencilStateManager::Instance()->Set(DepthStencilStateTypes::Default);
    SamplerStateManager::Instance()->Set(SamplerStateType::Default);

}


/*----------------------------------------------------OutlineShader UpdateConstantBuffers()-----------------------------------------*/

void OutlineShader::UpdateConstantBuffers(OBJECT* parent)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    MODEL_RESOURCES* model = (MODEL_RESOURCES*)parent;
    MODEL_RESOURCES::MESH_CONSTANT_BUFFER data = model->data;
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);

    CBuffer_Outline outline_data{};
    outline_data.outline_size = 1.01f;
    outline_data.outline_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
    dc->UpdateSubresource(outlineConstantBuffer.Get(), 0, 0, &outline_data, 0, 0);

}

/*--------------------------------------------------------------OutlineShader SetConstantBuffers()---------------------------------*/

void OutlineShader::SetConstantBuffers()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    ID3D11Buffer* buffers[] = { meshConstantBuffer.Get(), outlineConstantBuffer.Get() };
    dc->VSSetConstantBuffers(1, 2, buffers);
    dc->PSSetConstantBuffers(1, 2, buffers);

}