#pragma once
#include "Base Classes/Component.h"

class OBBCollider_Data : public ComponentData
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 offset{};
    Vector3 min{};
    Vector3 max{};

    OBBCollider_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), name, bone_name, min, max);
    }
};


class OBBCollider_Component : public BaseColliderComponent
{
    std::shared_ptr<COLLIDERS::OBB>collider;
    std::shared_ptr<DYNAMIC_CUBE>cube;
    OBBCollider_Data* data;
public:
    OBBCollider_Component() {};
    OBBCollider_Component(GameObject* t, ComponentData* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (OBBCollider_Data)</para>
    /// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBBCollider_Data) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute(XMMATRIX transform) override;
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
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> プレイヤーとの距離を計算する </para>
    /// </summary>
    Vector3 DistanceToPlayer(PlayerController_Component* target) override;
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> プレイヤーとの距離を計算する </para>
    /// </summary>
    bool Collide(GameObject* target) override;
    /// <summary>
    /// <para> Perform collision check based on coordinates </para>
    /// <para> 座標点を基づいて当たり判定を計算 </para>
    /// </summary>
    /// <returns></returns>
    bool Collide(Vector3 p) override;
    /// <summary>
    /// Returns the name of the collider
    /// </summary>
    /// <returns></returns>
    const std::string& ColliderName();
    /// <summary>
    /// Returns the name of the bone that the collider is attached to
    /// </summary>
    /// <returns></returns>
    const std::string& BoneName();
    std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider();
    /// <summary>
    /// Returns the minimum point of the collider
    /// </summary>
    /// <returns></returns>
    Vector3 Min();
    /// <summary>
    /// Returns the maximum point of the collider
    /// </summary>
    /// <returns></returns>
    Vector3 Max();
    /// <summary>
    /// Returns the data class that stores all the data used in this component
    /// </summary>
    /// <returns></returns>
    OBBCollider_Data* Data();
    COMPONENT_TYPE GetComponentType() override;


};

CEREAL_REGISTER_TYPE(OBBCollider_Data)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ComponentData, OBBCollider_Data)