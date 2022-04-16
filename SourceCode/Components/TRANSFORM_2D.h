#pragma once
#include "Base Classes/Component.h"

class TRANSFORM_2D_DATA : public ComponentData
{
public:
    Vector2 position;
    float rotation;

    TRANSFORM_2D_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), position, rotation);
    }
};


/// <summary>
/// Class to manage the transform of a 2D gameobject. 2D GetGameObjects will generally have one when created
/// </summary>
class TRANSFORM_2D : public Component
{
    TRANSFORM_2D_DATA* data;
public:
    TRANSFORM_2D() {};
    TRANSFORM_2D(GameObject* t, ComponentData* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
    /// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UIを描画する関数 </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// Returns the position
    /// </summary>
    /// <returns></returns>
    Vector2 Position();
    /// <summary>
    /// Returns the angle of rotation
    /// </summary>
    /// <returns></returns>
    float Rotation();
    /// <summary>
    /// Returns the data class that stores all the data used in this component
    /// </summary>
    /// <returns></returns>
    TRANSFORM_2D_DATA* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(TRANSFORM_2D_DATA)
