#include "EnemyController.h"

/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------EnemyController_Component Class---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------EnemyController_Component Constructor---------------------------------------------------------------*/


EnemyController_Component::EnemyController_Component(GameObject* g, ComponentData* d)
{
    parent = g;
    data = static_cast<EnemyController_Data*>(d);
}

/*---------------------------------------------------EnemyController_Component Initialize()---------------------------------------------------------------*/

HRESULT EnemyController_Component::Initialize()
{
    return S_OK;
}

/*---------------------------------------------------EnemyController_Component Execute()---------------------------------------------------------------*/

void EnemyController_Component::Execute()
{

}

/*---------------------------------------------------EnemyController_Component UI()---------------------------------------------------------------*/

void EnemyController_Component::UI()
{
    if (ImGui::TreeNode("Player Controller"))
    {
        ImGui::InputText("Name : ", data->name, 256);
        ImGui::TreePop();
    }
}

/*---------------------------------------------------EnemyController_Component Render()---------------------------------------------------------------*/

void EnemyController_Component::Render()
{

}

/*---------------------------------------------------EnemyController_Component GetComponentType()---------------------------------------------------------------*/

COMPONENT_TYPE EnemyController_Component::GetComponentType()
{
    return data->type;
}