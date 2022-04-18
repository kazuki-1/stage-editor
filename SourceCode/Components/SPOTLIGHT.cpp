#include "../Engine/LIGHTING.h"
#include "../GAMEOBJECT.h"
#include "SpotLight.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SpotLight_Data Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SpotLight_Data Constructor-----------------------------------------------*/

SpotLight_Data::SpotLight_Data()
{
    type = COMPONENT_TYPE::SPOTLIGHT;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SpotLight_Component Class----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SpotLight_Component Constructor----------------------------------------------------*/

SpotLight_Component::SpotLight_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<SpotLight_Data*>(data);
}

/*---------------------------------------SpotLight_Component Initialize()----------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SpotLight_Data)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (SpotLight_Data) </para>
/// </summary>
/// <returns></returns>
HRESULT SpotLight_Component::Initialize()
{
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::SPOT);
    LightingManager::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------SpotLight_Component Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SpotLight_Component::Execute()
{

}

/*---------------------------------------SpotLight_Component Render()----------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void SpotLight_Component::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------SpotLight_Component UI()----------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void SpotLight_Component::UI()
{
    if (ImGui::TreeNode("Spotlight"))
    {
        ImGui::DragFloat3("Direction", &data->direction.x, 0.05f, -1.0f, 1.0f);
        ImGui::DragFloat3("Position", &data->position.x, 0.05f);
        ImGui::ColorEdit4("Colour", &data->colour.x);
        ImGui::DragFloat("Range", &data->range);
        ImGui::DragFloat("Inner Corner", &data->innerCorner);
        ImGui::DragFloat("Outer Corner", &data->outerCorner);
        ImGui::TreePop();
    }

}

/*---------------------------------------SpotLight_Component Direction()----------------------------------------------------*/

Vector3 SpotLight_Component::Direction()
{
    return data->direction;
}

/*---------------------------------------SpotLight_Component Position()----------------------------------------------------*/

Vector3 SpotLight_Component::Position()
{
    return data->position;
}

/*---------------------------------------SpotLight_Component Colour()----------------------------------------------------*/

Vector4 SpotLight_Component::Colour()
{
    return data->colour;
}

/*---------------------------------------SpotLight_Component Range()----------------------------------------------------*/

float SpotLight_Component::Range()
{
    return data->range;
}

/*---------------------------------------SpotLight_Component InnerCorner()----------------------------------------------------*/

float SpotLight_Component::InnerCorner()
{
    return data->innerCorner;
}

/*---------------------------------------SpotLight_Component OuterCorner()----------------------------------------------------*/

float SpotLight_Component::OuterCorner()
{
    return data->outerCorner;
}

/*----------------------------------------------------------SpotLight_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE SpotLight_Component::GetComponentType()
{
    return data->type;
}

