#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>
#include <iostream>
#include <vector>
#include <map>
#include "Texture.h"
#include "BlendMode.h"
#include "Shaders/Shader.h"
#include "Shaders/ShaderManager.h"
using namespace Microsoft::WRL;
using namespace DirectX;
class OBJECT
{
protected:

    std::map<ShaderTypes, std::shared_ptr<Shader>>shaders;
    int* indices{};
    int indexCount{}, vertexCount{};
    virtual void Render(ID3D11DeviceContext* dc) {};
    virtual ~OBJECT()
    {
        shaders.clear();    
    }
public:
    /// <summary>
    /// Insert the shader to be used on the model
    /// </summary>
    /// <param name="shader_name"></param>
    /// <returns></returns>
    HRESULT InsertShader(ShaderTypes type)
    {
        shaders.emplace(type, ShaderManager::Instance()->Retrieve(type));
        return S_OK;
    }
    /// <summary>
    /// Removes the shader from the model
    /// </summary>
    /// <param name="shader_name"></param>
    /// <returns></returns>
    void RemoveShader(ShaderTypes type)
    {
        shaders.erase(type);



    }
};
