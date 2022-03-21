#include "GUI.h"
#include <string>
#include "Engine/LIGHTING.h"
#include "Engine/DEBUG_MANAGER.h"
#include "STAGE_UI.h"
#include "Components/Base Classes/COMPONENT_CREATOR.h"

void GUI::Initialize()
{

    COMPONENT_CREATOR::Instance()->Initialize();
    DEBUG_MANAGER::Instance()->Initialize();
}



void GUI::Execute()
{
    IMGUI::Instance()->Execute();
    LIGHTINGMANAGER::Instance()->RenderUI();
    STAGE_UI::Instance()->RenderUI();
}

void GUI::Render()
{
    STAGE_UI::Instance()->Render();
    IMGUI::Instance()->Render();


}

void GUI::Finalize()
{
}