#include "../Headers/TERRAIN_AUDIO.h"
#include "../Headers/MESH.h"
#include "../../Audio/AudioController.h"
#include "../../Engine/SoundEffect.h"
std::string types[] = { "Emitter", "Receiver" };

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TERRAIN_AUDIO_DATA Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TERRAIN_AUDIO_DATA Constructor------------------------------------------------*/

TERRAIN_AUDIO_DATA::TERRAIN_AUDIO_DATA()
{
    type = COMPONENT_TYPE::TERRAIN_AUDIO;

}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TERRAIN_AUDIO Class---------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TERRAIN_AUDIO Constructor------------------------------------------------*/

TERRAIN_AUDIO::TERRAIN_AUDIO(GAMEOBJECT* g, COMPONENT_DATA* d)
{
    data = static_cast<TERRAIN_AUDIO_DATA*>(d);
    parent = g;
}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT TERRAIN_AUDIO::Initialize()
{

    TERRAIN_AUDIO_DATA_EMITTER* emitter = dynamic_cast<TERRAIN_AUDIO_DATA_EMITTER*>(data->property_data.get());
    if (emitter)
    {
        AUDIOENGINE::Instance()->Insert(emitter->audio_data->name, emitter->audio_data->file_path);
        buffers.resize(32);
        for (auto& b : buffers)
        {
            b.buffer = std::make_shared<SoundEffect>(emitter->audio_data->file_path);
            b.buffer->Initialize();
            b.buffer->SetBuffer(AUDIOENGINE::Instance()->Retrieve(emitter->audio_data->name)->Buffer());

        }
    }




    return S_OK;
}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void TERRAIN_AUDIO::Execute()
{
    if (data->class_type != TERRAIN_AUDIO_PROPERTY::EMITTER)
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

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void TERRAIN_AUDIO::Render()
{

}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void TERRAIN_AUDIO::UI()
{
    if (ImGui::TreeNode("Terrain Audio"))
    {
        // Component is based on MESH component, so if not inserted, display nothing.
        MESH* mesh = GetComponent<MESH>();
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
            data->class_type = (TERRAIN_AUDIO_PROPERTY)ind;
            ind == (int)TERRAIN_AUDIO_PROPERTY::EMITTER ? data->property_data = std::make_shared<TERRAIN_AUDIO_DATA_EMITTER>() : data->property_data = std::make_shared<TERRAIN_AUDIO_DATA_RECEIVER>();
        }
        if (!data->property_data)
        {
            ImGui::TreePop();
            return;
        }


        switch (data->class_type)
        {
        case TERRAIN_AUDIO_PROPERTY::EMITTER:
        {
            if (ImGui::TreeNode("Emitter Properties"))
            {


                TERRAIN_AUDIO_DATA_EMITTER* current = static_cast<TERRAIN_AUDIO_DATA_EMITTER*>(data->property_data.get());
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
        case TERRAIN_AUDIO_PROPERTY::RECEIVER:
        {
            if (ImGui::TreeNode("Receiver Properties"))
            {
                MESH* mesh{ GetComponent<MESH>() };
                TERRAIN_AUDIO_DATA_RECEIVER* current = static_cast<TERRAIN_AUDIO_DATA_RECEIVER*>(data->property_data.get());
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

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Cycles through the buffer for unplayed audio and play it</para>
/// <para> バッファーの中にプレイしてないサウンドを探しプレイする</para>
/// </summary>
void TERRAIN_AUDIO::Play()
{
    TERRAIN_AUDIO_DATA_EMITTER* emitter = dynamic_cast<TERRAIN_AUDIO_DATA_EMITTER*>(data->property_data.get());
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

/*----------------------------------------------TERRAIN_AUDIO Property()------------------------------------------------*/

TERRAIN_AUDIO_PROPERTY TERRAIN_AUDIO::Property()
{
    return data->class_type;
}

/*----------------------------------------------TERRAIN_AUDIO Data()------------------------------------------------*/

TERRAIN_AUDIO_DATA* TERRAIN_AUDIO::Data()
{
    return data;
}

/*----------------------------------------------TERRAIN_AUDIO Buffers()------------------------------------------------*/

std::vector<TERRAIN_AUDIO::AUDIO_BUFFER> TERRAIN_AUDIO::Buffers()
{
    return buffers;
}

/*----------------------------------------------TERRAIN_AUDIO GetComponentType()------------------------------------------------*/

COMPONENT_TYPE TERRAIN_AUDIO::GetComponentType()
{
    return data->type;
}
