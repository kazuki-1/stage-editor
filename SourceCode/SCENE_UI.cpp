#include "SCENE_UI.h"
#include "Components/Base Classes/COMPONENT_CREATOR.h"

#include "Engine/DEBUG_MANAGER.h"
#include "Engine/IMGUI.h"
#include "Engine/LIGHTING.h"
#include "STAGE_UI.h"


HRESULT SCENE_UI::Initialize()
{
    COMPONENT_CREATOR::Instance()->Initialize();
    DEBUG_MANAGER::Instance()->Initialize();
    return S_OK;

}
void SCENE_UI::Execute()
{
    IMGUI::Instance()->Execute();
    LIGHTINGMANAGER::Instance()->RenderUI();
    STAGE_UI::Instance()->RenderUI();

}
void SCENE_UI::Render()
{
    STAGE_UI::Instance()->Render();
    IMGUI::Instance()->Render();

}
void SCENE_UI::Finalize()
{

}