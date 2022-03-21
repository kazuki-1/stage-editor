#pragma once
#include "../Base Classes/COMPONENT.h"

class ENEMY_CONTROLLER_DATA : public CHARACTER_DATA
{
public:
    ENEMY_CONTROLLER_DATA() { type = COMPONENT_TYPE::ENEMY_CONTROLLER; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<CHARACTER_DATA>(this));
    }
};



class ENEMY_CONTROLLER : public BASE_CHARACTER_COMPONENT
{   
    ENEMY_CONTROLLER_DATA* data;
public:
    ENEMY_CONTROLLER(GAMEOBJECT* g, COMPONENT_DATA* d);
    HRESULT Initialize() override;
    void Execute() override;
    void UI() override;
    void Render() override;
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(ENEMY_CONTROLLER_DATA)