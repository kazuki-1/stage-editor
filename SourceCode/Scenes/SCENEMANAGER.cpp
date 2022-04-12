#include "SCENEMANAGER.h"
#include "SCENE_UI.h"
#include "SCENE_DEMO.h"



/*--------------------------------------------------SCENEMANAGER Initialize()-------------------------------------*/

HRESULT SCENEMANAGER::Initialize()
{
    Insert(ScenesEnum::Scene_UI, std::make_shared<SCENE_UI>());
    Insert(ScenesEnum::Scene_Demo, std::make_shared<SCENE_DEMO>());
    ChangeScene(ScenesEnum::Scene_UI);
    return S_OK;
}

/*--------------------------------------------------SCENEMANAGER Execute()-------------------------------------*/

void SCENEMANAGER::Execute()
{
    if (!cur_Scene)
        return;
    cur_Scene->Execute();
}

/*--------------------------------------------------SCENEMANAGER Render()-------------------------------------*/

void SCENEMANAGER::Render()
{
    if (!cur_Scene)
        return;
    cur_Scene->Render();
}

/*--------------------------------------------------SCENEMANAGER Finalize()-------------------------------------*/

void SCENEMANAGER::Finalize()
{
    if (!cur_Scene)
        return;
    cur_Scene->Finalize();
    cur_Scene = nullptr;
}

/*--------------------------------------------------SCENEMANAGER ChangeScene()-------------------------------------*/

void SCENEMANAGER::ChangeScene(ScenesEnum name)
{
    if (cur_Scene)
        Finalize();
    cur_Scene = scenes.find(name)->second;
    cur_Scene->Initialize();
    current_Enum = name;
}

/*--------------------------------------------------SCENEMANAGER Insert()-------------------------------------*/

void SCENEMANAGER::Insert(ScenesEnum name, std::shared_ptr<SCENE>s)
{
    scenes.emplace(name, s);
}

/*--------------------------------------------------SCENEMANAGER Play()-------------------------------------*/
/// <summary>
/// Transition into SceneDemo
/// </summary>
void SCENEMANAGER::Play()
{
    // Changes the scene if in UI
    if (current_Enum == ScenesEnum::Scene_UI)
        ChangeScene(ScenesEnum::Scene_Demo);

    // Resumes the scene if demo scene is paused
    if (current_Enum == ScenesEnum::Scene_Demo && ((SCENE_DEMO*)cur_Scene.get())->GetState() == false)
    {
        ((SCENE_DEMO*)cur_Scene.get())->Resume();
    }
}

/*--------------------------------------------------SCENEMANAGER Pause()-------------------------------------*/
/// <summary>
/// Pauses the demo scene
/// </summary>
void SCENEMANAGER::Pause()
{
    if (current_Enum == ScenesEnum::Scene_Demo && ((SCENE_DEMO*)cur_Scene.get())->GetState() == true)
        ((SCENE_DEMO*)cur_Scene.get())->Pause();
}

/*--------------------------------------------------SCENEMANAGER Stop()-------------------------------------*/
/// <summary>
/// Stops the demo scene and transition back into SceneUI
/// </summary>
void SCENEMANAGER::Stop()
{
    if (current_Enum == ScenesEnum::Scene_Demo)
        ChangeScene(ScenesEnum::Scene_UI);
}




/*--------------------------------------------------SCENEMANAGER Retrieve()-------------------------------------*/

std::shared_ptr<SCENE>SCENEMANAGER::Retrieve(ScenesEnum name)
{
    return scenes.find(name)->second;
}

/*--------------------------------------------------SCENEMANAGER CurrentSceneEnum()-------------------------------------*/
/// <summary>
/// Returns the enum of this current scene
/// </summary>
/// <returns></returns>
ScenesEnum SCENEMANAGER::CurrentSceneEnum()
{
    return current_Enum;
}

