#pragma once
#include "../Base Classes/COMPONENT.h"

class CAMERA_DATA : public COMPONENT_DATA
{

};



class CAMERA : public COMPONENT
{
    CAMERA_DATA* data;
public:
    CAMERA(GAMEOBJECT* g, COMPONENT_DATA* d);
};

CEREAL_REGISTER_TYPE(CAMERA_DATA)