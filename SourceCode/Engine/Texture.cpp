#include "Texture.h"

/*-------------------------------------------------------------------------------------*/
/*-----------------------------------------Texture Class-------------------------------*/
/*-------------------------------------------------------------------------------------*/
/*-----------------------------------------Texture Constructors------------------------*/
/// <summary>
/// Creates a texture based on the path
/// </summary>
/// <param name="tex_path"></param>
/// <param name="dv"></param>
Texture::Texture(const wchar_t* tex_path, ID3D11Device* dv)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(dv, tex_path, TextureResource.GetAddressOf(), SRV.GetAddressOf());
    if (FAILED(hr))
        assert(!"Couldn't find texture");

}

/// <summary>
/// Creates a texture based on the path
/// </summary>
/// <param name="tex_path"></param>
/// <param name="dv"></param>
Texture::Texture(std::wstring tex_path, ID3D11Device* dv)
{
    HRESULT hr = DirectX::CreateWICTextureFromFile(dv, tex_path.c_str(), TextureResource.GetAddressOf(), SRV.GetAddressOf());
    if (FAILED(hr))
        GenerateEmptyTexture(dv, 0xFFFFFFFF, 16);
}

/// <summary>
/// Creates an empty texture
/// </summary>
/// <param name="dv"></param>
Texture::Texture(ID3D11Device* dv)
{
    HRESULT hr{ GenerateEmptyTexture(dv, 0xFFFFFFFF, 16) };
    if (FAILED(hr))
        assert(!"Failed to create Empty Texture");
}

/// <summary>
/// Creates a Empty texture with a colour according to the colour code
/// </summary>
/// <param name="colour_code"></param>
Texture::Texture(UINT colour_code)
{
    HRESULT hr{ GenerateEmptyTexture(DirectX11::Instance()->Device(), colour_code, 16) };
    assert(hr == S_OK);
}

/*--------------------------------------------Texture Load()---------------------------*/
/// <summary>
/// Loads the texture based on the path
/// </summary>
/// <param name="tex_path"></param>
/// <param name="dv"></param>
/// <returns></returns>
HRESULT Texture::Load(const wchar_t* tex_path, ID3D11Device* dv)
{
    return DirectX::CreateWICTextureFromFile(dv, tex_path, nullptr, SRV.GetAddressOf());
}

/*--------------------------------------------Texture GenerateEmptyTexture()-----------*/
/// <summary>
/// Generate an empty texture
/// </summary>
/// <param name="dv"></param>
/// <param name="rgb_value"></param>
/// <param name="dimensions"></param>
/// <returns></returns>
HRESULT Texture::GenerateEmptyTexture(ID3D11Device* dv, DWORD rgb_value, UINT dimensions)
{
    HRESULT hr{ S_OK };
    D3D11_TEXTURE2D_DESC d2d{};
    d2d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    d2d.ArraySize = 1;
    d2d.MipLevels = 1;
    d2d.Usage = D3D11_USAGE_DEFAULT;
    d2d.SampleDesc = { 1, 0 };
    d2d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    d2d.Width = d2d.Height = dimensions;
    size_t texels{ (unsigned long long)(dimensions * dimensions) };
    std::unique_ptr<DWORD[]>sysMem{ std::make_unique<DWORD[]>(texels) };
    for (size_t alpha = 0; alpha < texels; ++alpha)
        sysMem[alpha] = rgb_value;

    D3D11_SUBRESOURCE_DATA dsd{};
    dsd.pSysMem = sysMem.get();
    dsd.SysMemPitch = sizeof(DWORD) * dimensions;

    ComPtr<ID3D11Texture2D>dt2d;
    hr = dv->CreateTexture2D(&d2d, &dsd, dt2d.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create texture");

    D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd{};
    dsrvd.Format = d2d.Format;
    dsrvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    dsrvd.Texture2D.MipLevels = 1;
    hr = dv->CreateShaderResourceView(dt2d.Get(), &dsrvd, SRV.GetAddressOf());
    assert(hr == S_OK);
    return hr;
}

/*--------------------------------------------Texture QueryTextureDesc()---------------*/
/// <summary>
/// Query the texture desc
/// </summary>
/// <param name="t2d"></param>
void Texture::QueryTextureDesc(D3D11_TEXTURE2D_DESC* t2d)
{
    ID3D11Texture2D* t;
    TextureResource->QueryInterface<ID3D11Texture2D>(&t);
    t->GetDesc(t2d);
    t->Release();
}

/*--------------------------------------------Texture SetTexture()---------------------*/
/// <summary>
/// Performs PSSetShaderResources
/// </summary>
/// <param name="dc"></param>
void Texture::SetTexture(ID3D11DeviceContext* dc)
{
    dc->PSSetShaderResources(0, 1, SRV.GetAddressOf());
}

/*--------------------------------------------Texture GetSRV()-------------------------*/

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetSRV()
{
    return SRV;
}
