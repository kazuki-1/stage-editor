#include "../Base Classes/COMPONENT.h"

#include "../Headers/TRANSFORM_2D.h"


/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_2D_DATA Class--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_2D_DATA Constructor--------------------------------------------*/

TRANSFORM_2D_DATA::TRANSFORM_2D_DATA()
{
    type = COMPONENT_TYPE::TRANSFORM_2D;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_2D Class-------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_2D Constructor-------------------------------------------------*/

TRANSFORM_2D::TRANSFORM_2D(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<TRANSFORM_2D_DATA*>(data);

}

/*---------------------------------------TRANSFORM_2D Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT TRANSFORM_2D::Initialize()
{
    return S_OK;
}

/*---------------------------------------TRANSFORM_2D UI()-------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void TRANSFORM_2D::UI()
{
    D3D11_VIEWPORT vp{};
    UINT numVP{ 1 };
    DirectX11::Instance()->DeviceContext()->RSGetViewports(&numVP, &vp);
    if (ImGui::TreeNode("Transform 2D"))
    {
        ImGui::DragFloat("X", &data->position.x, 0.5f, 0, vp.Width);
        ImGui::DragFloat("Y", &data->position.y, 0.5f, 0, vp.Height);
        ImGui::DragFloat("Rotation", &data->rotation, 0.5f);
        ImGui::TreePop();
    }
}

/*---------------------------------------TRANSFORM_2D Position()-------------------------------------------------*/
/// <summary>
/// Returns the position
/// </summary>
/// <returns></returns>
Vector2 TRANSFORM_2D::Position()
{
    return data->position;
}

/*---------------------------------------TRANSFORM_2D Rotation()-------------------------------------------------*/
/// <summary>
/// Returns the angle of rotation
/// </summary>
/// <returns></returns>
float TRANSFORM_2D::Rotation()
{
    return data->rotation;
}

/*---------------------------------------TRANSFORM_2D Data()-------------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
TRANSFORM_2D_DATA* TRANSFORM_2D::Data()
{
    return data;
}

/*----------------------------------------------------------TRANSFORM_2D GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE TRANSFORM_2D::GetComponentType()
{
    return data->type;
}