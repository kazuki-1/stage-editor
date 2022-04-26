#include "SceneUI.h"
#include "../Components/Base Classes/ComponentCreator.h"
#include "../Engine/DEBUG_MANAGER.h"
#include "../Engine/IMGUI.h"
#include "../GAMEOBJECT.h"
#include "../Engine/LIGHTING.h"
#include "../STAGE_UI.h"
#include "../Components/Base Classes/DataManager.h"
#include "../Engine/Text.h"


/*---------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------SceneUI Class------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------SceneUI Initialize()-----------------------------------------------------------*/

HRESULT SceneUI::Initialize()
{

    // Loads pre existing data if it exists
    std::string cur_scene_path{ DataManager::Instance()->GetCurrentScenePath() };
    if (cur_scene_path == "")
        DataManager::Instance()->InsertAndInitialize();
    else
        DataManager::Instance()->Load(cur_scene_path);

    return S_OK;

}

/*--------------------------------------SceneUI Execute()-----------------------------------------------------------*/

void SceneUI::Execute()
{
    IMGUI::Instance()->Execute();
    LightingManager::Instance()->RenderUI();
    STAGE_UI::Instance()->RenderUI();

}

/*--------------------------------------SceneUI Render()-----------------------------------------------------------*/

void SceneUI::Render()
{
    STAGE_UI::Instance()->Render();
    IMGUI::Instance()->Render();
}
