#include "../Headers/MESH_COLLIDER.h"
#include "../../Engine/COLLISION.h"
#include "../Headers/MESH.h"
/*----------------------------------------------MESH_COLLIDER Class----------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MESH_COLLIDER Constructor------------------------------------------------------*/

MESH_COLLIDER::MESH_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    data = static_cast<MESH_COLLIDER_DATA*>(data);
}

/*----------------------------------------------MESH_COLLIDER Initialize()------------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> �f�[�^���琶�����ɌĂяo�� </para>
/// </summary>
/// <returns></returns>
HRESULT MESH_COLLIDER::Initialize()
{

    if (data)
    {
        COLLIDERS::RAYCAST_MANAGER::Instance()->Insert(GetComponent<MESH>());
    }
    return S_OK;
}

/*----------------------------------------------MESH_COLLIDER Execute()------------------------------------------------------*/
/// <summary>
/// <para> Called every frame </para>
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void MESH_COLLIDER::Execute()
{

}

/*----------------------------------------------MESH_COLLIDER Render()------------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> �`��֐����Ăяo�� </para>
/// </summary>
void MESH_COLLIDER::Render()
{

}

/*----------------------------------------------MESH_COLLIDER UI()------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UI��`�� </para>
/// </summary>
void MESH_COLLIDER::UI()
{

}

/*----------------------------------------------MESH_COLLIDER Triggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to true,</para>
/// <para> �g���K�[�X�e�[�g��True�ɐݒ肷�� </para>
/// </summary>
void MESH_COLLIDER::Triggered()
{
    trigger = true;
}

/*----------------------------------------------MESH_COLLIDER NotTriggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to false,</para>
/// <para> �g���K�[�X�e�[�g��false�ɐݒ肷�� </para>
/// </summary>
void MESH_COLLIDER::NotTriggered()
{
    trigger = false;
}

/*----------------------------------------------MESH_COLLIDER OnTrigger()------------------------------------------------------*/
/// <summary>
/// <para> Returns the boolean member trigger </para>
/// <para> trigger�����o�[��Ԃ� </para>
/// </summary>
/// <returns></returns>
bool MESH_COLLIDER::OnTrigger()
{
    return trigger;
}