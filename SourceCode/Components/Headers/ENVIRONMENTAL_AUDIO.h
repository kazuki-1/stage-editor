#pragma once
#include "../Base Classes/COMPONENT.h"

class ENVIRONMENTAL_AUDIO;
class TRANSFORM_3D_DATA;
class TRANSFORM_3D;
class LOCALIZED_COLLIDER;
/// <summary>
/// Stores the data for the localized collider
/// </summary>
class LOCALIZED_COLLIDER_DATA 
{
    LOCALIZED_COLLIDER* parent{};
public:
    std::shared_ptr<TRANSFORM_3D_DATA>transform;
    std::shared_ptr<COMPONENT_DATA>collider_data;
    // Minimum distance to play max volume
    // �ő剹�ʂ��o�͂���ŒZ����
    float minimum_distance{};
    LOCALIZED_COLLIDER_DATA() {};
    LOCALIZED_COLLIDER_DATA(COMPONENT_TYPE t);
    /// <summary>
    /// <para> Called when creating from data. Links both data to its respective data </para>
    /// <para> �f�[�^���[�h����Ƃ��ɌĂяo���B�f�[�^�������N����</para>
    /// </summary>
    /// <param name="t3d"> : TRANSFORD_3D_DATA</param>
    /// <param name="cd"> : collider Data</param>
    /// <returns></returns>
    HRESULT Initialize(std::shared_ptr<COMPONENT_DATA>t3d, std::shared_ptr<COMPONENT_DATA>cd);
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
class LOCALIZED_COLLIDER
{
    std::shared_ptr<BASE_COLLIDER_COMPONENT>collider;
    std::shared_ptr<TRANSFORM_3D>transform;
    LOCALIZED_COLLIDER_DATA* data;
    ENVIRONMENTAL_AUDIO* parent;
    friend ENVIRONMENTAL_AUDIO;
public:
    /// <summary>
    /// <para> Use this when creating a localized collider from existing data </para>
    /// <para> �����f�[�^���g����Localized Collider�𐶐����ɌĂяo��</para>
    /// </summary>
    LOCALIZED_COLLIDER(ENVIRONMENTAL_AUDIO* g, LOCALIZED_COLLIDER_DATA* data);
    /// <summary>
    /// <para> Use this when creating a localized collider</para>
    /// <para> LOCALIZED_COLLIDER�𐶐����ɌĂяo��</para>
    /// </summary>
    LOCALIZED_COLLIDER(GAMEOBJECT* g, LOCALIZED_COLLIDER_DATA* data);
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

    std::shared_ptr<BASE_COLLIDER_COMPONENT>Collider();
    std::shared_ptr<TRANSFORM_3D>Transform();

    LOCALIZED_COLLIDER_DATA* Data();


};




class ENVIRONMENTAL_AUDIO_DATA : public AUDIO_DATA
{

public:
    // For storing each collider data for exporting purposes
    // �G�L�X�p�[�g�p�̃R���C�_�[�f�[�^���i�[���邽��
    std::vector<std::shared_ptr<LOCALIZED_COLLIDER_DATA>>collider_dataset;
    ENVIRONMENTAL_AUDIO_DATA() { type = COMPONENT_TYPE::ENV_AUDIO; };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AUDIO_DATA>(this), collider_dataset);
    }
};
class ENVIRONMENTAL_AUDIO : public BASE_AUDIO_COMPONENT
{
    // We are using the collider component instead of the code based component for convenience
    // �֗�����D�悷�邽�߃R�[�h�x�[�X�R���C�_�[�ł͂Ȃ��R���|�l���g�x�[�X�̃R���C�_�[���g��
    std::vector < std::shared_ptr<LOCALIZED_COLLIDER>>colliders;
    ENVIRONMENTAL_AUDIO_DATA* data;

    /// <summary>
    /// <para> Creates a localized collider and insert it into the dataset </para>
    /// <para> LOCALIZED_COLLIDER�𐶐����A�f�[�^�Z�b�g�ɓo�^ </para>
    /// </summary>
    void CreateLocalizedCollider(COMPONENT_TYPE t);
    friend LOCALIZED_COLLIDER;
public:
    ENVIRONMENTAL_AUDIO() {};
    ENVIRONMENTAL_AUDIO(GAMEOBJECT* t, COMPONENT_DATA* d);
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
    std::vector<std::shared_ptr<LOCALIZED_COLLIDER>>Colliders();

};


CEREAL_REGISTER_TYPE(ENVIRONMENTAL_AUDIO_DATA)
