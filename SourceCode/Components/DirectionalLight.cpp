#include "Base Classes/Component.h"
#include "DirectionalLight.h"
#include "../Engine/LIGHTING.h"
#include "../GAMEOBJECT.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DirectionalLight_Data Class---------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DirectionalLight_Data Constructor---------------------------------------------------------------*/

DirectionalLight_Data::DirectionalLight_Data()
{
    type = COMPONENT_TYPE::DIRECTIONAL_LIGHT;
}

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DirectionalLight_Component Class---------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DirectionalLight_Component Constructor---------------------------------------------------------------*/

DirectionalLight_Component::DirectionalLight_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<DirectionalLight_Data*>(data);

}

/*---------------------------------------------------DirectionalLight_Component Initialize()---------------------------------------------------------------*/

HRESULT DirectionalLight_Component::Initialize()
{
    
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::DIRECTIONAL);
    LightingManager::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------------------DirectionalLight_Component Execute()---------------------------------------------------------------*/

void DirectionalLight_Component::Execute()
{

}

/*---------------------------------------------------DirectionalLight_Component Render()---------------------------------------------------------------*/

void DirectionalLight_Component::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------------------DirectionalLight_Component UI()---------------------------------------------------------------*/

void DirectionalLight_Component::UI()
{
    if (ImGui::TreeNode("Directional Light"))
    {
        ImGui::DragFloat3("Direction", &data->direction.x, 0.05f, -1.0f, 1.0f);
        ImGui::ColorEdit4("Colour", &data->colour.x);
        ImGui::TreePop();
    }
}

/*---------------------------------------------------DirectionalLight_Component Direction()---------------------------------------------------------------*/

Vector3 DirectionalLight_Component::Direction()
{
    return data->direction;
}

/*---------------------------------------------------DirectionalLight_Component Colour()---------------------------------------------------------------*/

Vector4 DirectionalLight_Component::Colour()
{
    return data->colour;
}

/*---------------------------------------------------DirectionalLight_Component GetComponentType()---------------------------------------------------------------*/

COMPONENT_TYPE DirectionalLight_Component::GetComponentType()
{
    return data->type;
}