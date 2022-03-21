#pragma once
#include "../Base Classes/COMPONENT.h"

class MESH_COLLIDER_DATA : public COMPONENT_DATA
{
public:
    MESH_COLLIDER_DATA() { type = COMPONENT_TYPE::MESH_COL; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this));
    }
};


class MESH_COLLIDER : public COMPONENT
{
    MESH_COLLIDER_DATA* data;
public:
    MESH_COLLIDER() {};
    MESH_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data);

    /// <summary>
    /// <para> Called when created from data </para>
    /// <para> �f�[�^���琶�����ɌĂяo�� </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Perform rendering functions </para>
    /// <para> �`��֐����Ăяo�� </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI </para>
    /// <para> UI��`�� </para>
    /// </summary>
    void UI() override;
    COMPONENT_TYPE GetComponentType() override;
};



CEREAL_REGISTER_TYPE(MESH_COLLIDER_DATA)