#pragma once
#include "Base Classes/COMPONENT.h"

class SPHERE_COLLIDER_DATA : public COMPONENT_DATA
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 center{};
    float radius{};
    SPHERE_COLLIDER_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(name, bone_name, center, radius);
    }
};


class SPHERE_COLLIDER : public BASE_COLLIDER_COMPONENT
{
    std::shared_ptr<COLLIDERS::SPHERE>collider;
    std::shared_ptr<DYNAMIC_SPHERE>sphere;
    SPHERE_COLLIDER_DATA* data;
public:
    SPHERE_COLLIDER() {};
    /// <summary>
    /// <para> Call this constructor when creating this component. Registers the parent of the component and the data linked to the component </para>
    /// <para> �R���|�l���g�𐶐����Ăяo�������������B�R���|�l���g��Parent�ƃf�[�^��o�^���� </para>
    /// </summary>
    /// <param name="t"> : Parent of component (GAMEOBJECT class)</param>
    /// <param name="d"> : Data used by the component</param>
    SPHERE_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
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
    Vector3 DistanceToPlayer(PLAYER_CONTROLLER* target) override;
    /// <summary>
    /// <para> Perform collision check with another gameObject </para>
    /// <para> �����ł���Q�[���I�u�W�F�N�g�𓖂��蔻����v�Z</para>
    /// </summary>
    /// <returns></returns>
    bool Collide(GAMEOBJECT* target) override;
    /// <summary>
    /// <para> Perform collision check based on coordinates </para>
    /// <para> ���W�_����Â��ē����蔻����v�Z </para>
    /// </summary>
    /// <returns></returns>
    bool Collide(Vector3 p) override;
    float Radius();
    Vector3 Center();
    std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider() override;
    SPHERE_COLLIDER_DATA* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(SPHERE_COLLIDER_DATA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT_DATA, SPHERE_COLLIDER_DATA)