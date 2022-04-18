#pragma once
#include "Base Classes/Component.h"

class EnvironmentalAudio_Component;
class Transform3D_Data;
class Transform3D_Component;
class LocalizedCollider_SubComponent;
/// <summary>
/// Stores the data for the localized collider
/// </summary>
class LocalizedCollider_Data 
{
    LocalizedCollider_SubComponent* parent{};
public:
    std::shared_ptr<Transform3D_Data>transform;
    std::shared_ptr<ComponentData>collider_data;
    // Minimum distance to play max volume
    // �ő剹�ʂ��o�͂���ŒZ����
    float minimum_distance{};
    LocalizedCollider_Data() {};
    LocalizedCollider_Data(COMPONENT_TYPE t);
    /// <summary>
    /// <para> Called when creating from data. Links both data to its respective data </para>
    /// <para> �f�[�^���[�h����Ƃ��ɌĂяo���B�f�[�^�������N����</para>
    /// </summary>
    /// <param name="t3d"> : TRANSFORD_3D_DATA</param>
    /// <param name="cd"> : collider Data</param>
    /// <returns></returns>
    HRESULT Initialize(std::shared_ptr<ComponentData>t3d, std::shared_ptr<ComponentData>cd);
    template <class T>
    void serialize(T& t)
    {
        t(transform, collider_data, minimum_distance);
    }
};
/// <summary>
/// <para> As we are using the collider components, it needs its own localized transform component, so we group them together </para>
/// <para> �R���C�_�[�R���|�l���g���g���Ă邽�߁A���R���g��TRANSFORM�R���|�l���g���K�v���ꂽ�̂ł�����̓N���X������</para>
/// </summary>
class LocalizedCollider_SubComponent
{
    std::shared_ptr<BaseColliderComponent>collider;
    std::shared_ptr<Transform3D_Component>transform;
    LocalizedCollider_Data* data;
    EnvironmentalAudio_Component* parent;
    friend EnvironmentalAudio_Component;
public:
    /// <summary>
    /// <para> Use this when creating a localized collider from existing data </para>
    /// <para> �����f�[�^���g����Localized Collider�𐶐����ɌĂяo��</para>
    /// </summary>
    LocalizedCollider_SubComponent(EnvironmentalAudio_Component* g, LocalizedCollider_Data* data);
    /// <summary>
    /// <para> Use this when creating a localized collider</para>
    /// <para> LOCALIZED_COLLIDER�𐶐����ɌĂяo��</para>
    /// </summary>
    LocalizedCollider_SubComponent(GameObject* g, LocalizedCollider_Data* data);
    /// <summary>
    /// <para> Called when created from data </para>
    /// <para> �f�[�^���琶�����ɌĂяo�� </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute();
    /// <summary>
    /// <para> Perform rendering functions </para>
    /// <para> �`��֐����Ăяo�� </para>
    /// </summary>
    void Render();
    /// <summary>
    /// <para> Renders the UI </para>
    /// <para> UI��`�� </para>
    /// </summary>
    void UI();

    std::shared_ptr<BaseColliderComponent>Collider();
    std::shared_ptr<Transform3D_Component>Transform();

    LocalizedCollider_Data* Data();


};




class EnvironmentalAudio_Data : public AudioData
{

public:
    // For storing each collider data for exporting purposes
    // �G�L�X�p�[�g�p�̃R���C�_�[�f�[�^���i�[���邽��
    std::vector<std::shared_ptr<LocalizedCollider_Data>>collider_dataset;
    EnvironmentalAudio_Data() { type = COMPONENT_TYPE::ENV_AUDIO; };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AudioData>(this), collider_dataset);
    }
};
class EnvironmentalAudio_Component : public BASE_AUDIO_COMPONENT
{
    // We are using the collider component instead of the code based component for convenience
    // �֗�����D�悷�邽�߃R�[�h�x�[�X�R���C�_�[�ł͂Ȃ��R���|�l���g�x�[�X�̃R���C�_�[���g��
    std::vector < std::shared_ptr<LocalizedCollider_SubComponent>>colliders;
    EnvironmentalAudio_Data* data;

    /// <summary>
    /// <para> Creates a localized collider and insert it into the dataset </para>
    /// <para> LOCALIZED_COLLIDER�𐶐����A�f�[�^�Z�b�g�ɓo�^ </para>
    /// </summary>
    void CreateLocalizedCollider(COMPONENT_TYPE t);
    friend LocalizedCollider_SubComponent;
public:
    EnvironmentalAudio_Component() {};
    EnvironmentalAudio_Component(GameObject* t, ComponentData* d);
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
    std::vector<std::shared_ptr<LocalizedCollider_SubComponent>>Colliders();

};


CEREAL_REGISTER_TYPE(EnvironmentalAudio_Data)
