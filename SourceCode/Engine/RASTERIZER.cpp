#include "RASTERIZER.h"
#include <assert.h>
#include "DirectX11.h"


/*-----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------Rasterizer Class----------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------Rasterizer Constructor----------------------------------------------------*/

Rasterizer::Rasterizer(ID3D11Device* dv, D3D11_RASTERIZER_DESC drd)
{
    assert(Initialize(dv, drd) == S_OK);
}


/*---------------------------------------------------Rasterizer Initialize()----------------------------------------------------*/

HRESULT Rasterizer::Initialize(ID3D11Device* dv, D3D11_RASTERIZER_DESC drd)
{
    return dv->CreateRasterizerState(&drd, rasterizer.GetAddressOf());

}

/*---------------------------------------------------Rasterizer GetRasterizer()----------------------------------------------------*/

ComPtr<ID3D11RasterizerState>Rasterizer::GetRasterizer()
{
    return rasterizer;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------RasterizerManager Class----------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------RasterizerManager Initialize()---------------------------------------------------*/


void RasterizerManager::Initialize()
{
    ID3D11Device* dv = DirectX11::Instance()->Device();


    D3D11_RASTERIZER_DESC drd{};
    drd.AntialiasedLineEnable = false;
    drd.CullMode = D3D11_CULL_BACK;
    drd.DepthBias = 0;
    drd.DepthBiasClamp = 0.0f;
    drd.DepthClipEnable = false;
    drd.FillMode = D3D11_FILL_SOLID;
    drd.FrontCounterClockwise = TRUE;
    drd.MultisampleEnable = false;
    drd.ScissorEnable = false;
    drd.SlopeScaledDepthBias = 0.0f;
    Insert(RasterizerTypes::Base_3D, dv, drd);

    drd.MultisampleEnable = drd.DepthClipEnable = true;
    Insert(RasterizerTypes::Skybox, dv, drd);

    drd.CullMode = D3D11_CULL_NONE;
    Insert(RasterizerTypes::Base_2D, dv, drd);

    drd.FillMode = D3D11_FILL_WIREFRAME;
    Insert(RasterizerTypes::Wireframe, dv, drd);


}

/*---------------------------------------------------RasterizerManager Insert()----------------------------------------------------*/

void RasterizerManager::Insert(RasterizerTypes type, ID3D11Device* dv, D3D11_RASTERIZER_DESC drd)
{
    std::shared_ptr<Rasterizer>temp = std::make_shared<Rasterizer>(dv, drd);
    rasterizers.insert(std::make_pair(type, temp));
}

/*---------------------------------------------------RasterizerManager Retrieve()---------------------------------------------------*/

std::shared_ptr<Rasterizer>RasterizerManager::Retrieve(RasterizerTypes name)
{
    return rasterizers.find(name)->second;
}

/*---------------------------------------------------RasterizerManager Remove()---------------------------------------------------*/

void RasterizerManager::Remove(RasterizerTypes name)
{
    rasterizers.erase(name);
}


void RasterizerManager::Set(RasterizerTypes type)
{
    DirectX11::Instance()->DeviceContext()->RSSetState(rasterizers.find(type)->second->GetRasterizer().Get());
}

/*---------------------------------------------------RasterizerManager GetRasterizers()---------------------------------------------------*/

std::map<RasterizerTypes, std::shared_ptr<Rasterizer>>RasterizerManager::GetRasterizers()
{
    return rasterizers;
}
