#include "../Base Classes/COMPONENT.h"
#include "../Headers/EFFECT.h"

EFFECT_DATA::EFFECT_DATA()
{
    type = COMPONENT_TYPE::EFFECT;
}

EFFECT::EFFECT(GAMEOBJECT* g, COMPONENT_DATA* d)
{
    parent = g;
    data = static_cast<EFFECT_DATA*>(d);
}
//EFFECT::~EFFECT()
//{
//    FX_MANAGER::Instance()->Remove(data->effect_name);
//}
//HRESULT EFFECT::Initialize()
//{
//    FX_MANAGER::Instance()->Insert(data->effect_name, std::make_shared<FX>());
//    return FX_MANAGER::Instance()->Retrieve(data->effect_name)->Initialize(data->effect_path);
//
//}
//void EFFECT::Execute()
//{
//
//}
//void EFFECT::Render()
//{
//
//}
//void EFFECT::UI()
//{
//
//}
//std::string EFFECT::EffectPath()
//{
//    return data->effect_path;
//}
//std::string EFFECT::EffectName()
//{
//    return data->effect_name;
//}
//VECTOR3 EFFECT::Position()
//{
//    return data->position;
//}
//EFFECT_DATA* EFFECT::Data()
//{
//    return data;
//}

