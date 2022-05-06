#pragma once
#include "DirectX11.h"

enum class SamplerStateType
{
	Default, 
	Skybox, 

	End
};

class SamplerState
{
public:
	ComPtr<ID3D11SamplerState>sampler;
	SamplerState(D3D11_SAMPLER_DESC dsd);
	void Set(ID3D11DeviceContext* dc)
	{
		dc->PSSetSamplers(0, 1, sampler.GetAddressOf());
	}
};


class SamplerStateManager : public Singleton<SamplerStateManager>
{
	std::map<SamplerStateType, std::shared_ptr<SamplerState>>samplers;

public:
	void Initialize();
	void Set(SamplerStateType type);
	void Insert(SamplerStateType type, D3D11_SAMPLER_DESC dsd);
	std::shared_ptr<SamplerState>Retrieve(SamplerStateType type);
};