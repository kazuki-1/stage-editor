#include "../Engine/LIGHTING.h"
#include "../GAMEOBJECT.h"
#include "Base Classes/Component.h"
#include "Transform3D.h"
#include "PointLight.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------PointLight_Data Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------PointLight_Data Constructor---------------------------------------------*/

PointLight_Data::PointLight_Data()
{
    type = COMPONENT_TYPE::POINT_LIGHT;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------PointLight_Component Class--------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------PointLight_Component Constructor--------------------------------------------------*/


PointLight_Component::PointLight_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<PointLight_Data*>(data);
}

/*---------------------------------------PointLight_Component Initialize()--------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT PointLight_Component::Initialize()
{
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::POINT);

    LightingManager::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------PointLight_Component Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void PointLight_Component::Execute()
{
    data->position = GetComponent<Transform3D_Component>()->Translation();
}

/*---------------------------------------PointLight_Component Render()--------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void PointLight_Component::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------PointLight_Component UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void PointLight_Component::UI()
{
    if (ImGui::TreeNode("Pointlight"))
    {
        ImGui::ColorEdit4("Colour", &data->colour.x);
        ImGui::DragFloat("Range", &data->range);
        ImGui::TreePop();
    }

}

/*---------------------------------------PointLight_Component Position()--------------------------------------------------*/

Vector3 PointLight_Component::Position()
{
    return data->position;
}

/*---------------------------------------PointLight_Component Colour()--------------------------------------------------*/

Vector4 PointLight_Component::Colour()
{
    return data->colour;
}

/*---------------------------------------PointLight_Component Range()--------------------------------------------------*/

float PointLight_Component::Range()
{
    return data->range;
}

/*----------------------------------------------------------PointLight_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE PointLight_Component::GetComponentType()
{
    return data->type;
}