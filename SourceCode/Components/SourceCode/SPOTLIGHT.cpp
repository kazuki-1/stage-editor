#include "../Headers/SPOTLIGHT.h"
#include "../../Engine/LIGHTING.h"
#include "../../GAMEOBJECT.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPOTLIGHT_DATA Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPOTLIGHT_DATA Constructor-----------------------------------------------*/

SPOTLIGHT_DATA::SPOTLIGHT_DATA()
{
    type = COMPONENT_TYPE::SPOTLIGHT;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPOTLIGHT Class----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPOTLIGHT Constructor----------------------------------------------------*/

SPOTLIGHT::SPOTLIGHT(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<SPOTLIGHT_DATA*>(data);
}

/*---------------------------------------SPOTLIGHT Initialize()----------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT SPOTLIGHT::Initialize()
{
    light = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::SPOT);
    LIGHTINGMANAGER::Instance()->Insert(Parent()->Data()->Name(), light);
    return S_OK;
}

/*---------------------------------------SPOTLIGHT Execute()----------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SPOTLIGHT::Execute()
{

}

/*---------------------------------------SPOTLIGHT Render()----------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void SPOTLIGHT::Render()
{
    light->WriteToData(this);
}

/*---------------------------------------SPOTLIGHT UI()----------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void SPOTLIGHT::UI()
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

/*---------------------------------------SPOTLIGHT Direction()----------------------------------------------------*/

Vector3 SPOTLIGHT::Direction()
{
    return data->direction;
}

/*---------------------------------------SPOTLIGHT Position()----------------------------------------------------*/

Vector3 SPOTLIGHT::Position()
{
    return data->position;
}

/*---------------------------------------SPOTLIGHT Colour()----------------------------------------------------*/

Vector4 SPOTLIGHT::Colour()
{
    return data->colour;
}

/*---------------------------------------SPOTLIGHT Range()----------------------------------------------------*/

float SPOTLIGHT::Range()
{
    return data->range;
}

/*---------------------------------------SPOTLIGHT InnerCorner()----------------------------------------------------*/

float SPOTLIGHT::InnerCorner()
{
    return data->innerCorner;
}

/*---------------------------------------SPOTLIGHT OuterCorner()----------------------------------------------------*/

float SPOTLIGHT::OuterCorner()
{
    return data->outerCorner;
}

/*----------------------------------------------------------SPOTLIGHT GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE SPOTLIGHT::GetComponentType()
{
    return data->type;
}

