#pragma once
#include "Base Classes/Component.h"

enum class PlayerAnim
{
    T_Pose,
    Walking,
    Idle,
    Jump,
    Landing

};


class PlayerController_Data : public CharacterData
{
public:
    PlayerController_Data() { type = COMPONENT_TYPE::PLAYER_CONTROLLER; }
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<CharacterData>(this));
    }
};


class PlayerController_Component : public BaseCharacterComponent
{
    PlayerController_Data* data;

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
    /// <para> SPACE jump input</para>
    /// <para> SPACE ジャンプ入力</para>
    /// </summary>    
    void JumpInput();
    /// <summary>
    /// <para> Perform collision check with EnvironmentalAudio_Component gameObjects </para>
    /// <para> ENVIRONMENTAL_AUDIOゲームオブジェクトと当たり判定を計算 </para>
    /// </summary>
    void SoundCollision();
    /// <summary>
    /// <para> Performs velocity control </para>
    /// <para> 加速度の調整を行う </para>
    /// </summary>
    void VelocityControl();
    /// <summary>
    /// <para> Performs gravity control </para>
    /// <para> 重力による落下処理の調整を行う </para>
    /// </summary>
    void GravityControl();
    /// <summary>
    /// <para> Calculates the velocity based on collision with anything below the player </para>
    /// <para> 他のモデルが衝突によって加速度の調整を行う </para>
    /// </summary>
    void GroundCollision();
    /// <summary>
    /// <para> Calculates the velocity based on collision with anything in front the player </para>
    /// <para> 他のモデルが衝突によって加速度の調整を行う </para>
    /// </summary>
    void WallCollision();
    /// <summary>
    /// <para> Performs collision check for TerrainAudio_Component component </para>
    /// <para> TERRAIN_AUDIOコンポネントの当たり判定を計算する </para>
    /// </summary>
    void TerrainAudioCollision();

    bool inDialogue{};
public:
    PlayerController_Component(GameObject* g, ComponentData* d);
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

    void NPCDialogueTrigger();
};

CEREAL_REGISTER_TYPE(PlayerController_Data)