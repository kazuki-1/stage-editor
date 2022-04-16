#include "../Engine/LIGHTING.h"
#include "../GAMEOBJECT.h"
#include "Base Classes/Component.h"
#include "TRANSFORM_3D.h"
#include "POINT_LIGHT.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------POINT_LIGHT_DATA Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------POINT_LIGHT_DATA Constructor---------------------------------------------*/

POINT_LIGHT_DATA::POINT_LIGHT_DATA()
{
    type = COMPONENT_TYPE::POINT_LIGHT;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------POINT_LIGHT Class--------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------POINT_LIGHT Constructor--------------------------------------------------*/


POINT_LIGHT::POINT_LIGHT(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<POINT_LIGHT_DATA*>(data);
}

/*---------------------------------------POINT_LIGHT Initialize()--------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT POINT_LIGHT::Initialize()
{
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::POINT);

    LIGHTINGMANAGER::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------POINT_LIGHT Execute()--------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void POINT_LIGHT::Execute()
{
    data->position = GetComponent<TRANSFORM_3D>()->Translation();
}

/*---------------------------------------POINT_LIGHT Render()--------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void POINT_LIGHT::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------POINT_LIGHT UI()--------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void POINT_LIGHT::UI()
{
    if (ImGui::TreeNode("Pointlight"))
    {
        ImGui::ColorEdit4("Colour", &data->colour.x);
        ImGui::DragFloat("Range", &data->range);
        ImGui::TreePop();
    }

}

/*---------------------------------------POINT_LIGHT Position()--------------------------------------------------*/

Vector3 POINT_LIGHT::Position()
{
    return data->position;
}

/*---------------------------------------POINT_LIGHT Colour()--------------------------------------------------*/

Vector4 POINT_LIGHT::Colour()
{
    return data->colour;
}

/*---------------------------------------POINT_LIGHT Range()--------------------------------------------------*/

float POINT_LIGHT::Range()
{
    return data->range;
}

/*----------------------------------------------------------POINT_LIGHT GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE POINT_LIGHT::GetComponentType()
{
    return data->type;
}