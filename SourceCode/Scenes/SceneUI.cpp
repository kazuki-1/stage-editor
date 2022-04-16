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
    DataManager::Instance()->InsertAndInitialize();
    ComponentCreator::Instance()->Initialize();
    DEBUG_MANAGER::Instance()->Initialize();
    TextManager::Instance()->SetCurrentFont("Font1");
    return S_OK;

}

/*--------------------------------------SceneUI Execute()-----------------------------------------------------------*/

void SceneUI::Execute()
{
    IMGUI::Instance()->Execute();
    LIGHTINGMANAGER::Instance()->RenderUI();
    STAGE_UI::Instance()->RenderUI();

}

/*--------------------------------------SceneUI Render()-----------------------------------------------------------*/

void SceneUI::Render()
{
    STAGE_UI::Instance()->Render();
    IMGUI::Instance()->Render();
}
