#include "Base Classes/COMPONENT_CREATOR.h"
#include "OBB_TERRAIN.h"
#include "OBB_COLLIDER.h"
#include "TRANSFORM_3D.h"

// UNUSED



/*----------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------OBB_TERRAIN_DATA----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------OBB_TERRAIN_DATA CONSTRUCTOR--------------------------------------------*/

OBB_TERRAIN_DATA::OBB_TERRAIN_DATA()
{
    type = COMPONENT_TYPE::OBB_TERRAIN;
    audioData = std::dynamic_pointer_cast<AUDIO_DATA>(INSTANTIATE(COMPONENT_TYPE::AUDIO));
    colliderData = std::dynamic_pointer_cast<OBB_COLLIDER_DATA>(INSTANTIATE(COMPONENT_TYPE::OBB_COL));
}


/*----------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------OBB_TERRAIN---------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------OBB_TERRAIN CONSTRUCTOR-------------------------------------------------*/

OBB_TERRAIN::OBB_TERRAIN(GAMEOBJECT* g, COMPONENT_DATA* d)
{
    data = static_cast<OBB_TERRAIN_DATA*>(d);
    parent = g;
    collider = std::dynamic_pointer_cast<OBB_COLLIDER>(INSTANTIATE(g, data->colliderData));
}

/*----------------------------------------------OBB_TERRAIN Initialize()-------------------------------------------------*/

HRESULT OBB_TERRAIN::Initialize()
{
    if (data)
    {
        if (data->audioData->file_path != L"")
        {
            AUDIOENGINE::Instance()->Insert(data->audioData->name, data->audioData->file_path);
            audio = AUDIOENGINE::Instance()->Retrieve(data->audioData->name);
        }
        if (data->colliderData)
        {
            collider = std::make_shared<OBB_COLLIDER>(parent, data->colliderData.get());
            collider->Initialize();
        }
    }
    return S_OK;
}

/*----------------------------------------------OBB_TERRAIN Execute()-------------------------------------------------*/

void OBB_TERRAIN::Execute()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    collider->Execute(transform->TransformMatrix());
}

/*----------------------------------------------OBB_TERRAIN Render()-------------------------------------------------*/

void OBB_TERRAIN::Render()
{
    collider->Render();
}

/*----------------------------------------------OBB_TERRAIN UI()-------------------------------------------------*/

void OBB_TERRAIN::UI()
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
                AUDIOENGINE::Instance()->Insert(data->audioData->name, data->audioData->file_path);
                audio = AUDIOENGINE::Instance()->Retrieve(data->audioData->name);
            }
        }
        ImGui::TreePop();
    }
}

/*----------------------------------------------------------OBB_TERRAIN GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE OBB_TERRAIN::GetComponentType()
{
    return data->type;
}