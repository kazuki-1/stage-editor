#include "../Audio/AudioController.h"
#include "../Engine/SoundEffect.h"
#include "TerrainAudio.h"
#include "Mesh.h"
std::string types[] = { "Emitter", "Receiver" };

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TerrainAudio_Data Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TerrainAudio_Data Constructor------------------------------------------------*/

TerrainAudio_Data::TerrainAudio_Data()
{
    type = COMPONENT_TYPE::TerrainAudio_Component;

}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TerrainAudio_Component Class---------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TerrainAudio_Component Constructor------------------------------------------------*/

TerrainAudio_Component::TerrainAudio_Component(GameObject* g, ComponentData* d)
{
    data = static_cast<TerrainAudio_Data*>(d);
    parent = g;
}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT TerrainAudio_Component::Initialize()
{

    TerrainAudio_Data_Emitter* emitter = dynamic_cast<TerrainAudio_Data_Emitter*>(data->property_data.get());
    if (emitter)
    {
        AudioEngine::Instance()->Insert(emitter->audio_data->name, emitter->audio_data->file_path);
        buffers.resize(32);
        for (auto& b : buffers)
        {
            b.buffer = std::make_shared<SoundEffect>(emitter->audio_data->file_path);
            b.buffer->Initialize();
            b.buffer->SetBuffer(AudioEngine::Instance()->Retrieve(emitter->audio_data->name)->Buffer());

        }
    }




    return S_OK;
}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void TerrainAudio_Component::Execute()
{
    if (data->class_type != TerrainAudio_Property::EMITTER)
        return;
    for (auto& b : buffers)
    {
        if (b.state)
            ++b.timer;
        if (b.timer > 120)
        {
            b.buffer->Stop();
            b.timer = 0;
            b.state = false;
        }
    }
}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void TerrainAudio_Component::Render()
{

}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void TerrainAudio_Component::UI()
{
    if (ImGui::TreeNode("Terrain Audio"))
    {
        // Component is based on Mesh_Component component, so if not inserted, display nothing.
        Mesh_Component* mesh = GetComponent<Mesh_Component>();
        if (!mesh || !mesh->Model())
        {
            ImGui::Text("Mesh Component required!");
            ImGui::TreePop();
            return;
        }
        static int ind{ 0 };
        if (ImGui::BeginCombo("Type", types[ind].c_str()))
        {
            int cur{};
            for (auto& s : types)
            {
                if (ImGui::Selectable(s.c_str()))
                    ind = cur;
                ++cur;
            }
            ImGui::EndCombo();
        }



        if (ImGui::Button("Create Parameters") && ind >= 0)
        {
            data->class_type = (TerrainAudio_Property)ind;
            ind == (int)TerrainAudio_Property::EMITTER ? data->property_data = std::make_shared<TerrainAudio_Data_Emitter>() : data->property_data = std::make_shared<TerrainAudio_Data_Receiver>();
        }
        if (!data->property_data)
        {
            ImGui::TreePop();
            return;
        }


        switch (data->class_type)
        {
        case TerrainAudio_Property::EMITTER:
        {
            if (ImGui::TreeNode("Emitter Properties"))
            {


                TerrainAudio_Data_Emitter* current = static_cast<TerrainAudio_Data_Emitter*>(data->property_data.get());
                ImGui::InputText("Terrain name", current->name, 256);
                ImGui::Text(std::to_string(current->mesh_index).c_str());
                ImGui::InputText("Audio Name", current->audio_data->name, 256);
                IMGUI::Instance()->InputText("Audio File", &current->audio_data->file_path);

                MODEL_RESOURCES* mr{ mesh->Model()->Resource().get() };
                ImGui::ListBoxHeader("Materials");
                static int sel{ -1 };
                for (auto& m : mr->Materials)
                {
                    if (ImGui::Selectable(m.second.name.c_str()))
                    {
                        sel = (int)m.first;
                        break;
                    }
                }
                ImGui::ListBoxFooter();
                if (ImGui::Button("Set Parameters"))
                {
                    current->mesh_index = sel;
                    std::wstring dir{ L"./Data/Audio/" };
                    std::filesystem::path filename(current->audio_data->file_path);
                    dir += filename.filename().wstring();
                    current->audio_data->file_path = dir;
                }
                ImGui::TreePop();
            }
            break;
        }
        case TerrainAudio_Property::RECEIVER:
        {
            if (ImGui::TreeNode("Receiver Properties"))
            {
                Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
                TerrainAudio_Data_Receiver* current = static_cast<TerrainAudio_Data_Receiver*>(data->property_data.get());
                if (ImGui::BeginCombo("Insert Bone", "Select bone"))
                {
                    for (auto& m : mesh->Model()->Resource()->Meshes)
                    {
                        bool selected{};
                        for (auto& b : m.Bind_Pose.Bones)
                        {
                            if (ImGui::Selectable(b.Name.c_str()))
                            {
                                current->receiver_bones.push_back(b.Name);
                                selected = true;
                                break;
                            }
                        }
                        if (selected)
                            break;
                    }
                    ImGui::EndCombo();
                }
                ImGui::ListBoxHeader("Bones");
                for (auto& s : current->receiver_bones)
                    ImGui::Text(s.c_str());
                ImGui::ListBoxFooter();
                ImGui::TreePop();
            }
        }
        break;


        }

        ImGui::TreePop();
    }
}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para> Cycles through the buffer for unplayed audio and play it</para>
/// <para> バッファーの中にプレイしてないサウンドを探しプレイする</para>
/// </summary>
void TerrainAudio_Component::Play()
{
    TerrainAudio_Data_Emitter* emitter = dynamic_cast<TerrainAudio_Data_Emitter*>(data->property_data.get());
    if (!emitter)
        return;
    bool isDucking{ AudioController::Instance()->IsDucking() };
    for (auto& b : buffers)
    {
        if (isDucking)
            b.buffer->SetVolume(0.3f);
        else
            b.buffer->SetVolume(1.0f);
        if (b.state)
            continue;
        b.buffer->Play();
        b.state = true;
        break;
    }
}

/*----------------------------------------------TerrainAudio_Component Property()------------------------------------------------*/

TerrainAudio_Property TerrainAudio_Component::Property()
{
    return data->class_type;
}

/*----------------------------------------------TerrainAudio_Component Data()------------------------------------------------*/

TerrainAudio_Data* TerrainAudio_Component::Data()
{
    return data;
}

/*----------------------------------------------TerrainAudio_Component Buffers()------------------------------------------------*/

std::vector<TerrainAudio_Component::AUDIO_BUFFER> TerrainAudio_Component::Buffers()
{
    return buffers;
}

/*----------------------------------------------TerrainAudio_Component GetComponentType()------------------------------------------------*/

COMPONENT_TYPE TerrainAudio_Component::GetComponentType()
{
    return data->type;
}
