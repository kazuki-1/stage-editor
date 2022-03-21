#include "../Headers/MESH_COLLIDER.h"

/*----------------------------------------------MESH_COLLIDER Class----------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MESH_COLLIDER Constructor------------------------------------------------------*/

MESH_COLLIDER::MESH_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<MESH_COLLIDER_DATA*>(data);
}

/*----------------------------------------------MESH_COLLIDER Initialize()------------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT MESH_COLLIDER::Initialize()
{
    return S_OK;
}

/*----------------------------------------------MESH_COLLIDER Execute()------------------------------------------------------*/
/// <summary>
/// <para> Called every frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void MESH_COLLIDER::Execute()
{

}

/*----------------------------------------------MESH_COLLIDER Render()------------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void MESH_COLLIDER::Render()
{
}

/*----------------------------------------------MESH_COLLIDER UI()------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void MESH_COLLIDER::UI()
{
    if (ImGui::TreeNode("Mesh Collider"))
    {
        ImGui::TreePop();
    }

}


/*----------------------------------------------------------MESH_COLLIDER GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE MESH_COLLIDER::GetComponentType()
{
    return data->type;
}