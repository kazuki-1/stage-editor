#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <WICTextureLoader.h>
#include <wrl.h>
#include <string>
#include <memory>
#include <assert.h>
#include "DirectX11.h"
using namespace Microsoft::WRL;
class Texture
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>SRV;
    Microsoft::WRL::ComPtr<ID3D11Resource>TextureResource;
public:
    /// <summary>
    /// Creates a texture based on the path
    /// </summary>
    /// <param name="tex_path"></param>
    /// <param name="dv"></param>
    Texture(const wchar_t* tex_path, ID3D11Device* dv);
    /// <summary>
    /// Creates a texture based on the path
    /// </summary>
    /// <param name="tex_path"></param>
    /// <param name="dv"></param>
    Texture(std::wstring tex_path, ID3D11Device* dv);
    /// <summary>
    /// Creates an empty texture
    /// </summary>
    /// <param name="dv"></param>
    Texture(ID3D11Device* dv);
    /// <summary>
    /// Creates a Empty texture with a colour according to the colour code
    /// </summary>
    /// <param name="colour_code"></param>
    Texture(UINT colour_code);
    /// <summary>
    /// Loads the texture based on the path
    /// </summary>
    /// <param name="tex_path"></param>
    /// <param name="dv"></param>
    /// <returns></returns>
    HRESULT Load(const wchar_t* tex_path, ID3D11Device* dv);
    /// <summary>
    /// Generate an empty texture
    /// </summary>
    /// <param name="dv"></param>
    /// <param name="rgb_value"></param>
    /// <param name="dimensions"></param>
    /// <returns></returns>
    HRESULT GenerateEmptyTexture(ID3D11Device* dv, DWORD rgb_value, UINT dimensions);
    /// <summary>
    /// Query the texture desc
    /// </summary>
    /// <param name="t2d"></param>
    void QueryTextureDesc(D3D11_TEXTURE2D_DESC* t2d);
    /// <summary>
    /// Performs PSSetShaderResources
    /// </summary>
    /// <param name="dc"></param>
    void SetTexture(ID3D11DeviceContext* dc);
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetSRV();

};