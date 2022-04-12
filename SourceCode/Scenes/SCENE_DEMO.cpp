#include "SCENE_DEMO.h"
#include "../GAMEOBJECT.h"
#include "../STAGE_UI.h"
#include "../Audio/AudioController.h"
#include "../Components\Base Classes\DATAMANAGER.h"
#include "../Engine/IMGUI.h"


/*--------------------------------------------------SCENE_DEMO Insert()-------------------------------------*/

HRESULT SCENE_DEMO::Initialize()
{
    DATAMANAGER::Instance()->InsertAndInitialize();
    AudioController::Instance()->Initialize();
    return S_OK;
}

/*--------------------------------------------------SCENE_DEMO Execute()-------------------------------------*/

void SCENE_DEMO::Execute()
{
    IMGUI::Instance()->Execute();
    AudioController::Instance()->Execute();
    if(state)
        GAMEOBJECTS->Execute();;
}

/*--------------------------------------------------SCENE_DEMO Render()-------------------------------------*/

void SCENE_DEMO::Render()
{
    GAMEOBJECTS->Render();  
    STAGE_UI::Instance()->PlayUI();
    AudioController::Instance()->DebugUI();
    IMGUI::Instance()->Render();

}

/*--------------------------------------------------SCENE_DEMO Finalize()-------------------------------------*/

//void SCENE_DEMO::Finalize()
//{
//    GAMEOBJECTS->Finalize();
//}
//
/*--------------------------------------------------SCENE_DEMO Pause()-------------------------------------*/

void SCENE_DEMO::Pause()
{
    state = false;
}

/*--------------------------------------------------SCENE_DEMO Resume()-------------------------------------*/

void SCENE_DEMO::Resume()
{
    state = true;
}

/*--------------------------------------------------SCENE_DEMO GetState()-------------------------------------*/

bool SCENE_DEMO::GetState()
{
    return state;
}

