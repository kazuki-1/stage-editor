#include "../Headers/PLAYER_CONTROLLER.h"
#include "../../Engine/Input.h"
#include "../../Engine/Camera.h"
#include "../Headers/TRANSFORM_3D.h"
#include "../../GAMEOBJECT.h"
#include "../Headers/MESH.h"
#include "../Headers/ENVIRONMENTAL_AUDIO.h"
#include "../Headers/CAPSULE_COLLIDER.h"

/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------PLAYER_CONTROLLER Class-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------PLAYER_CONTROLLER Constructor-------------------------------------------------*/

PLAYER_CONTROLLER::PLAYER_CONTROLLER(GAMEOBJECT* g, COMPONENT_DATA* d)
{
    parent = g;
    data = static_cast<PLAYER_CONTROLLER_DATA*>(d);
    
}

/*--------------------------------------------------------PLAYER_CONTROLLER MovementInput()-------------------------------------------------*/
/// <summary>
/// <para> WASD movement input </para>
/// <para> WASD 移動入力 </para>
/// </summary>
void PLAYER_CONTROLLER::MovementInput()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    static VECTOR3 d_Rot{ Camera::Instance()->Rotation() };
    static float move{};
    VECTOR2 k_Axis{ INPUTMANAGER::Instance()->Keyboard()->AxisX() };
    if (!k_Axis.x && !k_Axis.y)
    {
        move = 0;
        return;
    }
    VECTOR2 limit{ -20.0f, 20.0f };
    if (k_Axis.x || k_Axis.y)
        move += 0.005f;
    move = Math::Clamp(move, 0, 0.6f);
    fabs(move) < 0.0001f ? move = 0 : move = move;
    VECTOR3 f;
    f = GetComponent<TRANSFORM_3D>()->Forward();
    f = { f.x, 0, f.z };
    f.Normalize();
    VECTOR3 output{};
    output.x = (f * move).x;
    output.z = (f * move).z;
    transform->SetVelocity(output);
}

/*--------------------------------------------------------PLAYER_CONTROLLER RotationInput()-------------------------------------------------*/
/// <summary>
/// <para> WASD rotation input </para>
/// <para> WASD 回転入力 </para>
/// </summary>
void PLAYER_CONTROLLER::RotationInput()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    VECTOR2 ax{ INPUTMANAGER::Instance()->Keyboard()->AxisX() };
    if (!ax.x && !ax.y)
    {
        //isRotating = false;
        return;
    }
    float y_ax{ Camera::Instance()->Rotation().y };

    

    VECTOR3 d_Front{ 0, 0, 1 }, d_Right{ 1, 0, 0 };
    VECTOR3 input{ ax.x, 0, ax.y };
    float t_Angle{ Math::GetDirection(d_Front, input) };
    if (d_Right.Dot(input) < 0)
        t_Angle *= -1;
     float new_Angle = y_ax + t_Angle ;
     VECTOR3 rotation = transform->Rotation();
     rotation.y = new_Angle;
     transform->SlerpRotation(rotation);





}

/*--------------------------------------------------------PLAYER_CONTROLLER SoundCollision()-------------------------------------------------*/
/// <summary>
/// <para> Perform collision check with ENVIRONMENTAL_AUDIO gameObjects </para>
/// <para> ENVIRONMENTAL_AUDIOゲームオブジェクトと当たり判定を計算 </para>
/// </summary>
void PLAYER_CONTROLLER::SoundCollision()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    CAPSULE_COLLIDER* capsule = GetComponent<CAPSULE_COLLIDER>();
    for (auto& g : GAMEOBJECT_MANAGER::Instance()->GameObjects())
    {
        bool no_sound_comp{};
        ENVIRONMENTAL_AUDIO* audio = g.second->GetComponent<ENVIRONMENTAL_AUDIO>();
        if (!audio)
            continue;

        bool collided{};
        for (auto& c : audio->Colliders())
        {
            if (c->Collider()->Collide(transform->Translation()))
            {
                if (!audio->Audio()->IsPlaying())
                {
                    audio->Audio()->Play();
                }
                VECTOR3 closest_dist{ c->Collider()->DistanceToPlayer(this) };
                closest_dist -= transform->Translation();
                float length{ closest_dist.Length() };
                length = max(c->Data()->minimum_distance, length);
                float volume = 1.0f - (length / c->Collider()->Collider()->Size());
                audio->Audio()->SourceVoice()->SetVolume(volume);
                collided = true;
                break;
            }
        }
        if (!collided)
        {
            audio->Audio()->Stop();
        }
    }
}

/*--------------------------------------------------------PLAYER_CONTROLLER Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT PLAYER_CONTROLLER::Initialize()
{
    return S_OK;

}

/*--------------------------------------------------------PLAYER_CONTROLLER Execute()-------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void PLAYER_CONTROLLER::Execute()
{
    //VECTOR3 position = GetComponent<TRANSFORM_3D>()->Translation();
    //position.y += 5.0f;
    //Camera::Instance()->SetTarget(position);
    //MovementInput();
    //RotationInput();
    //SoundCollision();
}

/*--------------------------------------------------------PLAYER_CONTROLLER Render()-------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void PLAYER_CONTROLLER::Render()
{

}

/*--------------------------------------------------------PLAYER_CONTROLLER UI()-------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void PLAYER_CONTROLLER::UI()
{
    if (ImGui::TreeNode("Player Controller"))
    {
        ImGui::InputText("Name : ", data->name, 256);
        ImGui::TreePop();
    }
}

/*--------------------------------------------------------PLAYER_CONTROLLER AnimationSettings()-------------------------------------------------*/
/// <summary>
/// <para> All input functions are called here </para>
/// <para> すべての入力関数を呼び出す</para>
/// </summary>
void PLAYER_CONTROLLER::Input()
{
    
}

/*--------------------------------------------------------PLAYER_CONTROLLER Input()-------------------------------------------------*/
/// <summary>
/// <para> Perform animation switching </para>
/// <para> アニメーション変更処理 </para>
/// </summary>
void PLAYER_CONTROLLER::AnimationSettings()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    MESH* mesh = GetComponent<MESH>();
    if (transform->Velocity().Length() >= 0.01f)
        mesh->Model()->SetTake((int)PLAYER_ANIM::WALK);

}

/*--------------------------------------------------------PLAYER_CONTROLLER Hit()-------------------------------------------------*/
/// <summary>
/// <para> Called when hit </para>
/// <para> 攻撃されたら呼び出す </para>
/// </summary>
void PLAYER_CONTROLLER::Hit()
{

}

/*--------------------------------------------------------PLAYER_CONTROLLER Kill()-------------------------------------------------*/
/// <summary>
/// <para> Called when killed </para>
/// <para> 殺されたら呼び出す </para>
/// </summary>
void PLAYER_CONTROLLER::Kill()
{

}

/*--------------------------------------------------------PLAYER_CONTROLLER UpdateRotation()-------------------------------------------------*/
/// <summary>
/// <para> Updates rotation</para>
/// <para> 回転を更新</para>
/// </summary>
void PLAYER_CONTROLLER::UpdateRotation()
{

}

/*--------------------------------------------PLAYER_CONTROLLER GetComponentType()--------------------------------------------------*/

COMPONENT_TYPE PLAYER_CONTROLLER::GetComponentType()
{
    return data->type;
}