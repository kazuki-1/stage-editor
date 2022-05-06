
#pragma comment(lib, "DirectXTK.lib")
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <map>
using namespace Microsoft::WRL;

enum class BlendModes
{
    Alpha,
    Lighten, 
    Darken, 
    Add,
    Subtract, 
    Multiply, 
    Replace, 
    Screen
};

class BlendState
{
    ComPtr<ID3D11BlendState>blendState;
public:
    BlendState(ID3D11Device* dv, D3D11_BLEND_DESC dbd);
    ID3D11BlendState* Get() { return blendState.Get(); }
};





class BlendStateManager
{
    BlendStateManager() {};
    std::map<BlendModes, std::shared_ptr<BlendState>>states;
public:

    static BlendStateManager* Instance()
    {
        static BlendStateManager bm;
        return &bm;
    }
    HRESULT Initialize();

    void Insert(BlendModes mode, D3D11_BLEND_DESC dbd);
    void Set(BlendModes mode);
};