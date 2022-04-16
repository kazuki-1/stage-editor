#pragma once
#include "Base Classes/Component.h"

class EnemyController_Data : public CharacterData
{
public:
    EnemyController_Data() { type = COMPONENT_TYPE::ENEMY_CONTROLLER; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<CharacterData>(this));
    }
};



class EnemyController : public BaseCharacterComponent
{   
    EnemyController_Data* data;
public:
    EnemyController(GameObject* g, ComponentData* d);
    HRESULT Initialize() override;
    void Execute() override;
    void UI() override;
    void Render() override;
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(EnemyController_Data)