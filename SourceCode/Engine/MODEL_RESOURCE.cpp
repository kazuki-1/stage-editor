#include "Rasterizer.h"
#include "MODEL_RESOURCE.h"
#include <functional>
#include <sstream>
#include <filesystem>
#include "Shaders/Shader.h"
#include "BlendMode.h"
std::wstring texture_names[] = { L"Default_Diffuse", L"Default_Normal", L"Default_Diffuse" , L"Default_Diffuse" };

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------MODEL_RESOURCES Class---------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------MODEL_RESOURCES Constructor---------------------------------------------------------------*/

MODEL_RESOURCES::MODEL_RESOURCES(ID3D11Device* dv, std::string model_path, bool Triangulate)
{

    std::filesystem::path path(model_path);
    if (path.extension() != ".mrs")
        path.replace_extension("mrs");

    if (!std::filesystem::exists(path))
        assert("File does not exist");
    std::ifstream ifs(path, std::ios::binary);
    cereal::BinaryInputArchive in(ifs);
    in(Scenes, Axises, Meshes, Materials, Animations);
    CreateBuffers(dv, model_path.c_str());

}

/*-------------------------------------------MODEL_RESOURCES CreateBuffers()---------------------------------------------------------------*/

void MODEL_RESOURCES::CreateBuffers(ID3D11Device* dv, const char* model_path)
{
    for (auto& m : Meshes)
    {
        m.vbd.ByteWidth =(UINT)( m.Vertices.size() * sizeof(VERTEX));
        m.vbd.Usage = D3D11_USAGE_DEFAULT;
        m.vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        m.vd.pSysMem = m.Vertices.data();

        HRESULT hr = dv->CreateBuffer(&m.vbd, &m.vd, m.dxVertexBuffer.GetAddressOf());
        if (FAILED(hr))
            assert(!"Failed to create Vertex Buffer");


        for (int a = 0; a < m.Subsets.size(); ) {
            if (m.Subsets[a].indices.size() <= 0) {
                m.Subsets.erase(m.Subsets.begin() + a);
            }
            else {
                D3D11_BUFFER_DESC ibd{};
                ibd.Usage = D3D11_USAGE_DEFAULT;
                ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
                ibd.ByteWidth = (UINT)(sizeof(int) * m.Subsets[a].indices.size());
                D3D11_SUBRESOURCE_DATA id{};
                id.pSysMem = m.Subsets[a].indices.data();
                HRESULT hr = DirectX11::Instance()->Device()->CreateBuffer(&ibd, &id, m.Subsets[a].subsetIndexBuffer.GetAddressOf());
                if (FAILED(hr))
                    assert(!"Failed to create Index Buffer");
                ++a;
            }

        }
    }


    bool hasTexture{};
    for (auto m = Materials.begin(); m != Materials.end(); ++m)
    {

        // Set material path
        // 
        const char* back{ "/" };
        std::filesystem::path bs{ back };
        std::filesystem::path path(model_path);
        path.replace_extension("");
        std::filesystem::path model_name{ path.filename() };
        std::string dir{ "./Data/Model/" + model_name.string() + "/Textures/" };
        std::filesystem::path directory("/Textures/");
        path += directory;
        std::string form{ path.string() };

        // Begin setting texture from path
        for (int ind = 0; ind < 4; ++ind)
        {
            if (m->second.texture_path[ind] != "")
            {
                // If model has a texture loaded
                std::filesystem::path format{ dir };
                if (!std::filesystem::exists(format))
                    std::filesystem::create_directories(format);
                std::filesystem::path filename{ m->second.texture_path[ind] };
                filename.replace_extension("png");

                path = format += filename.filename();
                m->second.Textures[ind] = TextureManager::Instance()->Retrieve(path.wstring());
            }
            else
                // Loads an empty texture
                m->second.Textures[ind] = TextureManager::Instance()->Retrieve(texture_names[ind]);
        }


    }


}

/*-------------------------------------------MODEL_RESOURCES UpdateAnimation()---------------------------------------------------------------*/

void MODEL_RESOURCES::UpdateAnimation(ANIMATION::KEYFRAME* kf)
{
    size_t n_Count{ kf->Nodes.size() };
    for (size_t n_Index = 0; n_Index < n_Count; ++n_Index)
    {
        ANIMATION::KEYFRAME::NODE& n{ kf->Nodes.at(n_Index) };
        XMMATRIX S{ XMMatrixScaling(n.Scaling.x, n.Scaling.y, n.Scaling.z) };
        XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&n.Rotation)) };
        XMMATRIX T{ XMMatrixTranslation(n.Translation.x, n.Translation.y, n.Translation.z) };

        int64_t p_Index{ Scenes.Nodes.at(n_Index).p_Index };
        XMMATRIX P{ p_Index >= 0 ? XMLoadFloat4x4(&kf->Nodes.at(p_Index).g_Transform) : XMMatrixIdentity() };

        XMStoreFloat4x4(&n.g_Transform, S * R * T * P);

    }
}


/*-------------------------------------------MODEL_RESOURCES BlendAnimation()---------------------------------------------------------------*/


void MODEL_RESOURCES::BlendAnimation(ANIMATION::KEYFRAME* start, ANIMATION::KEYFRAME* end, float factor, ANIMATION::KEYFRAME* output)
{
    size_t n_Count{ start->Nodes.size() };

    output->Nodes.resize(n_Count);
    for (size_t n_Index = 0; n_Index < n_Count; ++n_Index)
    {
        XMVECTOR S[2] = { XMLoadFloat3(&start->Nodes.at(n_Index).Scaling), XMLoadFloat3(&end->Nodes.at(n_Index).Scaling) };
        XMVECTOR R[2] = { XMLoadFloat4(&start->Nodes.at(n_Index).Rotation), XMLoadFloat4(&end->Nodes.at(n_Index).Rotation) };
        XMVECTOR T[2] = { XMLoadFloat3(&start->Nodes.at(n_Index).Translation), XMLoadFloat3(&end->Nodes.at(n_Index).Translation) };


        XMStoreFloat3(&output->Nodes.at(n_Index).Scaling, XMVectorLerp(S[0], S[1], factor));
        XMStoreFloat4(&output->Nodes.at(n_Index).Rotation, XMQuaternionSlerp(R[0], R[1], factor));
        XMStoreFloat3(&output->Nodes.at(n_Index).Translation, XMVectorLerp(T[0], T[1], factor));
    }

}

/*-------------------------------------------MODEL_RESOURCES Render()---------------------------------------------------------------*/

void MODEL_RESOURCES::Render(ID3D11DeviceContext* dc, XMFLOAT4X4 world, XMFLOAT4 colour, const ANIMATION::KEYFRAME* kf)
{

    world_transform = XMLoadFloat4x4(&world);
    data.world = world;
    data.colour = colour;               // Incase model has no subsets

        for (auto& m : Meshes)
        {
            // move to shader
            //dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            //RasterizerManager::Instance()->Set(RasterizerTypes::Base_3D);
            //BlendStateManager::Instance()->Set(BlendModes::Alpha);
            // Updating Object Constant buffers (World and colour)
            // For some reason, the normals and coordinates are all flipped in release build
//#ifdef _DEBUG
            //data.world = world;
//#else     
//            XMMATRIX mat = { -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
//            XMMATRIX f_World{ XMLoadFloat4x4(&m.BaseTransform) * XMLoadFloat4x4(&world) };       // Converting  Axis Systems to Base Axis
//#endif


              // Converting  Axis Systems to Base Axis
            //XMMATRIX f_World
            //{ 
            //    XMLoadFloat4x4(&m.BaseTransform) * 
            //    
            //        XMLoadFloat4x4(&Axises.AxisCoords) *
            //        XMLoadFloat4x4(&world)
            //     
            //}; 


            //XMStoreFloat4x4(&data.world, f_World);

            //const size_t b_Count{ m.Bind_Pose.Bones.size() };
            //for (size_t b_Index = 0; b_Index < b_Count; ++b_Index)
            //{
            //    const SKELETON::BONE& b{ m.Bind_Pose.Bones.at(b_Index) };
            //    const ANIMATION::KEYFRAME::NODE& bNode{ kf->Nodes.at(b.n_Index) };


            //    XMStoreFloat4x4(&data.b_Transform[b_Index],
            //        XMLoadFloat4x4(&b.o_Transform)
            //        * XMLoadFloat4x4(&bNode.g_Transform)
            //        * XMMatrixInverse(nullptr, XMLoadFloat4x4(&m.BaseTransform)));

            //}

            //for (const auto& s : m.Subsets)
            //{
            //    //MATERIAL& ms{ Materials.at(s.m_UID) };
            //    //XMStoreFloat4(&data.colour, XMLoadFloat4(&colour) * XMLoadFloat4(&ms.Kd));
            //    //shader.second->UpdateConstantBuffers(dc, this);


            //    //ID3D11ShaderResourceView* textures[] =
            //    //{
            //    //ms.Textures[0]->GetSRV().Get(),
            //    //ms.Textures[1]->GetSRV().Get() ,
            //    //ms.Textures[2]->GetSRV().Get() ,
            //    //ms.Textures[3]->GetSRV().Get()
            //    //};

            //    //// move to shader
            //    //dc->PSSetShaderResources(0, (UINT)ARRAYSIZE(textures), textures);

            //    //dc->PSSetShaderResources(0, ts.size(), ts.data());
            //    //int ind{};
            //    //for (auto& t : ms.Textures)
            //    //{
            //    //    dc->PSSetShaderResources(ind, 1, t->GetSRV().GetAddressOf());
            //    //    ++ind;
            //    //}

            //    // move to shader
            //    //dc->DrawIndexed((UINT)(s.indices.size()), 0, 0);
            //}
        }
    //}
}

/*-------------------------------------------MODEL_RESOURCES Recreate()---------------------------------------------------------------*/

void MODEL_RESOURCES::Recreate(std::string file_path)
{
    std::filesystem::path path(file_path);
    if (std::filesystem::exists(file_path))
        assert(!"File already exists");
    std::ofstream ofs(path, std::ios::binary);
    cereal::BinaryOutputArchive in(ofs);
    in(Scenes, Axises, Meshes, Materials, Animations);
}
