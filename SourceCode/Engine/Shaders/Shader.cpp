#include "Shader.h"
#include "../RASTERIZER.h"
#include "../SamplerStateManager.h"
#include "../DepthStencilStateManager.h"
#include "../MODEL.h"
#include "../BlendMode.h"
//#include "PhongShader.h"
#include "../MODEL_RESOURCE.h"


/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Class--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Constructor--------------------------------------------------------------------*/

Shader::Shader(std::wstring path) : shader_path(path)
{


}

/*---------------------------------------------Shader Initialize()--------------------------------------------------------------------*/

HRESULT Shader::Initialize()
{
    return S_OK;
}

/*---------------------------------------------Shader SetShaders()--------------------------------------------------------------------*/

void Shader::SetShaders()
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(pixelShader.Get(), 0, 0);
    dc->IASetInputLayout(inputLayout.Get());
    DepthStencilStateManager::Instance()->Set(DepthStencilStateTypes::Default);
    SamplerStateManager::Instance()->Set(SamplerStateType::Default);
    //ID3D11SamplerState* sampler = SamplerStateManager::Instance()->Retrieve(SamplerStateType::Default)->sampler.Get();
    //dc->PSSetSamplers(0, 1, &sampler);

}



/*---------------------------------------------Shader Render()--------------------------------------------------------------------*/

void Shader::Render()
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    SetShaders();
    SetConstantBuffers();

    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    RasterizerManager::Instance()->Set(RasterizerTypes::Base_3D);
    BlendStateManager::Instance()->Set(BlendModes::Alpha);
    int index{};
    for (auto& object : objects)
    {
        if (!object->GetRenderState())
            continue;
        if (!object)
        {
            objects.erase(objects.begin() + index);
            continue;
        }
        MODEL* model = (MODEL*)object;
        //model->Render();
        for (auto& mesh : model->Resource()->Meshes)
        {
            UINT stride{ sizeof(MODEL_RESOURCES::VERTEX) }, offset{ 0 };

            dc->IASetVertexBuffers(0, 1, mesh.dxVertexBuffer.GetAddressOf(), &stride, &offset);
            for (auto& subset : mesh.Subsets)
            {

                dc->IASetIndexBuffer(subset.subsetIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
                MODEL_RESOURCES::MATERIAL* material = &model->Resource()->Materials.at(subset.m_UID);

                ID3D11ShaderResourceView* textures[] =
                {
                material->Textures[0]->GetSRV().Get(),
                material->Textures[1]->GetSRV().Get() ,
                material->Textures[2]->GetSRV().Get() ,
                material->Textures[3]->GetSRV().Get()
                };



                dc->PSSetShaderResources(0, (UINT)ARRAYSIZE(textures), textures);


                XMStoreFloat4(&model->Resource()->data.colour, XMLoadFloat4(&model->Resource()->data.colour) * XMLoadFloat4(&material->Kd));
                UpdateConstantBuffers(model);

                dc->DrawIndexed((UINT)subset.indices.size(),0, 0);

            }
        }
        ++index;
        object->DisableRendering();
    }
}

/*---------------------------------------------Shader Register()--------------------------------------------------------------------*/

void Shader::Register(OBJECT* object)
{
    objects.push_back(object);
}

/*---------------------------------------------Shader Deregister()--------------------------------------------------------------------*/

void Shader::Deregister(OBJECT* object)
{
    int ind{};
    for (auto& o : objects)
    {
        if (o == object)
            objects.erase(objects.begin() + ind);
        ++ind;
    }
}