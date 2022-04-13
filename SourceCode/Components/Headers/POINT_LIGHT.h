#pragma once
#include "../Base Classes/COMPONENT.h"

class POINT_LIGHT_DATA : public COMPONENT_DATA
{
public:
    POINT_LIGHT_DATA();
    Vector3 position{};
    Vector4 colour{};
    float range{};
    Vector3 placeholder{};
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), position, colour, range, placeholder);
    }
};


class POINT_LIGHT : public COMPONENT
{
    std::shared_ptr<LIGHTING>light;
    POINT_LIGHT_DATA* data;
public:
    POINT_LIGHT(GAMEOBJECT* t, COMPONENT_DATA* data);

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

    Vector3 Position();
    Vector4 Colour();
    float Range();
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(POINT_LIGHT_DATA)
