
#pragma comment(lib, "DirectXTK.lib")
#pragma once
#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;


class BLENDMODE
{
    BLENDMODE() {};
public:

    static BLENDMODE* Instance()
    {
        static BLENDMODE bm;
        return &bm;
    }
    ComPtr<ID3D11BlendState>dxBlendState;
    enum BLEND_MODE
    {
        ALPHA,
        LIGHTEN,
        DARKEN,
        ADD,
        SUBTRACT,
        MULTIPLY,
        REPLACE,
        SCREEN
    };
    HRESULT Create(BLEND_MODE m, ID3D11Device* dv);
    ComPtr<ID3D11BlendState>Get()
    {
        return dxBlendState;
    }
};