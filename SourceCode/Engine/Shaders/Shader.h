#pragma once
#include <d3dcompiler.h>
#include <string>
#include <memory>
#include <map>
#include <filesystem>
//#include "OBJECT.h"
#include "../DirectX11.h"
#include "../Singleton.h"

class OBJECT;
class Shader
{
protected:
    std::vector<OBJECT*>objects;


    ComPtr<ID3D11PixelShader>pixelShader;
    ComPtr<ID3D11VertexShader>vertexShader;
    ComPtr<ID3D11InputLayout>inputLayout;

    std::wstring shader_path{};
public:
    // Pixel Shader Retriever
    ComPtr<ID3D11PixelShader>PSS()
    {
        return pixelShader;
    }
    // Vertex Shader Retriever
    ComPtr<ID3D11VertexShader>VSS()
    {
        return vertexShader;
    }
    // Sampler State Retriever
    ComPtr<ID3D11InputLayout>InputLayout()
    {
        return inputLayout;
    }
    Shader() {}
    Shader(std::wstring shader_path);
    virtual HRESULT Initialize();
    virtual void SetShaders();
    virtual void UpdateConstantBuffers(OBJECT* parent) {};
    virtual void SetConstantBuffers() {};
    virtual void Render();
    /// <summary>
    /// Call this at the end of the render process to remove all shaders, which will be set before rendering again
    /// </summary>
    virtual void CleanupShaders();
    void Register(OBJECT* object);
    void Deregister(OBJECT* object);
};






