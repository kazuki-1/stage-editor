#pragma once
#include "Base Classes/Component.h"

class SpotLight_Data : public ComponentData
{
public:
    SpotLight_Data();
    Vector3 position{};
    Vector3 direction{};
    Vector4 colour{};
    float range{};
    float innerCorner{};
    float outerCorner{};
    float placeholder{};
    template <class T>
    void serialize(T& t)
    {
        t(position, direction, colour, range, innerCorner, outerCorner, placeholder);
    }
};


class SpotLight_Component : public Component
{
    std::shared_ptr<LIGHTING>light;
    SpotLight_Data* data;
public:
    SpotLight_Component(GameObject* t, ComponentData* data);


    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SpotLight_Data)</para>
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
    Vector3 Direction();
    Vector4 Colour();
    float Range();
    float InnerCorner();
    float OuterCorner();
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(SpotLight_Data)
