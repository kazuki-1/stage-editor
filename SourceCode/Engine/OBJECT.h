#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <iostream>
#include <vector>
#include "Texture.h"
#include "BlendMode.h"
#include "SHADERS.h"
using namespace Microsoft::WRL;
using namespace DirectX;
class OBJECT
{
protected:

    std::vector<std::shared_ptr<SHADERS>>shaders;
    int* indices;
    int indexCount{}, vertexCount{};
    virtual void Render(ID3D11DeviceContext* dc) {};
    virtual ~OBJECT()
    {
        shaders.clear();    
    }
public:
    HRESULT InsertShader(std::wstring shader_name)
    {
        shaders.push_back(SHADERMANAGER::Instance()->Retrieve(shader_name));
        return shaders.back() ? S_OK : E_INVALIDARG;
    }
    HRESULT SetShaderAt(std::wstring shader_name, int index = 0)
    {
        shaders[index] = SHADERMANAGER::Instance()->Retrieve(shader_name);
    }
};
