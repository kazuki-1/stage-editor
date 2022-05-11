#include "DepthStencilStateManager.h"
#include "DirectX11.h"
#include <assert.h>

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------DepthStencilState Class-------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------DepthStencilState Initialize()------------------------------*/


DepthStencilState::DepthStencilState(D3D11_DEPTH_STENCIL_DESC dsd)
{
    HRESULT hr = DirectX11::Instance()->Device()->CreateDepthStencilState(&dsd, depthStencilState.GetAddressOf());
    assert(hr == S_OK);
}


/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------DepthStencilStateManager Class-------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------DepthStencilStateManager Insert()------------------------------*/

void DepthStencilStateManager::Insert(DepthStencilStateTypes type, D3D11_DEPTH_STENCIL_DESC dsd)
{
    states.emplace(type, std::make_unique<DepthStencilState>(dsd));
}



/*----------------------------------------------------DepthStencilStateManager Initialize()------------------------------*/

HRESULT DepthStencilStateManager::Initialize()
{
    D3D11_DEPTH_STENCIL_DESC dsd{};
    dsd.DepthEnable = false;
    Insert(DepthStencilStateTypes::Skybox, dsd);



    dsd.DepthEnable = true;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    dsd.StencilEnable = false;
    dsd.StencilReadMask = 0xFF;
    dsd.StencilWriteMask = 0xFF;


    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;


    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    Insert(DepthStencilStateTypes::Default, dsd);
    return S_OK;
}

/*----------------------------------------------------DepthStencilStateManager Set()------------------------------*/

void DepthStencilStateManager::Set(DepthStencilStateTypes type)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();
    dc->OMSetDepthStencilState(states.find(type)->second->Get(), 0);
}

/*----------------------------------------------------DepthStencilStateManager Retrieve()------------------------------*/

ID3D11DepthStencilState* DepthStencilStateManager::Retrieve(DepthStencilStateTypes type)
{
    return states.find(type)->second->Get();
}
