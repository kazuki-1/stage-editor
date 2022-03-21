#pragma once
#include "../Base Classes/COMPONENT.h"

enum class PLAYER_ANIM
{
    T_POSE,
    WALK,
    IDLE,
    JUMP,
    LANDING

};


class PLAYER_CONTROLLER_DATA : public CHARACTER_DATA
{
public:
    PLAYER_CONTROLLER_DATA() { type = COMPONENT_TYPE::PLAYER_CONTROLLER; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<CHARACTER_DATA>(this));
    }
};



class PLAYER_CONTROLLER : public BASE_CHARACTER_COMPONENT
{
    PLAYER_CONTROLLER_DATA* data;

    /// <summary>
    /// <para> WASD movement input </para>
    /// <para> WASD 移動入力 </para>
    /// </summary>
    void MovementInput();
    /// <summary>
    /// <para> WASD rotation input </para>
    /// <para> WASD 回転入力 </para>
    /// </summary>
    void RotationInput();
    /// <summary>
    /// <para> Perform collision check with ENVIRONMENTAL_AUDIO gameObjects </para>
    /// <para> ENVIRONMENTAL_AUDIOゲームオブジェクトと当たり判定を計算 </para>
    /// </summary>
    void SoundCollision();





public:
    PLAYER_CONTROLLER(GAMEOBJECT* g, COMPONENT_DATA* d);
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
    /// <summary>
    /// <para> All input functions are called here </para>
    /// <para> すべての入力関数を呼び出す</para>
    /// </summary>
    void Input();
    /// <summary>
    /// <para> Perform animation switching </para>
    /// <para> アニメーション変更処理 </para>
    /// </summary>
    void AnimationSettings() override;
    /// <summary>
    /// <para> Called when hit </para>
    /// <para> 攻撃されたら呼び出す </para>
    /// </summary>
    void Hit() override;
    /// <summary>
    /// <para> Called when killed </para>
    /// <para> 殺されたら呼び出す </para>
    /// </summary>
    void Kill() override;
    /// <summary>
    /// <para> Updates rotation</para>
    /// <para> 回転を更新</para>
    /// </summary>
    void UpdateRotation() override;
    COMPONENT_TYPE GetComponentType() override;
};

CEREAL_REGISTER_TYPE(PLAYER_CONTROLLER_DATA)