#pragma once
#include "Base Classes/Component.h"

class SphereCollider_Data : public ComponentData
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 center{};
    float radius{};
    SphereCollider_Data();
    template <class T>
    void serialize(T& t)
    {
        t(name, bone_name, center, radius);
    }
};


class SphereCollider_Component : public BaseColliderComponent
{
    std::shared_ptr<COLLIDERS::SPHERE>collider;
    std::shared_ptr<DYNAMIC_SPHERE>sphere;
    SphereCollider_Data* data;
public:
    SphereCollider_Component() {};
    /// <summary>
    /// <para> Call this constructor when creating this component. Registers the parent of the component and the data linked to the component </para>
    /// <para> �R���|�l���g�𐶐����Ăяo�������������B�R���|�l���g��Parent�ƃf�[�^��o�^���� </para>
    /// </summary>
    /// <param name="t"> : Parent of component (GameObject class)</param>
    /// <param name="d"> : Data used by the component</param>
    SphereCollider_Component(GameObject* t, ComponentData* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SphereCollider_Data) </para>
    /// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iSPHERE_COLLIDER_DATA�j</para>
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
    /// <para>Called after Execute to perform any render functions </para>
    /// <para>Execute���s��ɕ`��p�֐����Ăяo���֐�</para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UI��`�悷��֐� </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> �v���C���[�Ƃ̋������v�Z���� </para>
    /// </summary>
    Vector3 DistanceToPlayer(PlayerController_Component* target) override;
    /// <summary>
    /// <para> Perform collision check with another gameObject </para>
    /// <para> �����ł���Q�[���I�u�W�F�N�g�𓖂��蔻����v�Z</para>
    /// </summary>
    /// <returns></returns>
    bool Collide(GameObject* target) override;
    /// <summary>
    /// <para> Perform collision check based on coordinates </para>
    /// <para> ���W�_����Â��ē����蔻����v�Z </para>
    /// </summary>
    /// <returns></returns>
    bool Collide(Vector3 p) override;
    float Radius();
    Vector3 Center();
    std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider() override;
    SphereCollider_Data* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(SphereCollider_Data)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ComponentData, SphereCollider_Data)