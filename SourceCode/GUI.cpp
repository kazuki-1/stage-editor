#include "GUI.h"
#include <string>
#include "Engine/LIGHTING.h"
#include "Engine/DEBUG_MANAGER.h"
#include "STAGE_UI.h"
#include "Components/Base Classes/ComponentCreator.h"

void GUI::Initialize()
{

    ComponentCreator::Instance()->Initialize();
    DebugController::Instance()->Initialize();
}



void GUI::Execute()
{
    IMGUI::Instance()->Execute();
    LightingManager::Instance()->RenderUI();
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