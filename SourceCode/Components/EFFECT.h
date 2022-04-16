#pragma once
#include "Base Classes/Component.h"

class EFFECT_DATA : public ComponentData
{
public:
    std::string effect_path;
    std::string effect_name;
    Vector3 position;
    float scale;

    EFFECT_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(effect_path, position, scale);
    }

};

class EFFECT : public Component
{
    //    std::shared_ptr<FX>fx;
    EFFECT_DATA* data;
public:
    EFFECT(GameObject* g, ComponentData* d);
    //~EFFECT();
    //HRESULT Initialize() override;
    //void Execute() override;
    //void Render() override;
    //void UI() override;

    //std::string EffectPath();
    //std::string EffectName();
    //VECTOR3 Position();
    //EFFECT_DATA* Data();
};

CEREAL_REGISTER_TYPE(EFFECT_DATA)