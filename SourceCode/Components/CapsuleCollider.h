#pragma once
#include "Base Classes/Component.h"

class CapsuleCollider_Data : public ComponentData
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 center{};
    //VECTOR3 offset{};
    Vector3 rotation{};
    float height{ 1.0f };
    float radius{ 0.5f };

    CapsuleCollider_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), name, bone_name, rotation, center, height, radius);
    }
};

class CapsuleCollider_Component : public BaseColliderComponent
{
    std::shared_ptr<COLLIDERS::CAPSULE>collider;
    std::shared_ptr<DYNAMIC_CAPSULE>capsule;
    CapsuleCollider_Data* data;
public:
    CapsuleCollider_Component() {};
    CapsuleCollider_Component(GameObject* t, ComponentData* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (CapsuleCollider_Data)</para>
    /// <para> �������ɌĂяo���B�Ώۂ̃f�[�^���g���ď��������� (CAPSULE COLLIDER DATA) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute(XMMATRIX transform) override;
    /// <summary>
    /// <para> Called after Execute() to perform any render functions </para>
    /// <para> Execute()��Ƀ����_�[�֐������s����悤�ɖ��t���[���ɌĂяo�� </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UI��`�� </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> �v���C���[�Ƃ̋������v�Z���� </para>    
    /// </summary>
    /// <returns></returns>
    Vector3 DistanceToPlayer(PlayerController_Component* target) override;
    /// <summary>
    /// <para> Perform collision check to the target gameObject </para>
    /// <para> �Q�[���I�u�W�F�N�g�ɓ����蔻����v�Z </para>
    /// </summary
    bool Collide(GameObject* target) override;
    /// <summary>
    /// <para> Perform collision check based on coordinates </para>
    /// <para> ���W�_����Â��ē����蔻����v�Z </para>
    /// </summary>
    /// <returns></returns>
    bool Collide(Vector3 p) override;
    /// <summary>
    /// Returns the name of the bone the collider is attached to
    /// </summary>
    /// <returns></returns>
    const std::string& BoneName();
    /// <summary>
    /// Returns the name of the collider
    /// </summary>
    /// <returns></returns>
    const std::string& ColliderName();
    /// <summary>
    /// Returns the center point of the collider
    /// </summary>
    /// <returns></returns>
    const Vector3 Center();
    /// <summary>
    /// Returns the radius of the collider
    /// </summary>
    /// <returns></returns>
    const float Radius();
    /// <summary>
    /// Returns the height of the collider
    /// </summary>
    /// <returns></returns>
    const float Height();
    /// <summary>
    /// Returns the data class that stores all the data used in this component
    /// </summary>
    /// <returns></returns>
    CapsuleCollider_Data* Data();
    COMPONENT_TYPE GetComponentType() override;
    std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider();

};

CEREAL_REGISTER_TYPE(CapsuleCollider_Data)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ComponentData, CapsuleCollider_Data)