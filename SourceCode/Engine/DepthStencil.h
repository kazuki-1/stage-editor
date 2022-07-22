#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <assert.h>
#include "../DirectX11.h"
using namespace Microsoft::WRL;


class DepthStencil
{
    ComPtr<ID3D11ShaderResourceView>shaderResourceView{};
    ComPtr<ID3D11DepthStencilView>depthStencilView{};
    D3D11_TEXTURE2D_DESC texture_desc{};

public:
    DepthStencil() {}
	DepthStencil(UINT width, UINT height)
	{
        ID3D11Device* dv = DirectX11::Instance()->Device();
        texture_desc = {};
        texture_desc.Width = width;
        texture_desc.Height = height;
        texture_desc.MipLevels = texture_desc.ArraySize = 1;
        texture_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags = texture_desc.MiscFlags = 0;
        ComPtr<ID3D11Texture2D>texture;
        assert(dv->CreateTexture2D(&texture_desc, 0, texture.GetAddressOf()) == S_OK);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MostDetailedMip = 0;
        srvd.Texture2D.MipLevels = 1;
        assert(dv->CreateShaderResourceView(texture.Get(), &srvd, shaderResourceView.ReleaseAndGetAddressOf()) == S_OK);

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
        dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Flags = 0;
        dsvd.Texture2D.MipSlice = 0;
        assert(dv->CreateDepthStencilView(texture.Get(), &dsvd, depthStencilView.GetAddressOf()) == S_OK);

	}

    // Call this if created using default constructor
	HRESULT Initialize(UINT width, UINT height)
	{
        ID3D11Device* dv = DirectX11::Instance()->Device();
        texture_desc = {};
        texture_desc.Width = width;
        texture_desc.Height = height;
        texture_desc.MipLevels = texture_desc.ArraySize = 1;
        texture_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
        texture_desc.SampleDesc.Count = 1;
        texture_desc.SampleDesc.Quality = 0;
        texture_desc.Usage = D3D11_USAGE_DEFAULT;
        texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        texture_desc.CPUAccessFlags = texture_desc.MiscFlags = 0;
        ComPtr<ID3D11Texture2D>texture;
        assert(dv->CreateTexture2D(&texture_desc, 0, texture.GetAddressOf()) == S_OK);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvd.Texture2D.MostDetailedMip = 0;
        srvd.Texture2D.MipLevels = 1;
        assert(dv->CreateShaderResourceView(texture.Get(), &srvd, shaderResourceView.ReleaseAndGetAddressOf()) == S_OK);

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
        dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        dsvd.Flags = 0;
        dsvd.Texture2D.MipSlice = 0;
        assert(dv->CreateDepthStencilView(texture.Get(), &dsvd, depthStencilView.GetAddressOf()) == S_OK);
        return S_OK;
	}

	ComPtr<ID3D11ShaderResourceView>GetSRV() { return shaderResourceView; }
	ComPtr<ID3D11DepthStencilView>GetDSV() { return depthStencilView; }
	D3D11_TEXTURE2D_DESC GetTextureDesc() { return texture_desc; }
	inline int GetWidth() { return texture_desc.Width; }
	inline int GetHeight() { return texture_desc.Height; }




};