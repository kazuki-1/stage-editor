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
        MODEL_RESOURCES* resource = model->Resource().get();

        Vector4 object_colour = resource->data.colour;
        XMFLOAT4X4 object_world_transform{};
        object_world_transform = model->Transform();


        for (auto& mesh : model->Resource()->Meshes)
        {
            UINT stride{ sizeof(MODEL_RESOURCES::VERTEX) }, offset{ 0 };
            dc->IASetVertexBuffers(0, 1, mesh.dxVertexBuffer.GetAddressOf(), &stride, &offset);

            // Converting  Axis Systems to Base Axis
            XMMATRIX f_World{ 
            XMLoadFloat4x4(&mesh.BaseTransform) * (
            XMLoadFloat4x4(&resource->Axises.AxisCoords) * 
            XMLoadFloat4x4(&object_world_transform))}; 

            XMStoreFloat4x4(&resource->data.world, f_World);




            const size_t b_Count{ mesh.Bind_Pose.Bones.size() };
            for (size_t b_Index = 0; b_Index < b_Count; ++b_Index)
            {
                const MODEL_RESOURCES::SKELETON::BONE& b{ mesh.Bind_Pose.Bones.at(b_Index) };
                const MODEL_RESOURCES::ANIMATION::KEYFRAME::NODE& bNode{ model->GetCurrentAnimationKeyframe()->Nodes.at(b.n_Index)};
            
            
                XMStoreFloat4x4(&resource->data.b_Transform[b_Index],
                    XMLoadFloat4x4(&b.o_Transform)
                    * XMLoadFloat4x4(&bNode.g_Transform)
                    * XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.BaseTransform)));
            
            }

            for (auto& subset : mesh.Subsets)
            {

                dc->IASetIndexBuffer(subset.subsetIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
                MODEL_RESOURCES::MATERIAL* material = &model->Resource()->Materials.at(subset.m_UID);

                ID3D11ShaderResourceView* textures[] =
                {
                material->Textures[0]->GetSRV().Get(),
                material->Textures[1]->GetSRV().Get(),
                material->Textures[2]->GetSRV().Get(),
                material->Textures[3]->GetSRV().Get()
                };



                dc->PSSetShaderResources(0, (UINT)ARRAYSIZE(textures), textures);


                XMStoreFloat4(&model->Resource()->data.colour, object_colour.XMV() * XMLoadFloat4(&material->Kd));
                UpdateConstantBuffers(model);

                dc->DrawIndexed((UINT)subset.indices.size(), 0, 0);

            }
        }
        ++index;
        resource->data.world = object_world_transform;
        object->DisableRendering();
    }
    CleanupShaders();
}

/*---------------------------------------------Shader CleanupShaders()--------------------------------------------------------------------*/

void Shader::CleanupShaders()
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();
    dc->PSSetShader(0, 0, 0);
    dc->VSSetShader(0, 0, 0);
    dc->PSSetConstantBuffers(1, 0, 0);
    dc->VSSetConstantBuffers(1, 0, 0);
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