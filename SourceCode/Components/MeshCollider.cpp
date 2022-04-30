#include "MeshCollider.h"
#include "../Engine/COLLISION.h"
#include "Transform3D.h"
#include "Mesh.h"
/*----------------------------------------------MeshCollider_Component Class----------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MeshCollider_Component Constructor------------------------------------------------------*/

MeshCollider_Component::MeshCollider_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<MeshCollider_Data*>(data);
}

/*----------------------------------------------MeshCollider_Component Initialize()------------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> データから生成時に呼び出す </para>
/// </summary>
/// <returns></returns>
HRESULT MeshCollider_Component::Initialize()
{

    if (data)
    {
        COLLIDERS::RAYCAST_MANAGER::Instance()->Insert(GetComponent<Mesh_Component>());
    }
    return S_OK;
}

/*----------------------------------------------MeshCollider_Component Execute()------------------------------------------------------*/
/// <summary>
/// <para> Called every frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void MeshCollider_Component::Execute()
{

#ifdef CHECK_VALUE

    // Parameter preparation
    Mesh_Component* mesh = GetComponent<Mesh_Component>();
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();

    Vector3 next_vertical, next_horizontal, current_position;

    current_position = transform->GetTranslation();
    next_vertical = current_position;
    next_vertical.y += transform->GetVelocity().y;
    next_horizontal = current_position;
    next_horizontal.x += transform->GetVelocity().x;
    next_horizontal.z += transform->GetVelocity().z;

    if (!mesh)
        return;
    COLLIDERS::RAYCASTDATA hori{}, verti{};;

    // Check horizontol
    enteredHorizontal = RAYCAST(current_position, next_horizontal, mesh, verti);

    // Check vertical
    enteredVertical = RAYCAST(current_position, next_vertical, mesh, verti);

    if ((last_state ^ enteredHorizontal || last_state ^ enteredVertical) && !last_state)
        trigger = true;
    else
        trigger = false;
    last_state = enteredHorizontal ? enteredHorizontal : enteredVertical ? enteredVertical : false;

#endif

}

/*----------------------------------------------MeshCollider_Component Render()------------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> 描画関数を呼び出す </para>
/// </summary>
void MeshCollider_Component::Render()
{

}

/*----------------------------------------------MeshCollider_Component UI()------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UIを描画 </para>
/// </summary>
void MeshCollider_Component::UI()
{

}

/*----------------------------------------------MeshCollider_Component Triggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to true,</para>
/// <para> トリガーステートをTrueに設定する </para>
/// </summary>
bool MeshCollider_Component::Triggered()
{
    return trigger;
}

/*----------------------------------------------MeshCollider_Component NotTriggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to false,</para>
/// <para> トリガーステートをfalseに設定する </para>
/// </summary>
bool MeshCollider_Component::CollidedHorizontal()
{
    return enteredHorizontal;
}

/*----------------------------------------------MeshCollider_Component OnTrigger()------------------------------------------------------*/
/// <summary>
/// <para> Returns the boolean member trigger </para>
/// <para> triggerメンバーを返す </para>
/// </summary>
/// <returns></returns>
bool MeshCollider_Component::CollidedVertical()
{
    return enteredVertical;
}

/*----------------------------------------------MeshCollider_Component GetComponentType()------------------------------------------------------*/

COMPONENT_TYPE MeshCollider_Component::GetComponentType()
{
    return data->type;
}