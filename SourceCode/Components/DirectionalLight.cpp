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
/*---------------------------------------------------DirectionalLight Class---------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------DirectionalLight Constructor---------------------------------------------------------------*/

DirectionalLight::DirectionalLight(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<DirectionalLight_Data*>(data);

}

/*---------------------------------------------------DirectionalLight Initialize()---------------------------------------------------------------*/

HRESULT DirectionalLight::Initialize()
{
    
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::DIRECTIONAL);
    LIGHTINGMANAGER::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------------------DirectionalLight Execute()---------------------------------------------------------------*/

void DirectionalLight::Execute()
{

}

/*---------------------------------------------------DirectionalLight Render()---------------------------------------------------------------*/

void DirectionalLight::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------------------DirectionalLight UI()---------------------------------------------------------------*/

void DirectionalLight::UI()
{
    if (ImGui::TreeNode("Directional Light"))
    {
        ImGui::DragFloat3("Direction", &data->direction.x, 0.05f, -1.0f, 1.0f);
        ImGui::ColorEdit4("Colour", &data->colour.x);
        ImGui::TreePop();
    }
}

/*---------------------------------------------------DirectionalLight Direction()---------------------------------------------------------------*/

Vector3 DirectionalLight::Direction()
{
    return data->direction;
}

/*---------------------------------------------------DirectionalLight Colour()---------------------------------------------------------------*/

Vector4 DirectionalLight::Colour()
{
    return data->colour;
}

/*---------------------------------------------------DirectionalLight GetComponentType()---------------------------------------------------------------*/

COMPONENT_TYPE DirectionalLight::GetComponentType()
{
    return data->type;
}