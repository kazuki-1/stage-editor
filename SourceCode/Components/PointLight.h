#pragma once
#include "Base Classes/Component.h"

class PointLight_Data : public ComponentData
{
public:
    PointLight_Data();
    Vector3 position{};
    Vector4 colour{};
    float range{};
    Vector3 placeholder{};
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), position, colour, range, placeholder);
    }
};


class PointLight_Component : public Component
{
    std::shared_ptr<LIGHTING>light;
    PointLight_Data* data;
public:
    PointLight_Component(GameObject* t, ComponentData* data);

    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
    /// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Called after Execute() to perform any render functions </para>
    /// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UIを描画 </para>
    /// </summary>
    void UI() override;

    Vector3 Position();
    Vector4 Colour();
    float Range();
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(PointLight_Data)
