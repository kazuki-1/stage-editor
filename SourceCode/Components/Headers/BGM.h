#pragma once
#include "../Base Classes/COMPONENT.h"
class OBB_COLLIDER;
class OBB_COLLIDER_DATA;
class BGM_DATA : public AUDIO_DATA
{
public:
    BGM_DATA() { type = COMPONENT_TYPE::BGM; };
    std::vector<std::shared_ptr<AUDIO_DATA>>dataset;
    std::shared_ptr<OBB_COLLIDER_DATA>collider_data;
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AUDIO_DATA>(this), dataset, collider_data);
    }
};


class BGM : public BASE_AUDIO_COMPONENT
{
    std::vector<std::shared_ptr<AUDIO>>audioList;
    std::shared_ptr<OBB_COLLIDER>collider;
    BGM_DATA* data;
public:
    BGM() {};
    BGM(GAMEOBJECT* t, COMPONENT_DATA* d);
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void UI() override;
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(BGM_DATA)