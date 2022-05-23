#include "EnvironmentalAudio.h"
#include "Base Classes/Component.h"
#include "Base Classes/ComponentCreator.h"
#include "../Engine/Audio.h"
#include "../Scenes/SceneManager.h"
#include "../Engine/Camera.h"
#include "../Engine/Input.h"
#include "../Engine/IMGUI.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PlayerController.h"
#include "OBBCollider.h"
#include "Transform3D.h"
#define POINTER_CAST std::dynamic_pointer_cast
std::string collider_types[3] = { "Sphere Collider", "Capsule Collider", "OBB Collider" };
enum class COL_TYPE
{
    SPHERE,
    CAPSULE,
    OBB
};
bool popup_e = {};
#pragma region LOCALIZED_COLLIDER_DATA

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LocalizedCollider_Data Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LocalizedCollider_Data Constructor--------------------------------------------------*/

LocalizedCollider_Data::LocalizedCollider_Data(COMPONENT_TYPE t)
{
    transform = std::dynamic_pointer_cast<Transform3D_Data>(ComponentCreator::Instance()->Instantiate(COMPONENT_TYPE::Transform3D_Component));
    transform->scale = { 1.0f, 1.0f ,1.0f };
    collider_data = ComponentCreator::Instance()->Instantiate(t);
}

/*--------------------------------------------LocalizedCollider_Data Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when creating from data. Links both data to its respective data </para>
/// <para> データロードするときに呼び出す。データをリンクする</para>
/// </summary>
/// <param name="t3d"> : TRANSFORD_3D_DATA</param>
/// <param name="cd"> : collider Data</param>
/// <returns></returns>
HRESULT LocalizedCollider_Data::Initialize(std::shared_ptr<ComponentData>t3d, std::shared_ptr<ComponentData>cd)
{
    transform = std::dynamic_pointer_cast<Transform3D_Data>(t3d);
    collider_data = cd;
    return S_OK;
}


#pragma endregion
#pragma region Localized_Collider
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LocalizedCollider_SubComponent Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LocalizedCollider_SubComponent Constructor--------------------------------------------------*/
/// <summary>
/// <para> Use this when creating a localized collider from existing data </para>
/// <para> 既存データを使ってLocalized Colliderを生成時に呼び出す</para>
/// </summary>
LocalizedCollider_SubComponent::LocalizedCollider_SubComponent(EnvironmentalAudio_Component* g, LocalizedCollider_Data* data)
{
    this->parent = g;
    this->data = data;
}
/// <summary>
/// <para> Use this when creating a localized collider</para>
/// <para> LOCALIZED_COLLIDERを生成時に呼び出す</para>
/// </summary>
LocalizedCollider_SubComponent::LocalizedCollider_SubComponent(GameObject* g, LocalizedCollider_Data* data)
{
    this->data = data;
    transform = POINTER_CAST<Transform3D_Component>(INSTANTIATE(g, data->transform));
    collider = POINTER_CAST<BaseColliderComponent>(INSTANTIATE(g, data->collider_data));
    transform->Initialize();
    collider->Initialize();
}

/*--------------------------------------------LocalizedCollider_SubComponent Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT LocalizedCollider_SubComponent::Initialize()
{
    transform = POINTER_CAST<Transform3D_Component>(INSTANTIATE(parent->parent, data->transform));
    collider = POINTER_CAST<BaseColliderComponent>(INSTANTIATE(parent->parent, data->collider_data));
    transform->Initialize();
    collider->Initialize();

    return S_OK;
}

/*--------------------------------------------LocalizedCollider_SubComponent Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
void LocalizedCollider_SubComponent::Execute()
{
    transform->Execute();
    XMMATRIX world_transform{ transform->TransformMatrix() * collider->GetParent()->GetComponent<Transform3D_Component>()->TransformMatrix() };
    collider->Execute(world_transform);
}

/*--------------------------------------------LocalizedCollider_SubComponent Render()--------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void LocalizedCollider_SubComponent::Render()
{
    collider->Render();
}

/*--------------------------------------------LocalizedCollider_SubComponent UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void LocalizedCollider_SubComponent::UI()
{
    if (ImGui::TreeNode("Local Collider"))
    {
        ImGui::DragFloat3("Translation", &data->transform->translation.x, 0.1f);

        collider->UI();
        ImGui::DragFloat("Minimum distance : ", &data->minimum_distance, 0.1f, 0.0f);

        ImGui::TreePop();
    }
}

/*--------------------------------------------LocalizedCollider_SubComponent Collider()--------------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>LocalizedCollider_SubComponent::GetCollider()
{
    return collider->Collider();
}

/*--------------------------------------------LocalizedCollider_SubComponent Transform()--------------------------------------------------*/

std::shared_ptr<Transform3D_Component>LocalizedCollider_SubComponent::Transform()
{
    return transform;
}

/*--------------------------------------------LocalizedCollider_SubComponent Data()--------------------------------------------------*/

LocalizedCollider_Data* LocalizedCollider_SubComponent::Data()
{
    return data;
}


#pragma endregion
#pragma region EnvironmentalAudio

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------EnvironmentalAudio_Component Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------EnvironmentalAudio_Component CreateLocalizedCollider()--------------------------------------------------*/
/// <summary>
/// <para> Creates a localized collider and insert it into the dataset </para>
/// <para> LOCALIZED_COLLIDERを生成し、データセットに登録 </para>
/// </summary>
void EnvironmentalAudio_Component::CreateLocalizedCollider(COMPONENT_TYPE t)
{
    data->collider_dataset.push_back(std::make_shared<LocalizedCollider_Data>(t));
    LocalizedCollider_Data* cur_data = data->collider_dataset.back().get();

    std::shared_ptr<ComponentData> cd = std::dynamic_pointer_cast<ComponentData>(cur_data->transform);
    colliders.push_back(std::make_shared<LocalizedCollider_SubComponent>(parent, cur_data));
    colliders.back()->data = data->collider_dataset.back().get();
}

/*--------------------------------------------EnvironmentalAudio_Component Constructor--------------------------------------------------*/

EnvironmentalAudio_Component::EnvironmentalAudio_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<EnvironmentalAudio_Data*>(data);
}

/*--------------------------------------------EnvironmentalAudio_Component Destructor--------------------------------------------------*/

EnvironmentalAudio_Component::~EnvironmentalAudio_Component()
{
    //if (audio->IsPlaying())
    //    audio->Stop();
    //// Delists the audio from the map
    //AudioEngine::Instance()->Delist(audio);
    //audio = {};
}

/*--------------------------------------------EnvironmentalAudio_Component Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT EnvironmentalAudio_Component::Initialize()
{
    if (data)
    {
        if (data->file_path != L"")
        {
            AudioEngine::Instance()->Insert(data->name, data->file_path);
            audio = AudioEngine::Instance()->Retrieve(data->name);
            audio->SetAudioEmitter(&audioEmitter);
            audio->Loop();
            audio->SetVolume(1.0f);
            audio->Play();
            //3DAudio emitter initialization
            Transform3D_Component* transform = GetComponent<Transform3D_Component>();
            audioEmitter.position = transform->GetTranslation();
            audio->SourceVoice()->GetFrequencyRatio(&audioEmitter.doppler_frequency);



            if (data->collider_dataset.size() > 0)
            {
                for (auto& d : data->collider_dataset) {
                    colliders.push_back(std::make_shared<LocalizedCollider_SubComponent>(this, d.get()));
                    colliders.back()->Initialize();
                    audioEmitter.size = colliders.back()->GetCollider()->GetSize();



                }
            }
        }

    }

    return S_OK;
}

/*--------------------------------------------EnvironmentalAudio_Component Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called every frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void EnvironmentalAudio_Component::Execute()
{
    if (!audio)
        return;
    if (colliders.size() < 1)
        return;
    for (auto& c : colliders)
        c->Execute();
    if (SceneManager::Instance()->PauseState())
    {
        audio->Stop();
        return;
    }



    // 3DAudio Emitter parameter updates
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    XMMATRIX matrix = transform->TransformMatrixQuaternion();
    Vector3 forward, top;
    forward.Load(matrix.r[2]);
    top.Load(matrix.r[1]);
    Vector3 closest_point = colliders[0]->collider->GetClosestPoint(Camera::Instance()->EyePosition());
    Vector3 last_pos{audioEmitter.position}, cur_pos{closest_point};
    Vector3 velocity = cur_pos - last_pos;



    audioEmitter.position = closest_point;
    audioEmitter.velocity = velocity;
    audioEmitter.vFrontVector = forward;
    audioEmitter.vTopVector = top;



}

/*--------------------------------------------EnvironmentalAudio_Component Render()--------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void EnvironmentalAudio_Component::Render()
{
    for (auto& c : colliders)
        c->Render();
    if(audio)
        audio->RenderDebug();

}

/*--------------------------------------------EnvironmentalAudio_Component UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void EnvironmentalAudio_Component::UI()
{
    if (ImGui::TreeNode("Environmental Audio"))
    {
        // File browser dialog
        ImGui::FileBrowser* browser{ IMGUI::Instance()->FileBrowser() };
        static bool fileOpenEA{};
        std::string preview{ "" };
        if (data->file_path.size() > 0) {
            for (auto& c : data->file_path)
            {
                preview.push_back(c);
            }
        }
        ImGui::InputText("Audio name", data->name, 256);
        ImGui::Text((char*)(preview.c_str()));
        if (ImGui::Button("Load Audio"))
        {
            browser->Open();
            browser->SetTitle("Open Audio");
            browser->SetTypeFilters({ ".wav", ".*" });
            fileOpenEA = true;

        }
         if (fileOpenEA)
         {
             browser->Display();
             if (browser->HasSelected())
             {
                 data->file_path = browser->GetSelected().wstring();
                 fileOpenEA = false;


                 std::filesystem::path path(data->file_path);
                 std::filesystem::path name(path.filename());
                 std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
                 AudioEngine::Instance()->Insert(data->name, full_path);
                 data->file_path = full_path;
                 audio = AudioEngine::Instance()->Retrieve(data->name);
                 for (auto& c : data->file_path)
                 {
                     preview.push_back(c);
                 }
                 audio->SetAudioEmitter(&audioEmitter);

                 browser->Close();
             }
         }

         // Properties
         ImGui::DragFloat("Maximum volume", &data->maximum_volume, 0.05f, 0.0f, 1.0f);

        if (ImGui::Button("Play"))
            audio->Play();
        else if (ImGui::Button("Stop") && audio->IsPlaying())
            audio->Stop();
        ImGui::ListBoxHeader("Colliders");
        static int index = 0;
        for (int ind = 0; ind < colliders.size(); ++ind)
        {
            std::string cur_c = "Collider " + std::to_string(ind);
            if (ImGui::Selectable(cur_c.c_str()))
            {
                index = ind;
                break;
            }
        }
        ImGui::ListBoxFooter();

        if (ImGui::Button("Insert Collider"))
        {
            popup_e = true;

        }
        if (popup_e)
        {
            ImGui::OpenPopup("Choose collider type");
            if (ImGui::BeginPopup("Choose collider type"))
            {
                ImGui::ListBoxHeader("Select Collider Type");
                int ind{};
                COMPONENT_TYPE s_type{};
                bool isClick = false;
                for (auto& s : collider_types)
                {
                    bool selected{};
                    if (ImGui::Selectable(s.c_str(), selected))
                    {
                        switch ((COL_TYPE)ind)
                        {
                        case COL_TYPE::SPHERE:
                            s_type = COMPONENT_TYPE::SPHERE_COL;
                            break;
                        case COL_TYPE::CAPSULE:
                            s_type = COMPONENT_TYPE::CAPSULE_COL;
                            break;
                        case COL_TYPE::OBB:
                            s_type = COMPONENT_TYPE::OBB_COL;
                            break;
                        }
                        CreateLocalizedCollider(s_type);
                        popup_e = false;
                        break;
                    }
                    ++ind;
                }

                ImGui::ListBoxFooter();

                ImGui::EndPopup();
            }

        }

        if (colliders.size() > 0)
            colliders[index]->UI();




        ImGui::TreePop();
    }

}

/*--------------------------------------------EnvironmentalAudio_Component Colliders()--------------------------------------------------*/

std::vector<std::shared_ptr<LocalizedCollider_SubComponent>>EnvironmentalAudio_Component::Colliders()
{
    return colliders;
}

/*--------------------------------------------EnvironmentalAudio_Component GetData()--------------------------------------------------*/

EnvironmentalAudio_Data* EnvironmentalAudio_Component::GetData()
{
    return data;
}


/*--------------------------------------------EnvironmentalAudio_Component GetComponentType()--------------------------------------------------*/
COMPONENT_TYPE EnvironmentalAudio_Component::GetComponentType()
{
    return data->type;
}

#pragma endregion