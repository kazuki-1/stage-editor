#include "Base Classes/Component.h"
#include "Effect_Component.h"

Effect_Data::Effect_Data()
{
    type = COMPONENT_TYPE::EFFECT;
}

Effect_Component::Effect_Component(GameObject* g, ComponentData* d)
{
    parent = g;
    data = static_cast<Effect_Data*>(d);
}
//Effect_Component::~Effect_Component()
//{
//    FX_MANAGER::Instance()->Remove(data->effect_name);
//}
//HRESULT Effect_Component::Initialize()
//{
//    FX_MANAGER::Instance()->Insert(data->effect_name, std::make_shared<FX>());
//    return FX_MANAGER::Instance()->Retrieve(data->effect_name)->Initialize(data->effect_path);
//
//}
//void Effect_Component::Execute()
//{
//
//}
//void Effect_Component::Render()
//{
//
//}
//void Effect_Component::UI()
//{
//
//}
//std::string Effect_Component::EffectPath()
//{
//    return data->effect_path;
//}
//std::string Effect_Component::EffectName()
//{
//    return data->effect_name;
//}
//VECTOR3 Effect_Component::Position()
//{
//    return data->position;
//}
//Effect_Data* Effect_Component::Data()
//{
//    return data;
//}

