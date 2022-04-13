#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#include "LIGHTING.h"
#define PLIGHT_MAX 8
#define SLIGHT_MAX 8
using namespace DirectX;





class Graphics
{
    Microsoft::WRL::ComPtr<ID3D11Buffer>dxSceneConstantBuffer;
    bool Render();
public:
    Graphics() {}
    static Graphics* Instance()
    {
        static Graphics g;
        return &g;
    }
    struct SCENE_CONSTANT_DATA
    {
        // View Projection
        XMFLOAT4X4 view_proj{};
        // Camera Position
        XMFLOAT4 camera_position{};
        XMFLOAT4 ambientLightColour;
        // Light Data
        DLIGHT_DATA directional;
        PLIGHT_DATA pointlights[PLIGHT_MAX];
        SLIGHT_DATA spotlights[SLIGHT_MAX];
        int pLightCount{};
        int sLightCount{};
        Vector2 temp;
    };
    
    HRESULT Initialize(int Width, int Height, HWND hwnd);
    bool Frame();
    void Finalize();
};