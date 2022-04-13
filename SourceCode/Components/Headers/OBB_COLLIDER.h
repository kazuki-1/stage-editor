#pragma once
#include "../Base Classes/COMPONENT.h"

class OBB_COLLIDER_DATA : public COMPONENT_DATA
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 offset{};
    Vector3 min{};
    Vector3 max{};

    OBB_COLLIDER_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), name, bone_name, min, max);
    }
};


class OBB_COLLIDER : public BASE_COLLIDER_COMPONENT
{
    std::shared_ptr<COLLIDERS::OBB>collider;
    std::shared_ptr<DYNAMIC_CUBE>cube;
    OBB_COLLIDER_DATA* data;
public:
    OBB_COLLIDER() {};
    OBB_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data);
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
    Vector3 DistanceToPlayer(PLAYER_CONTROLLER* target) override;
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> プレイヤーとの距離を計算する </para>
    /// </summary>
    bool Collide(GAMEOBJECT* target) override;
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
    OBB_COLLIDER_DATA* Data();
    COMPONENT_TYPE GetComponentType() override;


};

CEREAL_REGISTER_TYPE(OBB_COLLIDER_DATA)
CEREAL_REGISTER_POLYMORPHIC_RELATION(COMPONENT_DATA, OBB_COLLIDER_DATA)