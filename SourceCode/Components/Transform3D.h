#pragma once
#include "Base Classes/Component.h"


class Transform3D_Data : public ComponentData
{
public:
    Vector3 scale{ 1, 1, 1 };
    Vector3 rotation{};
    Vector3 translation{};

    Transform3D_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), scale, rotation, translation);
    }
};


/// <summary>
/// <para> Class to manage the transform of a 3d gameobject. GetGameObjects will generally have one when created. </para>
/// <para> �RD�Q�[���I�u�W�F�N�g�̕ϊ��s����Ǘ�����N���X�BGameObject�͊�{�I�Ɋ����ɓ����Ă܂��@</para>
/// </summary>
class Transform3D_Component : public Component
{
private :
    //VECTOR3 translation;
    //VECTOR3 scale;
    Vector4 quaternion;
    Vector3 velocity;
protected:
    XMFLOAT4X4 transform{};
    Transform3D_Data* data{};
    friend class DebugController;
public:
    Transform3D_Component() {};
    Transform3D_Component(GameObject* t, ComponentData* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (Transform3D_Data) </para>
    /// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iTRANSFORM_3D_DATA�j</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> �t���[���������ɌĂяo���� </para>
    /// </summary>
    void Execute() override;
    void ExecuteUI();
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
    /// Returns the scale 
    /// </summary>
    /// <returns></returns>
    Vector3 Scale();
    /// <summary>
    /// Returns the rotation
    /// </summary>
    /// <returns></returns>
    Vector3 Rotation();
    /// <summary>
    /// Returns the translation
    /// </summary>
    /// <returns></returns>
    Vector3 Translation();
    /// <summary>
    /// Returns the velocity
    /// </summary>
    /// <returns></returns>
    Vector3 Velocity();
    /// <summary>
    /// <para> Returns the transformation matrix in a XMFLOAT4X4 form </para>
    /// <para> �ϊ��s���XMFLOAT4X4�̌`�Ŗ߂�</para>
    /// </summary>
    /// <returns></returns>
    XMFLOAT4X4 Transform();
    /// <summary>
    /// <para> Returns the transformation matrix in a XMMATRIX form </para>
    /// <para> �ϊ��s���XMMATRIX�̌`�Ŗ߂�</para>
    /// </summary>
    /// <returns></returns>
    XMMATRIX TransformMatrix();
    /// <summary>
    /// <para> Returns the transformation matrix in a XMMATRIX form </para>
    /// <para> �ϊ��s���XMMATRIX�̌`�Ŗ߂�</para>
    /// </summary>
    /// <returns></returns>
    XMMATRIX TransformMatrixQuaternion();
    /// <summary>
    /// <para> Returns the data class that stores all the data of this component </para>
    /// <para> �R���|�l���g���G�L�X�p�[�g�p�f�[�^��߂�</para>
    /// </summary>
    /// <returns></returns>
    Transform3D_Data* Data();
    /// <summary>
    /// Returns the right vector of the object
    /// </summary>
    /// <returns></returns>
    Vector3 Right();
    /// <summary>
    /// Returns the forward vector of the object
    /// </summary>
    /// <returns></returns>
    Vector3 Forward();
    /// <summary>
    /// Returns the up vector of the object
    /// </summary>
    /// <returns></returns>
    Vector3 Up();
    /// <summary>
    /// Returns the quaternion of rotation
    /// </summary>
    /// <returns></returns>
    Vector4 Quaternion();
    /// <summary>
    /// <para> Slerps the rotation to the target vector </para>
    /// <para> Target�ɑ΂��ċ��ʐ��`��Ԃ��s��</para>
    /// </summary>
    void SlerpRotation(Vector3 target);
    /// <summary>
    /// <para> Slerps the rotation to the target quaternion </para>
    /// <para> Target�ɑ΂��ċ��ʐ��`��Ԃ��s��</para>
    /// </summary>
    void SlerpRotation(Vector4 target);



    void SetScale(Vector3 s);
    void SetRotation(Vector3 s);
    void SetTranslation(Vector3 s);
    void SetVelocity(Vector3 v);
    void OffsetTranslation(Vector3 off);
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(Transform3D_Data)
