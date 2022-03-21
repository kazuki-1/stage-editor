#pragma once
#include "../Base Classes/COMPONENT.h"

enum class PLAYER_ANIM
{
    T_POSE,
    WALK,
    IDLE,
    JUMP,
    LANDING

};


class PLAYER_CONTROLLER_DATA : public CHARACTER_DATA
{
public:
    PLAYER_CONTROLLER_DATA() { type = COMPONENT_TYPE::PLAYER_CONTROLLER; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<CHARACTER_DATA>(this));
    }
};



class PLAYER_CONTROLLER : public BASE_CHARACTER_COMPONENT
{
    PLAYER_CONTROLLER_DATA* data;

    /// <summary>
    /// <para> WASD movement input </para>
    /// <para> WASD �ړ����� </para>
    /// </summary>
    void MovementInput();
    /// <summary>
    /// <para> WASD rotation input </para>
    /// <para> WASD ��]���� </para>
    /// </summary>
    void RotationInput();
    /// <summary>
    /// <para> Perform collision check with ENVIRONMENTAL_AUDIO gameObjects </para>
    /// <para> ENVIRONMENTAL_AUDIO�Q�[���I�u�W�F�N�g�Ɠ����蔻����v�Z </para>
    /// </summary>
    void SoundCollision();





public:
    PLAYER_CONTROLLER(GAMEOBJECT* g, COMPONENT_DATA* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
    /// <para> �������ɌĂяo���B�Ώۂ̃f�[�^���g���ď��������� (OBB_COLLIDER_DATA) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute() override;
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
    /// <para> All input functions are called here </para>
    /// <para> ���ׂĂ̓��͊֐����Ăяo��</para>
    /// </summary>
    void Input();
    /// <summary>
    /// <para> Perform animation switching </para>
    /// <para> �A�j���[�V�����ύX���� </para>
    /// </summary>
    void AnimationSettings() override;
    /// <summary>
    /// <para> Called when hit </para>
    /// <para> �U�����ꂽ��Ăяo�� </para>
    /// </summary>
    void Hit() override;
    /// <summary>
    /// <para> Called when killed </para>
    /// <para> �E���ꂽ��Ăяo�� </para>
    /// </summary>
    void Kill() override;
    /// <summary>
    /// <para> Updates rotation</para>
    /// <para> ��]���X�V</para>
    /// </summary>
    void UpdateRotation() override;
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(PLAYER_CONTROLLER_DATA)