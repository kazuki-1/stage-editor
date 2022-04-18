#pragma once
#include <d3d11.h>  
#include <DirectXMath.h>
#include <wrl.h>
#include <iostream>
#include <vector>
#include "Texture.h"
#include "TextureManager.h"
#include "OBJECT.h"
using namespace Microsoft::WRL;

class SPRITE : protected OBJECT
{
    struct VERTEX
    {
        Vector3 position;
        Vector4 colour;
        Vector2 uv;
    };
    std::shared_ptr<TEXTURE>texture;
    ComPtr<ID3D11Buffer>dxVertexBuffer, dxIndexBuffer;
    std::vector<VERTEX>Vertices;
public:
    SPRITE(const wchar_t* img_path);
    SPRITE() {};
    HRESULT Initialize(const wchar_t* img_path);
    void Render(Vector2 position = { 0,0 }, Vector2 scale = { 1, 1 }, Vector2 tPos = { 0, 0 }, Vector2 tSize = { 0, 0 }, Vector2 pivot = { 0, 0 }, Vector4 colour = { 1, 1, 1, 1 }, float angle = 0);
    std::shared_ptr<TEXTURE>Texture() { return texture; }
};

