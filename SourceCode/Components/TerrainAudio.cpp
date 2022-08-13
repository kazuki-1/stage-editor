#include "../Audio/AudioController.h"
#include "../Engine/SoundEffect.h"
#include "PlayerController.h"
#include "Transform3D.h"
#include "TerrainAudio.h"
#include "Mesh.h"
#include "../Engine/AudioEngine.h"
//#include "OBBCollider.h"

std::string types[] = { "Emitter", "Receiver" };
#define MAX_TIME 120


/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TerrainAudio_Emitter Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TerrainAudio_Emitter Initialize()------------------------------------------------*/

void TerrainAudio_Emitter::Initialize(TerrainAudio_Component* parent, TerrainAudio_Data_Base* data)
{
    TerrainAudio_Data_Emitter* emitter_data = dynamic_cast<TerrainAudio_Data_Emitter*>(data);
    collider = std::make_shared<OBBCollider_Component>(parent->GetParent(), emitter_data->collider_data.get());
    collider->Initialize();
}

/*----------------------------------------------TerrainAudio_Emitter Finalize()------------------------------------------------*/

void TerrainAudio_Emitter::Finalize()
{
    if (collider)
        collider->Finalize();
}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TerrainAudio_Data Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TerrainAudio_Data Constructor------------------------------------------------*/

TerrainAudio_Data::TerrainAudio_Data()
{
    type = COMPONENT_TYPE::TerrainAudio_Component;

}

/*--------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TerrainAudio_Component Class---------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
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
            internal_component = std::make_shared<TerrainAudio_Emitter>();
            internal_component->Initialize(this, data);
            TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)internal_component.get();

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

    {
        TerrainAudio_Data_Receiver* data = dynamic_cast<TerrainAudio_Data_Receiver*>(this->data->property_data.get());
        if (data)
        {
            internal_component = std::make_shared<TerrainAudio_Receiver>();
        }
    }


    return S_OK;
}

/*----------------------------------------------TerrainAudio_Component Execute()------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void TerrainAudio_Component::Execute()
{
    ExecuteEmitter();
    ExecuteReceiver();
}

/*----------------------------------------------TerrainAudio_Component ExecuteUI()------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void TerrainAudio_Component::ExecuteUI()
{
    if (data->class_type != TerrainAudio_Property::EMITTER)
        return;

    TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)internal_component.get();

    emitter->collider->Execute();
}


/*----------------------------------------------TerrainAudio_Component Render()------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void TerrainAudio_Component::Render()
{
    TerrainAudio_Emitter* emitter = dynamic_cast<TerrainAudio_Emitter*>(internal_component.get());

    if (emitter)
        emitter->collider->Render();
}

/*----------------------------------------------TerrainAudio_Component UI()------------------------------------------------*/
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

            if (data->class_type == TerrainAudio_Property::EMITTER)
            {
                data->property_data = std::make_shared<TerrainAudio_Data_Emitter>();
                internal_component = std::make_shared<TerrainAudio_Emitter>();
                internal_component->Initialize(this, data->property_data.get());
            }
            else 
            {
                data->property_data = std::make_shared<TerrainAudio_Data_Receiver>();
                internal_component = std::make_shared<TerrainAudio_Receiver>();
            }

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
                TerrainAudio_Data_Emitter* emitter_data = static_cast<TerrainAudio_Data_Emitter*>(data->property_data.get());
                TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)internal_component.get();
                static bool fileOpenTA{};
                ImGui::FileBrowser* browser{ IMGUI::Instance()->FileBrowser() };
                static int cur_id{};
                static std::string previewTA{};

                ImGui::InputText("Audio Name", emitter_data->audio_data->name, 256);
                //ImGui::Text(std::to_string(emitter->mesh_index).c_str());
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
                        emitter_data->audio_data->file_path = browser->GetSelected().wstring();
                        fileOpenTA = false;

                        // Changes the file path to be detected in a local path
                        std::filesystem::path path(emitter_data->audio_data->file_path);
                        std::filesystem::path name(path.filename());
                        std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
                        AudioEngine::Instance()->Insert(emitter_data->audio_data->name, full_path);
                        emitter_data->audio_data->file_path = full_path;

                        // Converts the file path from wstring to string
                        for (auto& c : emitter_data->audio_data->file_path)
                        {
                            previewTA.push_back(c);
                        }

                        browser->Close();
                    }
                }

                if (emitter->collider)
                    emitter->collider->UI();

                ImGui::TreePop();
            }
            break;
        }
        case TerrainAudio_Property::RECEIVER:
        {
            Mesh_Component* mesh = GetComponent<Mesh_Component>();
            if (!mesh || !mesh->Model())
            {
                ImGui::Text("Mesh Component required!");
                ImGui::TreePop();
                return;
            }

            if (ImGui::TreeNode("Receiver Properties"))
            {
                // Parameter preparation
                Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
                TerrainAudio_Data_Receiver* receiver_data = static_cast<TerrainAudio_Data_Receiver*>(data->property_data.get());
                if (mesh)
                {
                    MODEL_RESOURCES* resource = mesh->Model()->Resource().get();
                    static int cur_anim_index{};
                    cur_anim_index = min(max(0, cur_anim_index), resource->Animations.size());

                    // Animation selection
                    if (ImGui::BeginCombo("Animations", resource->Animations[cur_anim_index].Name.c_str()))
                    {
                        int ind{};
                        for (auto& anim : resource->Animations)
                        {
                            if (ImGui::Selectable(anim.Name.c_str()))
                                cur_anim_index = ind;
                            ++ind;
                        }
                        ImGui::EndCombo();
                    }

                    // Keyframe selection
                    if (ImGui::BeginCombo("Frames", "Select frame"))
                    {
                        int ind{};
                        for (auto& frame : resource->Animations[cur_anim_index].Keyframes)
                        {
                            std::string cur_frame{ "Frame " + std::to_string(ind) };
                            if (ImGui::Selectable(cur_frame.c_str()))
                                receiver_data->parameters.push_back(TerrainAudio_Data_Receiver::Parameter({ cur_anim_index, ind }));
                            ++ind;

                        }
                        ImGui::EndCombo();

                    }

                    // Show list of parameters
                    ImGui::ListBoxHeader("Receivers");
                    int ind{};
                    static TerrainAudio_Data_Receiver::Parameter* sel_p{};
                    for (auto& param : receiver_data->parameters)
                    {
                        std::string preview_param{ "Param " + std::to_string(ind) };
                        if (ImGui::Selectable(preview_param.c_str()))
                            sel_p = &receiver_data->parameters[ind];
                        ++ind;
                    }
                    ImGui::ListBoxFooter();



                    if (sel_p)
                    {
                        std::string preview_take = { "Animation Take : " + std::to_string(sel_p->animation_take) };
                        std::string preview_anim_name = "Animation Name : " + resource->Animations[sel_p->animation_take].Name;
                        std::string preview_frame = { "Animation frame : " + std::to_string(sel_p->animation_frame) };
                        ImGui::Text(preview_take.c_str());
                        ImGui::Text(preview_anim_name.c_str());
                        ImGui::Text(preview_frame.c_str());

                    }


                }
                ImGui::TreePop();
            }
            break;
        }


        }

        ImGui::TreePop();
    }
}

/*----------------------------------------------TerrainAudio_Component Finalize()------------------------------------------------*/
/// <summary>
/// Called when component is destroyed
/// </summary>
void TerrainAudio_Component::Finalize()
{
    if(internal_component)
    internal_component->Finalize();
}

/*----------------------------------------------TerrainAudio_Component Play()------------------------------------------------*/
/// <summary>
/// <para> Cycles through the buffer for unplayed audio and play it</para>
/// <para> バッファーの中にプレイしてないサウンドを探しプレイする</para>
/// </summary>
void TerrainAudio_Component::Play()
{
    if (data->class_type != TerrainAudio_Property::EMITTER)
        return;
    TerrainAudio_Emitter* emitter = static_cast<TerrainAudio_Emitter*>(internal_component.get());
    if (!emitter)
        return;
    bool isDucking{ AudioController::Instance()->IsDucking() };

    // Cycle through the buffers to look for one that isn't being used 
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
        TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)internal_component.get();
        emitter->collider->Execute();
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

    if (data->class_type != TerrainAudio_Property::RECEIVER)
        return;

    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    Mesh_Component* mesh = GetComponent<Mesh_Component>();
    MODEL* model = mesh->Model().get();
    std::vector<TerrainAudio_Component*>emitters;
    TerrainAudio_Data_Receiver* receiver_data = (TerrainAudio_Data_Receiver*)data->property_data.get();
    TerrainAudio_Receiver* receiver = (TerrainAudio_Receiver*)internal_component.get();
    receiver->timer++;

    int cur_anim_index{ model->CurrentTake() };
    int cur_anim_frame{ model->CurrentFrame() };


    bool perform_check{};
    // Only perform collision check when model is at target animation frame
    for (auto& parameter : receiver_data->parameters)
    {
        if (cur_anim_index != parameter.animation_take)
            continue;

        if (cur_anim_frame != parameter.animation_frame)
            continue;

        perform_check = true;
        break;
    }
    if (!perform_check)
        return;

    // Searches all gameObjects for emitters
    for (auto& gameObject : GameObjectManager::Instance()->GetGameObjects())
    {
        // Skips this object
        if (gameObject.second.get() == GetParent())
            continue;

        for (auto& emitter : gameObject.second->GetListOfComponents<TerrainAudio_Component>())
        {
            if (emitter->data->class_type == TerrainAudio_Property::EMITTER)
                emitters.push_back(emitter);

        }
    }


	for (auto& em : emitters)
	{
        TerrainAudio_Emitter* emitter = (TerrainAudio_Emitter*)em->internal_component.get();

        bool collided = emitter->collider->Collide(transform->GetTranslation());
        if (collided && receiver->timer > 3)
        {
            receiver->timer = 0;
            em->Play();
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
