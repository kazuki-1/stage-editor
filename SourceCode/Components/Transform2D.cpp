#include "Base Classes/Component.h"
#include "Transform2D.h"



/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform2D_Data Class--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform2D_Data Constructor--------------------------------------------*/

Transform2D_Data::Transform2D_Data()
{
    type = COMPONENT_TYPE::Transform2D_Component;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform2D_Component Class-------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform2D_Component Constructor-------------------------------------------------*/

Transform2D_Component::Transform2D_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<Transform2D_Data*>(data);

}

/*---------------------------------------Transform2D_Component Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SphereCollider_Data) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT Transform2D_Component::Initialize()
{
    return S_OK;
}

/*---------------------------------------Transform2D_Component UI()-------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void Transform2D_Component::UI()
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

/*---------------------------------------Transform2D_Component Position()-------------------------------------------------*/
/// <summary>
/// Returns the position
/// </summary>
/// <returns></returns>
Vector2 Transform2D_Component::Position()
{
    return data->position;
}

/*---------------------------------------Transform2D_Component Rotation()-------------------------------------------------*/
/// <summary>
/// Returns the angle of rotation
/// </summary>
/// <returns></returns>
float Transform2D_Component::Rotation()
{
    return data->rotation;
}

/*---------------------------------------Transform2D_Component Data()-------------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
Transform2D_Data* Transform2D_Component::Data()
{
    return data;
}

/*----------------------------------------------------------Transform2D_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE Transform2D_Component::GetComponentType()
{
    return data->type;
}