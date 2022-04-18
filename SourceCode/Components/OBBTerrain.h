#pragma once
#include "Base Classes/Component.h"
class OBBCollider_Component;



// UNUSED
class OBBTerrain_Data : public ComponentData
{
public:
    std::shared_ptr<AudioData>audioData;
    std::shared_ptr<OBBCollider_Data>colliderData;
    OBBTerrain_Data();
    bool overlap;
    bool triggerAudio;

    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), audioData, colliderData, overlap, triggerAudio);
    }
};

// UNUSED
class OBBTerrain_Component : public Component
{
    std::shared_ptr<OBBCollider_Component>collider;
    std::shared_ptr<AUDIO>audio;
    OBBTerrain_Data* data;


public:
    OBBTerrain_Component() {};
    OBBTerrain_Component(GameObject* g, ComponentData* d);




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
    COMPONENT_TYPE GetComponentType() override;



};


























CEREAL_REGISTER_TYPE(OBBTerrain_Data)