#pragma once
#include "../Base Classes/COMPONENT.h"

class EFFECT_DATA : public COMPONENT_DATA
{
public:
    std::string effect_path;
    std::string effect_name;
    VECTOR3 position;
    float scale;

    EFFECT_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(effect_path, position, scale);
    }

};

class EFFECT : public COMPONENT
{
    //    std::shared_ptr<FX>fx;
    EFFECT_DATA* data;
public:
    EFFECT(GAMEOBJECT* g, COMPONENT_DATA* d);
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