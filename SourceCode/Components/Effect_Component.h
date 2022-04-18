#pragma once
#include "Base Classes/Component.h"

class Effect_Data : public ComponentData
{
public:
    std::string effect_path;
    std::string effect_name;
    Vector3 position;
    float scale;

    Effect_Data();
    template <class T>
    void serialize(T& t)
    {
        t(effect_path, position, scale);
    }

};

class Effect_Component : public Component
{
    //    std::shared_ptr<FX>fx;
    Effect_Data* data;
public:
    Effect_Component(GameObject* g, ComponentData* d);
    //~Effect_Component();
    //HRESULT Initialize() override;
    //void Execute() override;
    //void Render() override;
    //void UI() override;

    //std::string EffectPath();
    //std::string EffectName();
    //VECTOR3 Position();
    //Effect_Data* Data();
};

CEREAL_REGISTER_TYPE(Effect_Data)