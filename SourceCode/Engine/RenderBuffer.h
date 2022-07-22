#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "DirectX11.h"
using namespace Microsoft::WRL;

// Used for Post processing rendering
// Such as Shadows
class RenderBuffer
{
	ComPtr<ID3D11ShaderResourceView>shaderResourceView{};
	ComPtr<ID3D11RenderTargetView>renderTargetView{};
	D3D11_TEXTURE2D_DESC texture_desc{};
public:
	RenderBuffer();
	RenderBuffer(UINT width, UINT height, DXGI_FORMAT format)
	{
		ID3D11Device* dv{ DirectX11::Instance()->Device() };

		ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = texture_desc.ArraySize = 1;
		texture_desc.CPUAccessFlags = texture_desc.MiscFlags = 0;
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		ComPtr<ID3D11Texture2D>texture{};
		assert(dv->CreateTexture2D(&texture_desc, 0, texture.GetAddressOf()) == S_OK);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = texture_desc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		assert(dv->CreateShaderResourceView(texture.Get(), &srvd, shaderResourceView.GetAddressOf()) == S_OK);

		D3D11_RENDER_TARGET_VIEW_DESC rtvd{};
		rtvd.Format = texture_desc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		assert(dv->CreateRenderTargetView(texture.Get(), &rtvd, renderTargetView.GetAddressOf()) == S_OK);


	}

	/// <summary>
	/// Call this after using the default constructor
	/// </summary>
	/// <returns></returns>
	HRESULT Initialize(UINT width, UINT height, DXGI_FORMAT format)
	{
		ID3D11Device* dv{ DirectX11::Instance()->Device() };

		ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = texture_desc.ArraySize = 1;
		texture_desc.CPUAccessFlags = texture_desc.MiscFlags = 0;
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		ComPtr<ID3D11Texture2D>texture{};
		assert(dv->CreateTexture2D(&texture_desc, 0, texture.GetAddressOf()) == S_OK);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
		srvd.Format = texture_desc.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1;
		assert(dv->CreateShaderResourceView(texture.Get(), &srvd, shaderResourceView.GetAddressOf()) == S_OK);

		D3D11_RENDER_TARGET_VIEW_DESC rtvd{};
		rtvd.Format = texture_desc.Format;
		rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		assert(dv->CreateRenderTargetView(texture.Get(), &rtvd, renderTargetView.GetAddressOf()) == S_OK);
		return S_OK;
	}


	ComPtr<ID3D11ShaderResourceView>GetSRV() { return shaderResourceView; }
	ComPtr<ID3D11RenderTargetView>GetRTV() { return renderTargetView; }
	D3D11_TEXTURE2D_DESC GetTextureDesc() { return texture_desc; }
	inline int GetWidth() { return texture_desc.Width; }
	inline int GetHeight() { return texture_desc.Height; }

};
