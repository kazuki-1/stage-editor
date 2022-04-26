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
        for (auto& entity : emitter->dataset) {
            AudioEngine::Instance()->Insert(entity->audio_data->name, entity->audio_data->file_path);
            buffers.resize(32);
            for (auto& b : buffers)
            {
                b.buffer = std::make_shared<SoundEffect>(entity->audio_data->file_path);
                b.buffer->Initialize();
                b.buffer->SetBuffer(AudioEngine::Instance()->Retrieve(entity->audio_data->name)->Buffer());

            }
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
                // Prepare TerrainAudio_Data_Emitter entities
                TerrainAudio_Data_Emitter* emitter = static_cast<TerrainAudio_Data_Emitter*>(data->property_data.get());
                static bool fileOpenTA{};
                ImGui::FileBrowser* browser{ IMGUI::Instance()->FileBrowser() };
                static int cur_id{};
                static std::string previewTA{};
                if (ImGui::Button("Create Entity"))
                {
                    emitter->dataset.push_back(std::make_shared<TerrainAudio_Data_Emitter::DataStruct>());
                    cur_id++;
                }
                static int cur_data_index{};
                std::string cur_preview{ "Entity " + std::to_string(cur_data_index) };

                if (ImGui::BeginCombo("TerrainAudio_Data_Emitter Entities", cur_preview.c_str()))
                {
                    int index{};
                    for (auto& d : emitter->dataset)
                    {
                        std::string preview{ "Entity " + std::to_string(index) };
                        if (ImGui::Selectable(preview.c_str()))
                        {
                            cur_data_index = index;
                            break;
                        }
                        ++index;
                    }
                    ImGui::EndCombo();
                }





                if (emitter->dataset.size() > 0)
                {


                    TerrainAudio_Data_Emitter::DataStruct* current{ emitter->dataset[cur_data_index].get() };
                    ImGui::InputText("Terrain name", current->audio_data->name, 256);
                    ImGui::Text(std::to_string(current->mesh_index).c_str());
                    ImGui::InputText("Audio Name", current->audio_data->name, 256);
                    ImGui::Text(previewTA.c_str());

                    if (ImGui::Button("Load Audio"))
                    {
                        browser->Open();
                        browser->SetTitle("Open audio file");
                        browser->SetTypeFilters({ ".wav", ".*" });
                        fileOpenTA = true;
                    }
                    if (fileOpenTA)
                    {
                        browser->Display();
                        if (browser->HasSelected())
                        {
                            current->audio_data->file_path = browser->GetSelected().wstring();
                            fileOpenTA = false;


                            std::filesystem::path path(current->audio_data->file_path);
                            std::filesystem::path name(path.filename());
                            std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
                            AudioEngine::Instance()->Insert(current->audio_data->name, full_path);
                            current->audio_data->file_path = full_path;
                            for (auto& c : current->audio_data->file_path)
                            {
                                previewTA.push_back(c);
                            }

                            browser->Close();
                        }
                    }

                    MODEL_RESOURCES* mr{ mesh->Model()->Resource().get() };
                    static int sel{ 0 };
                    int ind{};
                    if (ImGui::BeginCombo("Meshes", mr->Meshes[sel].Name.c_str()))
                    {
                        for (auto& mesh : mr->Meshes)
                        {
                            if (ImGui::Selectable(mesh.Name.c_str()))
                            {
                                sel = ind;
                                break;
                            }
                            ++ind;
                        }
                        ImGui::EndCombo();
                    }
                    if (ImGui::Button("Set Parameters"))
                    {
                        current->mesh_index = sel;
                        std::wstring dir{ L"./Data/Audio/" };
                        std::filesystem::path filename(current->audio_data->file_path);
                        dir += filename.filename().wstring();
                        current->audio_data->file_path = dir;
                    }
                    ImGui::Separator();
                    if (ImGui::Button("Remove current entity"))
                    {
                        emitter->dataset.erase(emitter->dataset.begin() + cur_data_index);
                        cur_data_index = 0;
                    }
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


                static int sel_bone_index{};
                int current_index{};

                for (auto& s : current->receiver_bones)
                {
                    if (ImGui::Selectable(s.c_str()))
                    {
                        sel_bone_index = current_index;
                        break;
                    }
                    current_index++;
                }

                ImGui::ListBoxFooter();
                ImGui::Separator();
                if (ImGui::Button("Remove selected bone")) {
                    current->receiver_bones.erase(current->receiver_bones.begin() + sel_bone_index);
                    sel_bone_index = 0;
                }


                ImGui::TreePop();
            }
            break;
        }


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
