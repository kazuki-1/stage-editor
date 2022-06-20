#include "SceneDemo.h"
#include "../GAMEOBJECT.h"
#include "../STAGE_UI.h"
#include "../Audio/AudioController.h"
#include "../Components/Base Classes/DataManager.h"
#include "../DialogueController.h"
#include "../Engine/IMGUI.h"
#include "../DialogueController.h"
#include "../Engine/Audio.h"
#include "../Engine/AudioEngine.h"

/*--------------------------------------------------SceneDemo Insert()-------------------------------------*/

HRESULT SceneDemo::Initialize()
{
    DialogueController::Instance()->Initialize(L"./Data/Images/text_box.png", {50, 600}, {800, 200});
    DataManager::Instance()->InsertAndInitialize();
    AudioController::Instance()->Initialize();
    return S_OK;
}

/*--------------------------------------------------SceneDemo Execute()-------------------------------------*/

void SceneDemo::Execute()
{
    IMGUI::Instance()->Execute();
    AudioController::Instance()->Execute();
    if(state)
        GAMEOBJECTS->Execute();;
}

/*--------------------------------------------------SceneDemo Render()-------------------------------------*/

void SceneDemo::Render()
{
    GAMEOBJECTS->Render();  
    STAGE_UI::Instance()->PlayUI();
    // For debugging
    //AudioController::Instance()->DebugUI();
    DialogueController::Instance()->Render();
    IMGUI::Instance()->Render();

}

/*--------------------------------------------------SceneDemo Finalize()-------------------------------------*/

void SceneDemo::Finalize()
{
    GAMEOBJECTS->Finalize();
    AudioEngine::Instance()->CleanupObstructors();
    DataManager::Instance()->Finalize();

}

/*--------------------------------------------------SceneDemo Pause()-------------------------------------*/

void SceneDemo::Pause()
{
    state = false;
}

/*--------------------------------------------------SceneDemo Resume()-------------------------------------*/

void SceneDemo::Resume()
{
    state = true;
}

/*--------------------------------------------------SceneDemo GetState()-------------------------------------*/

bool SceneDemo::GetState()
{
    return state;
}

