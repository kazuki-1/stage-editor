#include "Shader2D.h"
#include "../Sprite.h"
#include "../RASTERIZER.h"
#include "../BlendMode.h"

/*--------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Shader2D Class--------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Shader2D Initialize()-------------------------------------*/

HRESULT Shader2D::Initialize()
{
    ID3D11Device* dv = DirectX11::Instance()->Device();

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOUR",      0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV",          0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


    };
    int ied_size{ ARRAYSIZE(ied) };


    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    hr = D3DCompileFromFile(L"./Shaders/Shader_2D.fx", NULL, NULL, "VS_MAIN", "vs_5_0", shaderFlags, NULL, &vs, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = D3DCompileFromFile(L"./Shaders/Shader_2D.fx", NULL, NULL, "PS_MAIN", "ps_5_0", shaderFlags, NULL, &ps, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = dv->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
    if (FAILED(hr))
    {
        assert(!"Failed to create Vertex Shader");
    }
    hr = dv->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
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

    return S_OK;
}

/*----------------------------------------------------------Shader2D Render()-------------------------------------*/

void Shader2D::Render()
{
    SetShaders();
    SetConstantBuffers();
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();
    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    for (auto& object : objects)
    {
        if (!object->GetRenderState())
            continue;


        SPRITE* sprite = (SPRITE*)object;
        sprite->Texture()->SetTexture(dc);



        RasterizerManager::Instance()->Set(RasterizerTypes::Base_2D);
        BlendStateManager::Instance()->Set(BlendModes::Alpha);


        UINT stride{ sizeof(SPRITE::VERTEX) }, offset{};


        dc->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
        dc->IASetIndexBuffer(sprite->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, offset);
        dc->DrawIndexed(6, 0, 0);
        object->DisableRendering();
    }
}