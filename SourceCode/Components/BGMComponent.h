#pragma once
#include "./Base Classes/Component.h"
class OBB_COLLIDER;
class OBB_COLLIDER_DATA;
class BGMComponent_Data : public AudioData
{
public:
    BGMComponent_Data() { type = COMPONENT_TYPE::BGM; };
    std::vector<std::shared_ptr<AudioData>>dataset;
    std::shared_ptr<OBB_COLLIDER_DATA>collider_data;
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AudioData>(this), dataset, collider_data);
    }
};


class BGMComponent : public BASE_AUDIO_COMPONENT
{
    std::vector<std::shared_ptr<AUDIO>>audioList;
    std::shared_ptr<OBB_COLLIDER>collider;
    BGMComponent_Data* data;
public:
    BGMComponent() {};
    BGMComponent(GameObject* t, ComponentData* d);
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void UI() override;
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(BGMComponent_Data)