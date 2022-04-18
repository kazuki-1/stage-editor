#pragma once
#include "Base Classes/Component.h"
class DirectionalLight_Data : public ComponentData
{
public:
    DirectionalLight_Data();
    Vector3 direction{};
    Vector4 colour{};
    template <class T>
    void serialize(T& t)
    {
        t(direction, colour);
    }
};


class DirectionalLight_Component : public Component
{
    std::shared_ptr<LIGHTING>light;
    DirectionalLight_Data* data;
public:
    DirectionalLight_Component(GameObject* t, ComponentData* data);

    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void UI() override;

    Vector3 Direction();
    Vector4 Colour();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(DirectionalLight_Data)
