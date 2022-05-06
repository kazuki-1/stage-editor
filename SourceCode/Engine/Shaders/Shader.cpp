#include "Shader.h"
#include "../SamplerStateManager.h"
#include "PhongShader.h"


/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Class--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Constructor--------------------------------------------------------------------*/

Shader::Shader(std::wstring shader_path, ID3D11Device* dv, D3D11_INPUT_ELEMENT_DESC* ied, UINT ied_count)
{
    HRESULT hr{ S_OK };
    ID3D10Blob* vs{}, * ps{}, * error{};
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    hr = D3DCompileFromFile(shader_path.c_str(), NULL, NULL, "VS_MAIN", "vs_5_0", shaderFlags, NULL, &vs, &error);
    if (FAILED(hr))
    {
        std::string er = (const char*)error->GetBufferPointer();
        assert(!er.c_str());
    }
    hr = D3DCompileFromFile(shader_path.c_str(), NULL, NULL, "PS_MAIN", "ps_5_0", shaderFlags, NULL, &ps, &error);
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
    hr = dv->CreateInputLayout(ied, ied_count, vs->GetBufferPointer(), vs->GetBufferSize(), inputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        assert(!"Failed to create Input Layout");
    }
    vs->Release();
    ps->Release();




}

/*---------------------------------------------Shader SetShaders--------------------------------------------------------------------*/

void Shader::SetShaders(ID3D11DeviceContext* dc, OBJECT* parent)
{
    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(pixelShader.Get(), 0, 0);
    dc->IASetInputLayout(inputLayout.Get());
    SamplerStateManager::Instance()->Set(SamplerStateType::Default);

}


void ShaderManager::Set(std::wstring shader_name)
{
    //shaders.find(shader_name)->second->SetShaders(DirectX11::Instance()->DeviceContext(), );
    
}

HRESULT ShaderManager::Insert(ShaderTypes type, std::shared_ptr<Shader>shader)
{
    return S_OK;
    // std::shared_ptr<Shader>s = std::make_shared<Shader>(shader_path, DirectX11::Instance()->Device(), ied, ied_count);
    // std::filesystem::path path(shader_path);
    // path = path.filename();
    // std::wstring name = path.wstring();
    // if (!s)
    //     assert(!"Failed to create Shader");
    // shaders.insert({ name, s });
    // return s ? S_OK : E_FAIL;
}
HRESULT ShaderManager::Initialize()
{
    //D3D11_INPUT_ELEMENT_DESC ied[] =
    //{
    //    {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"UV",          0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"B_WEIGHTS",   0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    //    {"B_INDICES",   0, DXGI_FORMAT_R32G32B32A32_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}


    //};
    //int ied_Size{ ARRAYSIZE(ied) };

    D3D11_INPUT_ELEMENT_DESC ied_2d[] =
    {
        {"SV_POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR",          0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV",             0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    int ied_2d_size{ ARRAYSIZE(ied_2d) };


    D3D11_INPUT_ELEMENT_DESC ied_p[] =
    {
        {"POSITION",    0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV",          0, DXGI_FORMAT_R32G32_FLOAT,        0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"WEIGHTS",     0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"BONES",     0, DXGI_FORMAT_R32G32B32A32_UINT,   0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},


    };
    int ied_p_size{ ARRAYSIZE(ied_p) };
    D3D11_INPUT_ELEMENT_DESC ied_d[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    int ied_d_size{ ARRAYSIZE(ied_d) };


    ID3D11Device* dv{ DirectX11::Instance()->Device() };


    //Insert(L"Shaders/Shader_3d.fx", ied, ied_Size);
    shaders.emplace(ShaderTypes::Shader_2D, std::make_unique<Shader>(L"./Shaders/Shader_2D.fx", dv, ied_2d, ied_2d_size));
    shaders.emplace(ShaderTypes::PhongShader, std::make_unique<PhongShader>());
    shaders.emplace(ShaderTypes::Base_3D, std::make_unique<Shader>(L"Shaders/Base3DShader.fx", dv, ied_p, ied_p_size));
    shaders.emplace(ShaderTypes::Debug_3D,std::make_unique<Shader>(L"Shaders/DebugShader.fx", dv, ied_d, ied_d_size));
    shaders.emplace(ShaderTypes::Outline, std::make_unique<Shader>(L"./Shaders/OutlineShader.fx", dv, ied_p, ied_p_size));
    return S_OK;
}
void ShaderManager::Clear()
{
    shaders.clear();
}
std::shared_ptr<Shader>ShaderManager::Retrieve(ShaderTypes type)
{
    return shaders.find(type)->second;
}
