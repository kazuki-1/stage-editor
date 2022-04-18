#include "Base Classes/ComponentCreator.h"
#include "OBBTerrain.h"
#include "OBBCollider.h"
#include "Transform3D.h"

// UNUSED



/*----------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------OBBTerrain_Data----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------OBBTerrain_Data CONSTRUCTOR--------------------------------------------*/

OBBTerrain_Data::OBBTerrain_Data()
{
    type = COMPONENT_TYPE::OBB_TERRAIN;
    audioData = std::dynamic_pointer_cast<AudioData>(INSTANTIATE(COMPONENT_TYPE::AUDIO));
    colliderData = std::dynamic_pointer_cast<OBBCollider_Data>(INSTANTIATE(COMPONENT_TYPE::OBB_COL));
}


/*----------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------OBBTerrain_Component---------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------OBBTerrain_Component CONSTRUCTOR-------------------------------------------------*/

OBBTerrain_Component::OBBTerrain_Component(GameObject* g, ComponentData* d)
{
    data = static_cast<OBBTerrain_Data*>(d);
    parent = g;
    collider = std::dynamic_pointer_cast<OBBCollider_Component>(INSTANTIATE(g, data->colliderData));
}

/*----------------------------------------------OBBTerrain_Component Initialize()-------------------------------------------------*/

HRESULT OBBTerrain_Component::Initialize()
{
    if (data)
    {
        if (data->audioData->file_path != L"")
        {
            AudioEngine::Instance()->Insert(data->audioData->name, data->audioData->file_path);
            audio = AudioEngine::Instance()->Retrieve(data->audioData->name);
        }
        if (data->colliderData)
        {
            collider = std::make_shared<OBBCollider_Component>(parent, data->colliderData.get());
            collider->Initialize();
        }
    }
    return S_OK;
}

/*----------------------------------------------OBBTerrain_Component Execute()-------------------------------------------------*/

void OBBTerrain_Component::Execute()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    collider->Execute(transform->TransformMatrix());
}

/*----------------------------------------------OBBTerrain_Component Render()-------------------------------------------------*/

void OBBTerrain_Component::Render()
{
    collider->Render();
}

/*----------------------------------------------OBBTerrain_Component UI()-------------------------------------------------*/

void OBBTerrain_Component::UI()
{
    if (ImGui::TreeNode("OBB Terrain"))
    {
        ImGui::Checkbox("Overlap", &data->overlap);
        ImGui::Checkbox("Trigger audio", &data->triggerAudio);
        collider->UI();
        if (data->triggerAudio)
        {
            ImGui::InputText("Name", data->audioData->name, 256);
            IMGUI::Instance()->InputText("Audio Path", &data->audioData->file_path);
            if (ImGui::Button("Load Audio"))
            {
                std::wstring dir{ L"./Data/Audio/Terrain/" };
                std::filesystem::path path(data->audioData->file_path);
                path = path.filename();
                dir += path.filename().wstring();
                data->audioData->file_path = dir;
                AudioEngine::Instance()->Insert(data->audioData->name, data->audioData->file_path);
                audio = AudioEngine::Instance()->Retrieve(data->audioData->name);
            }
        }
        ImGui::TreePop();
    }
}

/*----------------------------------------------------------OBBTerrain_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE OBBTerrain_Component::GetComponentType()
{
    return data->type;
}