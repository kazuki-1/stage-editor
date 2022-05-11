#pragma once
#include "Singleton.h"
#include <wrl.h>
#include <d3d11.h>
#include <map>
#include <memory>
using namespace Microsoft::WRL;

enum class DepthStencilStateTypes
{
	Default, 
	Skybox, 
	End
};

class DepthStencilState
{
	ComPtr<ID3D11DepthStencilState>depthStencilState;
public:
	DepthStencilState() {};
	DepthStencilState(D3D11_DEPTH_STENCIL_DESC dsd);

	ID3D11DepthStencilState* Get() { return depthStencilState.Get(); }
};



class DepthStencilStateManager : public Singleton<DepthStencilStateManager>
{
	std::map<DepthStencilStateTypes, std::unique_ptr<DepthStencilState>>states;
	void Insert(DepthStencilStateTypes type, D3D11_DEPTH_STENCIL_DESC dsd);

public:
	HRESULT Initialize();
	void Set(DepthStencilStateTypes type);

	ID3D11DepthStencilState* Retrieve(DepthStencilStateTypes type);

};
