#include "SkyboxShader.h"
#include "../SamplerStateManager.h"
#include "../DepthStencilStateManager.h"
#include "../Graphics.h"
#include "../Camera.h"
#include "../MODEL_RESOURCE.h"
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SkyboxShader Class----------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SkyboxShader Initialize()---------------------------------------------------------*/

HRESULT SkyboxShader::Initialize()
{


    ID3D11Device* dv = DirectX11::Instance()->Device();
    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif


    // Shader creation
    hr = D3DCompileFromFile(L"./Shaders/SkyboxShader.fx" , NULL, NULL, "VS_MAIN", "vs_5_0", shaderFlags, NULL, &vs, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = D3DCompileFromFile(L"./Shaders/SkyboxShader.fx" , NULL, NULL, "PS_MAIN", "ps_5_0", shaderFlags, NULL, &ps, &error);
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

    // Input Layout creation
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOUR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    size_t ied_size = ARRAYSIZE(ied);
    hr = dv->CreateInputLayout(ied, ied_size, vs->GetBufferPointer(), vs->GetBufferSize(), inputLayout.GetAddressOf());
    assert(hr == S_OK, "Failed to create input layout");

    // Blob release
    vs->Release();
    ps->Release();


    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(CBuffer_Skybox);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    return dv->CreateBuffer(&cbd, 0, skyboxConstantBuffer.GetAddressOf());



}

/*---------------------------------------SkyboxShader UpdateConstantBuffer()---------------------------------------------------------------*/

void SkyboxShader::UpdateConstantBuffers(ID3D11DeviceContext* dc, OBJECT* parent)
{
    CBuffer_Skybox data{};
    XMMATRIX view = Camera::Instance()->ViewMatrix() ;
    XMMATRIX proj = DirectX11::Instance()->ProjectionMatrix() ;

    data.view_position = Graphics::Instance()->scene_data.camera_position;



    XMStoreFloat4x4(&data.inverse_view, XMMatrixInverse(0, view));
    XMStoreFloat4x4(&data.inverse_projection, XMMatrixInverse(0, proj));
    dc->UpdateSubresource(skyboxConstantBuffer.Get(), 0, 0, &data, 0, 0);


}

/*---------------------------------------SkyboxShader SetShaders()---------------------------------------------------------------*/

void SkyboxShader::SetShaders(ID3D11DeviceContext* dc, OBJECT* parent)
{
    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(pixelShader.Get(), 0, 0);
    dc->IASetInputLayout(inputLayout.Get());
    DepthStencilStateManager::Instance()->Set(DepthStencilStateTypes::Skybox);
    SamplerStateManager::Instance()->Set(SamplerStateType::Skybox);
}


void SkyboxShader::SetConstantBuffers(ID3D11DeviceContext* dc)
{
    dc->VSSetConstantBuffers(1, 1, skyboxConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(1, 1, skyboxConstantBuffer.GetAddressOf());

}