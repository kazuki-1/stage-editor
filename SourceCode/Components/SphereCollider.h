#pragma once
#include "Base Classes/Component.h"

class SphereCollider_Data : public ComponentData
{
public:
    char name[256] = "";
    std::string bone_name{};
    Vector3 center{};
    float radius{};
    SphereCollider_Data();
    template <class T>
    void serialize(T& t)
    {
        t(name, bone_name, center, radius);
    }
};


class SphereCollider_Component : public BaseColliderComponent
{
    std::shared_ptr<COLLIDERS::SPHERE>collider;
    std::shared_ptr<DYNAMIC_SPHERE>sphere;
    SphereCollider_Data* data;
public:
    SphereCollider_Component() {};
    /// <summary>
    /// <para> Call this constructor when creating this component. Registers the parent of the component and the data linked to the component </para>
    /// <para> コンポネントを生成時呼び出したください。コンポネントのParentとデータを登録する </para>
    /// </summary>
    /// <param name="t"> : Parent of component (GameObject class)</param>
    /// <param name="d"> : Data used by the component</param>
    SphereCollider_Component(GameObject* t, ComponentData* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SphereCollider_Data) </para>
    /// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
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
    /// <para>Called after Execute to perform any render functions </para>
    /// <para>Execute実行後に描画用関数を呼び出す関数</para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UIを描画する関数 </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// <para> Calculates the distance to the player </para>
    /// <para> プレイヤーとの距離を計算する </para>
    /// </summary>
    Vector3 DistanceToPlayer(PlayerController_Component* target) override;
    /// <summary>
    /// <para> Perform collision check with another gameObject </para>
    /// <para> 引数であるゲームオブジェクトを当たり判定を計算</para>
    /// </summary>
    /// <returns></returns>
    bool Collide(GameObject* target) override;
    /// <summary>
    /// <para> Perform collision check based on coordinates </para>
    /// <para> 座標点を基づいて当たり判定を計算 </para>
    /// </summary>
    /// <returns></returns>
    bool Collide(Vector3 p) override;
    float Radius();
    Vector3 Center();
    std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider() override;
    SphereCollider_Data* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(SphereCollider_Data)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ComponentData, SphereCollider_Data)