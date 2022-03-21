#include "RASTERIZER.h"
#include "MODEL_RESOURCE.h"
#include <functional>
#include <sstream>
#include <filesystem>
#include "SHADERS.h"
#include "BlendMode.h"
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
    //InsertShader(L"OutlineShader.fx");
    InsertShader(L"PhongShader.fx");

}

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

        //a.ibd.ByteWidth = a.Indices.size() * sizeof(int);
        //a.ibd.Usage = D3D11_USAGE_DEFAULT;
        //a.ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        //a.id.pSysMem = a.Indices.data();
        //hr = dv->CreateBuffer(&a.ibd, &a.id, a.dxIndexBuffer.GetAddressOf());
        //if (FAILED(hr))
        //    assert(!"Failed to create Index Buffer");
        //a.index_Count = a.Indices.size();

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
                /*a.index_Count = a.Indices.size();*/
                ++a;
            }

        }
    }


    bool hasTexture{};
    for (auto m = Materials.begin(); m != Materials.end(); ++m)
    {
        for (auto m2 = Materials.begin(); m2 != Materials.end(); ++m2)
        {
            if (m2->second.texture_path[0] != "")
            {
                hasTexture = true;
                break;
            }
        }
        const char* back{ "/" };
        std::filesystem::path bs{ back };
        std::filesystem::path path(model_path);
        path.replace_extension("");
        std::filesystem::path model_name{ path.filename() };
        std::string dir{ "./Data/Model/" + model_name.string() + "/Textures/" };
        //model_name += 
        std::filesystem::path directory("/Textures/");
        path += directory;
        std::string form{ path.string() };

        for (int ind = 0; ind < 4; ++ind)
        {
            if (m->second.texture_path[ind] != "")
            {
                std::filesystem::path format{ dir };
                if (!std::filesystem::exists(format))
                    std::filesystem::create_directories(format);
                std::filesystem::path filename{ m->second.texture_path[ind] };
                filename.replace_extension("png");

                path = format += filename.filename();
                //m->second.Textures[ind].reset(new TEXTURE(path, dv));
                m->second.Textures[ind] = TEXTURE_MANAGER::Instance()->Retrieve(path.wstring());
            }
            else
                m->second.Textures[ind] = TEXTURE_MANAGER::Instance()->Retrieve(L"");
        }


    }
    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(MESH_CONSTANT_BUFFER);
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = dv->CreateBuffer(&cbd, nullptr, meshConstantBuffer.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create constant buffer");


    cbd.ByteWidth = sizeof(SUBSET_CONSTANT_BUFFER);
    hr = dv->CreateBuffer(&cbd, nullptr, subsetConstantBuffer.GetAddressOf());
    assert(hr == S_OK);

    cbd.ByteWidth = sizeof(OUTLINE_CONSTANT_BUFFER);
    hr = dv->CreateBuffer(&cbd, nullptr, outlineConstantBuffer.GetAddressOf());
    assert(hr == S_OK);


}

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


void MODEL_RESOURCES::Render(ID3D11DeviceContext* dc, XMFLOAT4X4 world, XMFLOAT4 colour, const ANIMATION::KEYFRAME* kf)
{
    OUTLINE_CONSTANT_BUFFER outlineData{};
    outlineData.outline_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
    outlineData.outline_size = 0.03f;
    dc->RSSetState(RASTERIZERMANAGER::Instance()->Retrieve("3D")->Rasterizer().Get());

    for (auto& s : shaders)
    {
        for (auto& m : Meshes)
        {
            dc->RSSetState(RASTERIZERMANAGER::Instance()->Retrieve("3D")->Rasterizer().Get());
            dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            dc->OMSetBlendState(BLENDMODE::Instance()->Get().Get(), 0, 0xFFFFFFFF);
            s->SetShaders(dc);
            // Updating Object Constant buffers (World and colour)
            MESH_CONSTANT_BUFFER data{};
            const ANIMATION::KEYFRAME::NODE& n{ kf->Nodes.at(0) };
            //XMMATRIX f_World{ XMLoadFloat4x4(&Axises.AxisCoords) * XMLoadFloat4x4(&world) };
            XMMATRIX f_World{ XMLoadFloat4x4(&m.BaseTransform) * (XMLoadFloat4x4(&Axises.AxisCoords) * XMLoadFloat4x4(&world)) };       // Converting  Axis Systems to Base Axis
            data.colour = colour;               // Incase model has no subsets
            //ANIMATION::KEYFRAME::NODE node{ kf->Nodes.at(m.n_Index) };
            //XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&node.g_Transform) * f_World);

            const size_t b_Count{ m.Bind_Pose.Bones.size() };
            for (size_t b_Index = 0; b_Index < b_Count; ++b_Index)
            {
                const SKELETON::BONE& b{ m.Bind_Pose.Bones.at(b_Index) };
                const ANIMATION::KEYFRAME::NODE& bNode{ kf->Nodes.at(b.n_Index) };
                XMStoreFloat4x4(&data.world, f_World);


                XMStoreFloat4x4(&data.b_Transform[b_Index],
                    XMLoadFloat4x4(&b.o_Transform)
                    * XMLoadFloat4x4(&bNode.g_Transform)
                    * XMMatrixInverse(nullptr, XMLoadFloat4x4(&m.BaseTransform)));


                //XMStoreFloat4x4(&data.b_Transform[b_Index],
                //    XMLoadFloat4x4(&b.o_Transform) 
                //    * XMMatrixInverse(nullptr, XMLoadFloat4x4(&m.BaseTransform))
                //);
            }



            for (const auto& s : m.Subsets)
            {

                UINT stride{ sizeof(VERTEX) }, offset{ 0 };
                dc->IASetIndexBuffer(s.subsetIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
                dc->IASetVertexBuffers(0, 1, m.dxVertexBuffer.GetAddressOf(), &stride, &offset);

                MATERIAL& ms{ Materials.at(s.m_UID) };
                //ms.Textures[0]->SetTexture(dc);
                XMStoreFloat4(&data.colour, XMLoadFloat4(&colour) * XMLoadFloat4(&ms.Kd));
                dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);
                dc->UpdateSubresource(outlineConstantBuffer.Get(), 0, 0, &outlineData, 0, 0);
                //dc->PSSetShaderResources(0, 1, ms.Textures[0]->GetSRV().GetAddressOf());
                //dc->PSSetShaderResources(1, 1, ms.Textures[1]->GetSRV().GetAddressOf());
                std::vector<ID3D11ShaderResourceView*>ts;
                for (int a = 0; a < 4; ++a)
                {
                    if (ms.texture_path[a] != "")
                        ts.push_back(ms.Textures[a]->GetSRV().Get());
                }

                //dc->PSSetShaderResources(0, (uint)ts.size(), ts.data());
                int ind{};
                for (auto& t : ms.Textures)
                {
                    dc->PSSetShaderResources(ind, 1, t->GetSRV().GetAddressOf());
                    ++ind;
                }
                ID3D11Buffer* buffers[] = { meshConstantBuffer.Get(), outlineConstantBuffer.Get() };
                dc->VSSetConstantBuffers(1, 2, buffers);
                dc->PSSetConstantBuffers(1, 2, buffers);
                dc->DrawIndexed((UINT)(s.indices.size()), 0, 0);
            }
        }
    }
}


