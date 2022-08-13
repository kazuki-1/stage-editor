#include "SceneUI.h"
#include "../Components/Base Classes/ComponentCreator.h"
#include "../Engine/DEBUG_MANAGER.h"
#include "../Engine/IMGUI.h"
#include "../GAMEOBJECT.h"
#include "../Engine/LIGHTING.h"
#include "../STAGE_UI.h"
#include "../Components/Base Classes/DataManager.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"
#include "../Engine/COLLISION.h"
#include "../Engine/AudioEngine.h"


/*---------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------SceneUI Class------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------SceneUI Initialize()-----------------------------------------------------------*/

HRESULT SceneUI::Initialize()
{
    // Stops any audio that is playing
    for (auto& audio : AudioEngine::Instance()->Audios())
    {
        if (audio.second->IsPlaying())
            audio.second->Stop();
    }

    // Loads pre existing data if it exists
    DataManager::Instance()->InsertAndInitialize();

    return S_OK;

}

/*--------------------------------------SceneUI Execute()-----------------------------------------------------------*/

void SceneUI::Execute()
{
    IMGUI::Instance()->Execute();
    STAGE_UI::Instance()->RenderUI();

}

/*--------------------------------------SceneUI Render()-----------------------------------------------------------*/

void SceneUI::Render()
{
    STAGE_UI::Instance()->Render();
    ShaderManager::Instance()->Render();
    IMGUI::Instance()->Render();

}

/*--------------------------------------SceneUI Finalize()-----------------------------------------------------------*/

void SceneUI::Finalize()
{
    GameObjectManager::Instance()->Finalize();
    AudioEngine::Instance()->CleanupObstructors();
    COLLIDERS::RAYCAST_MANAGER::Instance()->Finalize();
    STAGE_UI::Instance()->Finalize();
}