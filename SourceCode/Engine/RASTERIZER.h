#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <map>
#include <string>
#include <memory>
#include <Library/Singleton.h>
using namespace Microsoft::WRL;

enum class RasterizerTypes
{
    Base_2D, 
    Base_3D, 
    Wireframe, 
    Skybox, 
    ShadowMapper, 

    End

};

class Rasterizer
{
    ComPtr<ID3D11RasterizerState>rasterizer;
public:

    Rasterizer(ID3D11Device* dv, D3D11_RASTERIZER_DESC drd);
    HRESULT Initialize(ID3D11Device* dv,D3D11_RASTERIZER_DESC drd);
    ComPtr<ID3D11RasterizerState>GetRasterizer();
};

class RasterizerManager : public Singleton<RasterizerManager>
{
    std::map<RasterizerTypes, std::shared_ptr<Rasterizer>>rasterizers;

public:
    void Initialize();
    void Insert(RasterizerTypes type, ID3D11Device* dv, D3D11_RASTERIZER_DESC drd);
    void Remove(RasterizerTypes type);
    void Set(RasterizerTypes type);

    std::shared_ptr<Rasterizer> Retrieve(RasterizerTypes type);
    std::map < RasterizerTypes, std::shared_ptr<Rasterizer>>GetRasterizers();

};