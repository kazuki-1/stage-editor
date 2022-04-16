#pragma once
#include "./Base Classes/Component.h"

class CAMERA_DATA : public ComponentData
{

};



class CAMERA : public Component
{
    CAMERA_DATA* data;
public:
    CAMERA(GameObject* g, ComponentData* d);
};

CEREAL_REGISTER_TYPE(CAMERA_DATA)