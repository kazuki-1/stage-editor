#pragma once
#include <d3d11.h>  
#include <DirectXMath.h>
#include <wrl.h>
#include <iostream>
#include <vector>
#include "Texture.h"
#include "TEXTURE_MANAGER.h"
#include "OBJECT.h"
using namespace Microsoft::WRL;

class SPRITE : protected OBJECT
{
    struct VERTEX
    {
        XMFLOAT3 position;
        XMFLOAT4 colour;
        XMFLOAT2 uv;
    };
    std::shared_ptr<TEXTURE>texture;
    ComPtr<ID3D11Buffer>dxVertexBuffer, dxIndexBuffer;
    std::vector<VERTEX>Vertices;
public:
    SPRITE(const wchar_t* img_path, ID3D11Device* dv);
    SPRITE() {};
    HRESULT Initialize(const wchar_t* img_path, ID3D11Device* dv);
    void Render(XMFLOAT2 position = { 0,0 }, XMFLOAT2 scale = { 1, 1 }, XMFLOAT2 tPos = { 0, 0 }, XMFLOAT2 tSize = { 0, 0 }, XMFLOAT2 pivot = { 0, 0 }, XMFLOAT4 colour = { 1, 1, 1, 1 }, float angle = 0);
    std::shared_ptr<TEXTURE>Texture() { return texture; }
};

