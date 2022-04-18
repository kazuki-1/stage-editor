#include "MeshCollider.h"
#include "../Engine/COLLISION.h"
#include "Mesh.h"
/*----------------------------------------------MeshCollider_Component Class----------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------MeshCollider_Component Constructor------------------------------------------------------*/

MeshCollider_Component::MeshCollider_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    data = static_cast<MeshCollider_Data*>(data);
}

/*----------------------------------------------MeshCollider_Component Initialize()------------------------------------------------------*/
/// <summary>
/// <para> Called when created from data </para>
/// <para> �f�[�^���琶�����ɌĂяo�� </para>
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
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void MeshCollider_Component::Execute()
{

}

/*----------------------------------------------MeshCollider_Component Render()------------------------------------------------------*/
/// <summary>
/// <para> Perform rendering functions </para>
/// <para> �`��֐����Ăяo�� </para>
/// </summary>
void MeshCollider_Component::Render()
{

}

/*----------------------------------------------MeshCollider_Component UI()------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI </para>
/// <para> UI��`�� </para>
/// </summary>
void MeshCollider_Component::UI()
{

}

/*----------------------------------------------MeshCollider_Component Triggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to true,</para>
/// <para> �g���K�[�X�e�[�g��True�ɐݒ肷�� </para>
/// </summary>
void MeshCollider_Component::Triggered()
{
    trigger = true;
}

/*----------------------------------------------MeshCollider_Component NotTriggered()------------------------------------------------------*/
/// <summary>
/// <para> Sets the trigger state to false,</para>
/// <para> �g���K�[�X�e�[�g��false�ɐݒ肷�� </para>
/// </summary>
void MeshCollider_Component::NotTriggered()
{
    trigger = false;
}

/*----------------------------------------------MeshCollider_Component OnTrigger()------------------------------------------------------*/
/// <summary>
/// <para> Returns the boolean member trigger </para>
/// <para> trigger�����o�[��Ԃ� </para>
/// </summary>
/// <returns></returns>
bool MeshCollider_Component::OnTrigger()
{
    return trigger;
}