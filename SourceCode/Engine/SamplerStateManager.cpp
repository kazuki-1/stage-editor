#include "SamplerStateManager.h"


/*----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------SamplerState Class-------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------SamplerState Constructor-------------------------------------------------------*/

SamplerState::SamplerState(D3D11_SAMPLER_DESC dsd)
{
	assert(DirectX11::Instance()->Device()->CreateSamplerState(&dsd, sampler.GetAddressOf()) == S_OK);
}

/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------SamplerStateManager Class-------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------SamplerStateManager Initialize()------------------------------------------------------*/

void SamplerStateManager::Initialize()
{

    D3D11_SAMPLER_DESC dsd{};
    dsd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    dsd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    dsd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    dsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    dsd.MipLODBias = 0.0f;
    dsd.MaxAnisotropy = 1;
    dsd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    dsd.MinLOD = 0;
    dsd.MaxLOD = D3D11_FLOAT32_MAX;
    dsd.BorderColor[0] = 0;
    dsd.BorderColor[1] = 0;
    dsd.BorderColor[2] = 0;
    dsd.BorderColor[3] = 0;

    Insert(SamplerStateType::Default, dsd);

    dsd.MipLODBias = 0.0f;
    dsd.MaxAnisotropy = 1;
    dsd.ComparisonFunc = D3D11_COMPARISON_NEVER;
    dsd.MinLOD = -FLT_MAX;
    dsd.MaxLOD = FLT_MAX;
    dsd.BorderColor[0] = 1.0f;
    dsd.BorderColor[1] = 1.0f;
    dsd.BorderColor[2] = 1.0f;
    dsd.BorderColor[3] = 1.0f;
    dsd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    dsd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    dsd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    dsd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    Insert(SamplerStateType::Skybox, dsd);


}

/*---------------------------------------------SamplerStateManager Set()------------------------------------------------------*/

void SamplerStateManager::Set(SamplerStateType type)
{
    samplers.find(type)->second->Set(DirectX11::Instance()->DeviceContext());
}

/*---------------------------------------------SamplerStateManager Insert()------------------------------------------------------*/

void SamplerStateManager::Insert(SamplerStateType type, D3D11_SAMPLER_DESC dsd)
{
	samplers.emplace(type, std::make_shared<SamplerState>(dsd));
}

/*---------------------------------------------SamplerStateManager Retrieve()------------------------------------------------------*/

std::shared_ptr<SamplerState>SamplerStateManager::Retrieve(SamplerStateType type)
{
	return samplers.find(type)->second;
}
