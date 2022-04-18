#pragma once
#include "./Base Classes/Component.h"

class Camera_Data : public ComponentData
{

};



class Camera_Component : public Component
{
    Camera_Data* data;
public:
    Camera_Component(GameObject* g, ComponentData* d);
};

CEREAL_REGISTER_TYPE(Camera_Data)