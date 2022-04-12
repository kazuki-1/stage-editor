#pragma once
#include "../Base Classes/COMPONENT.h"


class TRANSFORM_3D_DATA : public COMPONENT_DATA
{
public:
    VECTOR3 scale{ 1, 1, 1 };
    VECTOR3 rotation{};
    VECTOR3 translation{};

    TRANSFORM_3D_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), scale, rotation, translation);
    }
};


/// <summary>
/// <para> Class to manage the transform of a 3d gameobject. GameObjects will generally have one when created. </para>
/// <para> �RD�Q�[���I�u�W�F�N�g�̕ϊ��s����Ǘ�����N���X�BGameObject�͊�{�I�Ɋ����ɓ����Ă܂��@</para>
/// </summary>
class TRANSFORM_3D : public COMPONENT
{
private :
    //VECTOR3 translation;
    //VECTOR3 scale;
    VECTOR4 quaternion;
    VECTOR3 velocity;
protected:
    XMFLOAT4X4 transform{};
    TRANSFORM_3D_DATA* data{};
    friend class DEBUG_MANAGER;
public:
    TRANSFORM_3D() {};
    TRANSFORM_3D(GAMEOBJECT* t, COMPONENT_DATA* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (TRANSFORM_3D_DATA) </para>
    /// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iTRANSFORM_3D_DATA�j</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> �t���[���������ɌĂяo���� </para>
    /// </summary>
    void Execute() override;
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
    VECTOR3 Scale();
    /// <summary>
    /// Returns the rotation
    /// </summary>
    /// <returns></returns>
    VECTOR3 Rotation();
    /// <summary>
    /// Returns the translation
    /// </summary>
    /// <returns></returns>
    VECTOR3 Translation();
    /// <summary>
    /// Returns the velocity
    /// </summary>
    /// <returns></returns>
    VECTOR3 Velocity();
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
    TRANSFORM_3D_DATA* Data();
    /// <summary>
    /// Returns the right vector of the object
    /// </summary>
    /// <returns></returns>
    VECTOR3 Right();
    /// <summary>
    /// Returns the forward vector of the object
    /// </summary>
    /// <returns></returns>
    VECTOR3 Forward();
    /// <summary>
    /// Returns the up vector of the object
    /// </summary>
    /// <returns></returns>
    VECTOR3 Up();
    /// <summary>
    /// Returns the quaternion of rotation
    /// </summary>
    /// <returns></returns>
    VECTOR4 Quaternion();
    /// <summary>
    /// <para> Slerps the rotation to the target vector </para>
    /// <para> Target�ɑ΂��ċ��ʐ��`��Ԃ��s��</para>
    /// </summary>
    void SlerpRotation(VECTOR3 target);
    /// <summary>
    /// <para> Slerps the rotation to the target quaternion </para>
    /// <para> Target�ɑ΂��ċ��ʐ��`��Ԃ��s��</para>
    /// </summary>
    void SlerpRotation(VECTOR4 target);



    void SetScale(VECTOR3 s);
    void SetRotation(VECTOR3 s);
    void SetTranslation(VECTOR3 s);
    void SetVelocity(VECTOR3 v);
    void OffsetTranslation(VECTOR3 off);
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(TRANSFORM_3D_DATA)
