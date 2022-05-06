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
    ComPtr<ID3D11PixelShader>pixelShader;
    ComPtr<ID3D11VertexShader>vertexShader;
    ComPtr<ID3D11InputLayout>inputLayout;
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
    Shader(std::wstring shader_path, ID3D11Device* dv, D3D11_INPUT_ELEMENT_DESC* ied, UINT ied_count);
    virtual void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent);
    virtual void UpdateConstantBuffer(ID3D11DeviceContext* dc, OBJECT* parent) {};
    virtual void SetConstantBuffers(ID3D11DeviceContext* dc) {};

};

enum class ShaderTypes
{
    Shader_2D, 
    PhongShader, 
    Outline, 
    Skybox, 

    Debug_3D, 
    Base_3D

};






class ShaderManager : public Singleton<ShaderManager>
{
    std::map<ShaderTypes, std::shared_ptr<Shader>>shaders;

public:

    void Set(std::wstring shader_name);
    void Clear();
    HRESULT Initialize();
    HRESULT Insert(ShaderTypes type, std::shared_ptr<Shader>s);



    std::shared_ptr<Shader>Retrieve(ShaderTypes type);
    ~ShaderManager()
    {
        Clear();
    }


};