#include "Base Classes/COMPONENT.h"
#include "Base Classes/COMPONENT_CREATOR.h"
#include "../Engine/Audio.h"
#include "../Engine/Input.h"
#include "../Engine/IMGUI.h"
#include "ENVIRONMENTAL_AUDIO.h"
#include "SPHERE_COLLIDER.h"
#include "CAPSULE_COLLIDER.h"
#include "OBB_COLLIDER.h"
#include "TRANSFORM_3D.h"
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
/*--------------------------------------------LOCALIZED_COLLIDER_DATA Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LOCALIZED_COLLIDER_DATA Constructor--------------------------------------------------*/

LOCALIZED_COLLIDER_DATA::LOCALIZED_COLLIDER_DATA(COMPONENT_TYPE t)
{
    transform = std::dynamic_pointer_cast<TRANSFORM_3D_DATA>(COMPONENT_CREATOR::Instance()->Instantiate(COMPONENT_TYPE::TRANSFORM_3D));
    transform->scale = { 1.0f, 1.0f ,1.0f };
    collider_data = COMPONENT_CREATOR::Instance()->Instantiate(t);
}

/*--------------------------------------------LOCALIZED_COLLIDER_DATA Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when creating from data. Links both data to its respective data </para>
/// <para> データロードするときに呼び出す。データをリンクする</para>
/// </summary>
/// <param name="t3d"> : TRANSFORD_3D_DATA</param>
/// <param name="cd"> : collider Data</param>
/// <returns></returns>
HRESULT LOCALIZED_COLLIDER_DATA::Initialize(std::shared_ptr<COMPONENT_DATA>t3d, std::shared_ptr<COMPONENT_DATA>cd)
{
    transform = std::dynamic_pointer_cast<TRANSFORM_3D_DATA>(t3d);
    collider_data = cd;
    return S_OK;
}


#pragma endregion
#pragma region LOCALIZED_COLLIDER
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LOCALIZED_COLLIDER Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------LOCALIZED_COLLIDER Constructor--------------------------------------------------*/
/// <summary>
/// <para> Use this when creating a localized collider from existing data </para>
/// <para> 既存データを使ってLocalized Colliderを生成時に呼び出す</para>
/// </summary>
LOCALIZED_COLLIDER::LOCALIZED_COLLIDER(ENVIRONMENTAL_AUDIO* g, LOCALIZED_COLLIDER_DATA* data)
{
    this->parent = g;
    this->data = data;
}
/// <summary>
/// <para> Use this when creating a localized collider</para>
/// <para> LOCALIZED_COLLIDERを生成時に呼び出す</para>
/// </summary>
LOCALIZED_COLLIDER::LOCALIZED_COLLIDER(GAMEOBJECT* g, LOCALIZED_COLLIDER_DATA* data)
{
    this->data = data;
    transform = POINTER_CAST<TRANSFORM_3D>(INSTANTIATE(g, data->transform));
    collider = POINTER_CAST<BASE_COLLIDER_COMPONENT>(INSTANTIATE(g, data->collider_data));
    transform->Initialize();
    collider->Initialize();
}

/*--------------------------------------------LOCALIZED_COLLIDER Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT LOCALIZED_COLLIDER::Initialize()
{
    transform = POINTER_CAST<TRANSFORM_3D>(INSTANTIATE(parent->parent, data->transform));
    collider = POINTER_CAST<BASE_COLLIDER_COMPONENT>(INSTANTIATE(parent->parent, data->collider_data));
    transform->Initialize();
    collider->Initialize();

    return S_OK;
}

/*--------------------------------------------LOCALIZED_COLLIDER Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
void LOCALIZED_COLLIDER::Execute()
{
    transform->Execute();
    XMMATRIX world_transform{ transform->TransformMatrix() * collider->Parent()->GetComponent<TRANSFORM_3D>()->TransformMatrix() };
    collider->Execute(world_transform);
}

/*--------------------------------------------LOCALIZED_COLLIDER Render()--------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void LOCALIZED_COLLIDER::Render()
{
    collider->Render();
}

/*--------------------------------------------LOCALIZED_COLLIDER UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void LOCALIZED_COLLIDER::UI()
{
    if (ImGui::TreeNode("Local Collider"))
    {
        ImGui::DragFloat3("Translation", &data->transform->translation.x, 0.1f);

        collider->UI();
        ImGui::DragFloat("Minimum distance : ", &data->minimum_distance, 0.1f, 0.0f);

        ImGui::TreePop();
    }
}

/*--------------------------------------------LOCALIZED_COLLIDER Collider()--------------------------------------------------*/

std::shared_ptr<BASE_COLLIDER_COMPONENT>LOCALIZED_COLLIDER::Collider()
{
    return collider;
}

/*--------------------------------------------LOCALIZED_COLLIDER Transform()--------------------------------------------------*/

std::shared_ptr<TRANSFORM_3D>LOCALIZED_COLLIDER::Transform()
{
    return transform;
}

/*--------------------------------------------LOCALIZED_COLLIDER Data()--------------------------------------------------*/

LOCALIZED_COLLIDER_DATA* LOCALIZED_COLLIDER::Data()
{
    return data;
}


#pragma endregion
#pragma region ENVIRONMENTAL_AUDIO

/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------ENVIRONMENTAL_AUDIO Class--------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------ENVIRONMENTAL_AUDIO CreateLocalizedCollider()--------------------------------------------------*/
/// <summary>
/// <para> Creates a localized collider and insert it into the dataset </para>
/// <para> LOCALIZED_COLLIDERを生成し、データセットに登録 </para>
/// </summary>
void ENVIRONMENTAL_AUDIO::CreateLocalizedCollider(COMPONENT_TYPE t)
{
    data->collider_dataset.push_back(std::make_shared<LOCALIZED_COLLIDER_DATA>(t));
    LOCALIZED_COLLIDER_DATA* cur_data = data->collider_dataset.back().get();

    std::shared_ptr<COMPONENT_DATA> cd = std::dynamic_pointer_cast<COMPONENT_DATA>(cur_data->transform);
    colliders.push_back(std::make_shared<LOCALIZED_COLLIDER>(parent, cur_data));
    colliders.back()->data = data->collider_dataset.back().get();
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO Constructor--------------------------------------------------*/

ENVIRONMENTAL_AUDIO::ENVIRONMENTAL_AUDIO(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<ENVIRONMENTAL_AUDIO_DATA*>(data);
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO Initialize()--------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT ENVIRONMENTAL_AUDIO::Initialize()
{
    if (data)
    {
        if (data->file_path != L"")
        {
            AUDIOENGINE::Instance()->Insert(data->name, data->file_path);
            audio = AUDIOENGINE::Instance()->Retrieve(data->name);
            audio->Loop();
            if (data->collider_dataset.size() > 0)
            {
                for (auto& d : data->collider_dataset) {
                    colliders.push_back(std::make_shared<LOCALIZED_COLLIDER>(this, d.get()));
                    colliders.back()->Initialize();

                }
            }
        }

    }
    return S_OK;
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called every frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void ENVIRONMENTAL_AUDIO::Execute()
{
    for (auto& c : colliders)
    {
        c->Execute();
    }
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO Render()--------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void ENVIRONMENTAL_AUDIO::Render()
{
    for (auto& c : colliders)
        c->Render();
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void ENVIRONMENTAL_AUDIO::UI()
{
    if (ImGui::TreeNode("Environmental Audio"))
    {
        ImGui::InputText("Audio name", data->name, 256);
        IMGUI::Instance()->InputText("Audio File Path", &data->file_path);
        if (ImGui::Button("Load Audio"))
        {
            //data->name += "##" + std::to_string(data->id);
            std::filesystem::path path(data->file_path);
            std::filesystem::path name(path.filename());
            std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
            AUDIOENGINE::Instance()->Insert(data->name, full_path);
            data->file_path = full_path;
            audio = AUDIOENGINE::Instance()->Retrieve(data->name);
        }
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

/*--------------------------------------------ENVIRONMENTAL_AUDIO Colliders()--------------------------------------------------*/

std::vector<std::shared_ptr<LOCALIZED_COLLIDER>>ENVIRONMENTAL_AUDIO::Colliders()
{
    return colliders;
}

/*--------------------------------------------ENVIRONMENTAL_AUDIO GetComponentType()--------------------------------------------------*/
COMPONENT_TYPE ENVIRONMENTAL_AUDIO::GetComponentType()
{
    return data->type;
}

#pragma endregion