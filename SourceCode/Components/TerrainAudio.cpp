#include "../Audio/AudioController.h"
#include "../Engine/SoundEffect.h"
#include "Transform3D.h"
#include "TerrainAudio.h"
#include "Mesh.h"
std::string types[] = { "Emitter", "Receiver" };
#define MAX_TIME 120
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

    int ind{};
    // Individual component id tags
    for (auto& c : GetParent()->GetListOfComponents<TerrainAudio_Component>())
        ++ind;
    data->id = ind;


}

/*----------------------------------------------TerrainAudio_Component Initialize()------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT TerrainAudio_Component::Initialize()
{

    // Checks if component is an emitter or a receiver
    {
        TerrainAudio_Data_Emitter* data = dynamic_cast<TerrainAudio_Data_Emitter*>(this->data->property_data.get());


        // Creates data accordingly
        if (data)
        {
            parameters = std::make_shared<TerrainAudio_Emitter>();
            TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)parameters.get();

            // Instantiates the audio file and load the buffer with it
            AudioEngine::Instance()->Insert(data->audio_data->name, data->audio_data->file_path);
            emitter->buffers.resize(32);
            for (auto& b : emitter->buffers)
            {
                b.buffer = std::make_shared<SoundEffect>(data->audio_data->file_path);
                b.buffer->Initialize();
                b.buffer->SetBuffer(AudioEngine::Instance()->Retrieve(data->audio_data->name)->Buffer());

            }
        }
    }
    TerrainAudio_Data_Receiver* data = dynamic_cast<TerrainAudio_Data_Receiver*>(this->data->property_data.get());
    if (data)
    {
        parameters = std::make_shared<TerrainAudio_Receiver>();
        ((TerrainAudio_Receiver*)parameters.get())->parameters.resize(data->receiver_bones.size());
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
    ExecuteEmitter();
    ExecuteReceiver();
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

    std::string id{ "Terrain Audio ##" + std::to_string(data->id) };
    if (ImGui::TreeNode(id.c_str() ))
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


        // Creating parameter depending on the type of the data
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

                ImGui::InputText("Audio Name", emitter->audio_data->name, 256);
                ImGui::Text(std::to_string(emitter->mesh_index).c_str());
                ImGui::Text(previewTA.c_str());
                    // Data insertion
                if (ImGui::Button("Load Audio"))
                {
                    browser->Open();
                    browser->SetTitle("Open audio file");
                    browser->SetTypeFilters({ ".wav", ".*" });
                    fileOpenTA = true;
                }

                // ImGui file browser function
                if (fileOpenTA)
                {
                    browser->Display();
                    if (browser->HasSelected())
                    {
                        emitter->audio_data->file_path = browser->GetSelected().wstring();
                        fileOpenTA = false;

                        // Changes the file path to be detected in a local path
                        std::filesystem::path path(emitter->audio_data->file_path);
                        std::filesystem::path name(path.filename());
                        std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
                        AudioEngine::Instance()->Insert(emitter->audio_data->name, full_path);
                        emitter->audio_data->file_path = full_path;

                        // Converts the file path from wstring to string
                        for (auto& c : emitter->audio_data->file_path)
                        {
                            previewTA.push_back(c);
                        }

                        browser->Close();
                    }
                }

                // Select the meshes to perform collision check
                std::shared_ptr<MODEL_RESOURCES> mr{ mesh->Model()->Resource() };
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

                // Finalize data insertion
                if (ImGui::Button("Set Parameters"))
                {
                    emitter->mesh_index = sel;
                    std::wstring dir{ L"./Data/Audio/" };
                    std::filesystem::path filename(emitter->audio_data->file_path);
                    dir += filename.filename().wstring();
                    emitter->audio_data->file_path = dir;
                }
                ImGui::TreePop();
            }
            break;
        }
        case TerrainAudio_Property::RECEIVER:
        {
            if (ImGui::TreeNode("Receiver Properties"))
            {
                // Parameter preparation
                Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
                TerrainAudio_Data_Receiver* current = static_cast<TerrainAudio_Data_Receiver*>(data->property_data.get());

                // Entity selection
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


                // Entity selection to remove
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
    if (data->class_type != TerrainAudio_Property::EMITTER)
        return;
    TerrainAudio_Emitter* emitter = static_cast<TerrainAudio_Emitter*>(parameters.get());
    if (!emitter)
        return;
    bool isDucking{ AudioController::Instance()->IsDucking() };
    for (auto& b : emitter->buffers)
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

/*----------------------------------------------TerrainAudio_Component ExecuteEmitter()------------------------------------------------*/

void TerrainAudio_Component::ExecuteEmitter()
{
    if (data->class_type == TerrainAudio_Property::EMITTER)
    {
        TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)parameters.get();
        for (auto& buffer : emitter->buffers)
        {
            if (buffer.buffer->IsDucking())
                buffer.buffer->SetVolume(0.3f);
            // Stops the soundEffect if it goes past 2 seconds
            if (buffer.state)
                ++buffer.timer;
            if (buffer.timer > MAX_TIME)
            {
                buffer.buffer->Stop();
                buffer.timer = 0;
                buffer.state = false;
            }
        }
    }
}

/*----------------------------------------------TerrainAudio_Component ExecuteReceiver()------------------------------------------------*/

void TerrainAudio_Component::ExecuteReceiver()
{
    // Execute functions for emitter
    //PlayerController_Component* playerController = GetComponent<PlayerController_Component>();
    if (data->class_type != TerrainAudio_Property::RECEIVER)
        return;
    Mesh_Component* mesh = GetComponent<Mesh_Component>();
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    TerrainAudio_Data_Receiver* data = (TerrainAudio_Data_Receiver*)this->data->property_data.get();
    TerrainAudio_Receiver* receiver = (TerrainAudio_Receiver*)parameters.get();
    // Stops the function if gameObject does not have a playerController
    // if (!playerController)
    //     return;


    // Search all gameobjects for emitters
    if (receiver->list_of_emitters.size() < 1)
    {
        for (auto& object : GAMEOBJECTS->GetGameObjects())
        {

            // Skips this object. Just in case
            if (object.second.get() == this->GetParent())
                continue;

            for (auto& entity : object.second->GetListOfComponents<TerrainAudio_Component>())
            {
                if (entity->data->class_type == TerrainAudio_Property::EMITTER)
                    receiver->list_of_emitters.push_back(entity);
            }

            //if (component->data->class_type == TerrainAudio_Property::EMITTER)
            //    receiver->list_of_emitters.push_back(object.second.get());
        }
    }

    // Prepare a vector coordinates of the receivers
    std::vector<Vector3>receiver_coords;
    receiver_coords.resize(data->receiver_bones.size());
    for (int index = 0; index < receiver_coords.size(); ++index)
    {
        // Retrieve their coordinates via their bone
        XMVECTOR s, r, t;
        XMMatrixDecompose(&s, &r, &t, mesh->GetBoneTransform(data->receiver_bones[index]));
        receiver_coords[index].Load(t);
    }

    // Begin collision check
    std::shared_ptr<GameObject>player = GameObjectManager::Instance()->GetPlayer();
    if (!player)
        return;
    Mesh_Component* player_mesh = player->GetComponent<Mesh_Component>();
    for (int index = 0; index < data->receiver_bones.size(); ++index)
    {
        // Prepare the rays
        Vector3 start, end;
        start = receiver_coords[index];

        // Error compensation
        start.y += 0.092f;
        end = start;
        end.y -= 0.022f;
        // Perform rayCasting collision check
        COLLIDERS::RAYCASTDATA rcd{};
        bool collided;
        collided = RAYCAST(start, end, player_mesh, rcd);
        // Updates the collided mesh index
        if (rcd.m_Index >= 0 && collided)
            receiver->parameters[index].collided_mesh_index = rcd.m_Index;
        else if (!collided)
            receiver->parameters[index].collided_mesh_index = -1;

    }





    //for (auto& entity : receiver->list_of_emitters)
    //{
    //    //TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)(entity->parameters.get());
    //    Mesh_Component* emitter_mesh = entity->GetComponent<Mesh_Component>();
    //    TerrainAudio_Data_Emitter* emitter_data = (TerrainAudio_Data_Emitter*)(entity->data->property_data.get());

    //    for (int index = 0; index < data->receiver_bones.size(); ++index)
    //    {
    //        // Prepare the rays
    //        Vector3 start, end;
    //        start = receiver_coords[index];

    //        // Error compensation
    //        start.y += 0.092f;
    //        end = start;
    //        end.y -= 0.022f;


    //        // Perform rayCasting collision check
    //        COLLIDERS::RAYCASTDATA rcd{};
    //        bool collided;
    //        collided = RAYCAST(start, end, emitter_mesh,
    //            emitter_data->mesh_index, rcd);
    //        // Updates the collided mesh index
    //        if (rcd.m_Index >= 0 && collided)
    //            receiver->parameters[index].collided_mesh_index = rcd.m_Index;
    //        else if (!collided)
    //            receiver->parameters[index].collided_mesh_index = -1;

    //    }
    //}

    // Plays the audio if the collider has been triggered

    for (auto& status : receiver->parameters)
    {

        // Update the trigger status based on what it previously collided with
        if (status.previous_collided_mesh_index != status.collided_mesh_index && status.previous_collided_mesh_index == -1)
        {
            status.status = true;
            status.previous_collided_mesh_index = status.collided_mesh_index;
        }
        else
        {
            status.status = false;
            status.previous_collided_mesh_index = status.collided_mesh_index;
        }

        TerrainAudio_Receiver* receiver = (TerrainAudio_Receiver*)parameters.get();
        // If the collided mesh index matches, plays the audio
        for (auto& entity : receiver->list_of_emitters)
        {
            
            TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)entity->parameters.get();
            TerrainAudio_Data_Emitter* data = (TerrainAudio_Data_Emitter*)entity->data->property_data.get();
            if (status.status && status.collided_mesh_index == data->mesh_index)
                entity->Play();
        }

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

//std::vector<TerrainAudio_Component::AUDIO_BUFFER> TerrainAudio_Component::Buffers()
//{
//    return buffers;
//}

/*----------------------------------------------TerrainAudio_Component GetComponentType()------------------------------------------------*/

COMPONENT_TYPE TerrainAudio_Component::GetComponentType()
{
    return data->type;
}
