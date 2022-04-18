#pragma once
#include "Base Classes/Component.h"

class MeshCollider_Data : public ComponentData
{
public:
    MeshCollider_Data() { type = COMPONENT_TYPE::MESH_COL; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this));
    }
};


class MeshCollider_Component : public Component
{
    MeshCollider_Data* data;

public:
    // Check last state and gives true on moment of collision
    bool trigger;
    MeshCollider_Component() {};
    MeshCollider_Component(GameObject* t, ComponentData* data);

    /// <summary>
    /// <para> Called when created from data </para>
    /// <para> データから生成時に呼び出す </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Perform rendering functions </para>
    /// <para> 描画関数を呼び出す </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI </para>
    /// <para> UIを描画 </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// <para> Sets the trigger state to true,</para>
    /// <para> トリガーステートをTrueに設定する </para>
    /// </summary>
    void Triggered();
    /// <summary>
    /// <para> Sets the trigger state to false,</para>
    /// <para> トリガーステートをfalseに設定する </para>
    /// </summary>
    void NotTriggered();
    /// <summary>
    /// <para> Returns the boolean member trigger </para>
    /// <para> triggerメンバーを返す </para>
    /// </summary>
    /// <returns></returns>
    bool OnTrigger();
};


CEREAL_REGISTER_TYPE(MeshCollider_Data)