#pragma once
#include "./Base Classes/Component.h"
class OBBCollider_Component;
class OBBCollider_Data;
class BGMComponent_Data : public AudioData
{
public:
    BGMComponent_Data() { type = COMPONENT_TYPE::BGM; };
    std::vector<std::shared_ptr<AudioData>>dataset;
    std::shared_ptr<OBBCollider_Data>collider_data;
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AudioData>(this), dataset, collider_data);
    }
};


class BGM_Component : public BASE_AUDIO_COMPONENT
{
    std::vector<std::shared_ptr<AUDIO>>audioList;
    std::shared_ptr<OBBCollider_Component>collider;
    BGMComponent_Data* data;
public:
    BGM_Component() {};
    BGM_Component(GameObject* t, ComponentData* d);
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void UI() override;
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(BGMComponent_Data)