#include "BlendMode.h"
#include <assert.h>
#include "DirectX11.h"
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------BlendState Class------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------BlendState Constructor------------------------------------------------------*/

BlendState::BlendState(ID3D11Device* dv, D3D11_BLEND_DESC dbd)
{
    assert(dv->CreateBlendState(&dbd, blendState.GetAddressOf()) == S_OK);
}



HRESULT BlendStateManager::Initialize() 
{


    ID3D11Device* dv{ DirectX11::Instance()->Device() };

    D3D11_BLEND_DESC bld{};
    bld.AlphaToCoverageEnable = true;
    bld.IndependentBlendEnable = false;
    bld.RenderTarget[0].BlendEnable = true;
    bld.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // dst (Displaying colour on the screen)
    // src (Colour of the rendered object)
    // res (Resulting colour)
    // alpha 
    //  res = dst * (1 - src.a) + src * src.a

    // Alpha 
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    Insert(BlendModes::Alpha, bld);

    // Lighten 
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    Insert(BlendModes::Lighten, bld);

    // Darken
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    Insert(BlendModes::Darken, bld);

    // Addition
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    Insert(BlendModes::Add, bld);

    // Subtract
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_SUBTRACT;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    Insert(BlendModes::Subtract, bld);

    // Multiply
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    Insert(BlendModes::Multiply, bld);

    // Replace
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    Insert(BlendModes::Replace, bld);

    // Screen
    bld.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bld.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bld.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bld.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bld.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    Insert(BlendModes::Screen, bld);
    return S_OK;
}


void BlendStateManager::Insert(BlendModes mode, D3D11_BLEND_DESC dbd)
{
    states.emplace(mode, std::make_shared<BlendState>(DirectX11::Instance()->Device(), dbd));
}


void BlendStateManager::Set(BlendModes mode)
{
    DirectX11::Instance()->DeviceContext()->OMSetBlendState(states.find(mode)->second->Get(), 0, 0xFFFFFFFFF);
}