#pragma once
#include "Base Classes/COMPONENT.h"
class DIRECTIONAL_LIGHT_DATA : public COMPONENT_DATA
{
public:
    DIRECTIONAL_LIGHT_DATA();
    Vector3 direction{};
    Vector4 colour{};
    template <class T>
    void serialize(T& t)
    {
        t(direction, colour);
    }
};


class DIRECTIONAL_LIGHT : public COMPONENT
{
    std::shared_ptr<LIGHTING>light;
    DIRECTIONAL_LIGHT_DATA* data;
public:
    DIRECTIONAL_LIGHT(GAMEOBJECT* t, COMPONENT_DATA* data);

    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void UI() override;

    Vector3 Direction();
    Vector4 Colour();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(DIRECTIONAL_LIGHT_DATA)
