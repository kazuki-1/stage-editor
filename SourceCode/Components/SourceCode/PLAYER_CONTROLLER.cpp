#include "../Headers/PLAYER_CONTROLLER.h"
#include "../../Engine/Input.h"
#include "../../Engine/Camera.h"
#include "../Headers/TRANSFORM_3D.h"
#include "../../GAMEOBJECT.h"
#include "../Headers/MESH.h"
#include "../Headers/MESH_COLLIDER.h"
#include "../Headers/ENVIRONMENTAL_AUDIO.h"
#include "../Headers/CAPSULE_COLLIDER.h"
#include "../Headers/TERRAIN_AUDIO.h"
#define RAYCAST COLLIDERS::RAYCAST_MANAGER::Instance()->Collide
std::vector<bool>statuses;
std::vector<VECTOR3>receivers;

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
    move = Math::Clamp(move, 0, 0.2f);
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
    float new_Angle = y_ax + t_Angle;
    VECTOR3 rotation = transform->Rotation();
    rotation.y = new_Angle;
    transform->SlerpRotation(rotation);





}

/*--------------------------------------------------------PLAYER_CONTROLLER JumpInput()-------------------------------------------------*/
/// <summary>
/// <para> SPACE jump input</para>
/// <para> SPACE ジャンプ入力</para>
/// </summary>    
void PLAYER_CONTROLLER::JumpInput()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    if (INPUTMANAGER::Instance()->Keyboard()->Triggered(VK_SPACE))
    {
        VECTOR3 velocity{ transform->Velocity() };
        VECTOR3 position{ transform->Translation() };
        velocity.y += 5.0f;
        position.y += 0.3f;
        transform->SetTranslation(position);
        transform->SetVelocity(velocity);

    }
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
                if (audio->Audio()->IsDucking())
                    volume = min(volume, 0.3f);
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

/*--------------------------------------------------------PLAYER_CONTROLLER VelocityControl()-------------------------------------------------*/

void PLAYER_CONTROLLER::VelocityControl()
{
}

/*--------------------------------------------------------PLAYER_CONTROLLER GravityControl()----------------------------------------------------*/
/// <summary>
/// <para> Performs gravity control </para>
/// <para> 重力による落下処理の調整を行う </para>
/// </summary>
void PLAYER_CONTROLLER::GravityControl()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    VECTOR3 velocity = transform->Velocity();
    velocity.y -= 0.1f;
    velocity.y = min(velocity.y, 15.0f);
    transform->SetVelocity(velocity);
}

/*--------------------------------------------------------PLAYER_CONTROLLER GroundCollision()----------------------------------------------------*/
/// <summary>
/// <para> Calculates the velocity based on collision with anything below the player </para>
/// <para> 他のモデルが衝突によって加速度の調整を行う </para>
/// </summary>
void PLAYER_CONTROLLER::GroundCollision()
{
    // This is only for mesh colliders
    MESH_COLLIDER* collider{ GetComponent<MESH_COLLIDER>() };
    if (!collider)
        return;

    // Prepare parameters for rayCasting
    MESH* mesh{ GetComponent<MESH>() };
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
    VECTOR3 start, end;
    start = transform->Translation();
    end = transform->Translation();
    end.y += transform->Velocity().y;

    COLLIDERS::RAYCASTDATA rcd{};
    static bool last_state{};
    bool collided{ COLLIDERS::RAYCAST_MANAGER::Instance()->Collide(start, end, mesh, rcd) };

    // Update trigger state of mesh collider
    if (last_state != collided && collided == true)
        collider->Triggered();
    else
        collider->NotTriggered();
    last_state = collided;

    // Offsets position when collided
    if (collided)
    {
        VECTOR3 normal, position;
        normal = rcd.normal;
        transform->SetTranslation(rcd.position);
        VECTOR3 velocity;
        velocity = transform->Velocity();
        velocity.y = 0;
        transform->SetVelocity(velocity);

    }

}

/*--------------------------------------------------------PLAYER_CONTROLLER WallCollision()----------------------------------------------------*/
/// <summary>
/// <para> Calculates the velocity based on collision with anything in front the player </para>
/// <para> 他のモデルが衝突によって加速度の調整を行う </para>
/// </summary>
void PLAYER_CONTROLLER::WallCollision()
{
    // This is only for mesh colliders
    MESH_COLLIDER* collider{ GetComponent<MESH_COLLIDER>() };
    if (!collider)
        return;

    // Prepare parameters for rayCasting
    MESH* mesh{ GetComponent<MESH>() };
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
    VECTOR3 start, end;
    start = transform->Translation();
    end = transform->Translation();
    end.x += transform->Velocity().x;
    end.z += transform->Velocity().z;
    COLLIDERS::RAYCASTDATA rcd{};
    static bool last_state{};
    bool collided{ COLLIDERS::RAYCAST_MANAGER::Instance()->Collide(start, end, mesh, rcd) };

    // Update trigger state of mesh collider
    if (last_state != collided && collided == true)
        collider->Triggered();
    else
        collider->NotTriggered();
    last_state = collided;

    // Offsets position when collided
    if (collided)
    {
        VECTOR3 normal, position;
        normal = rcd.normal;
        rcd.position += normal * 0.5f;
        transform->SetTranslation(rcd.position);
        VECTOR3 velocity;
        velocity = transform->Velocity();
        velocity.y = 0;
        transform->SetVelocity(velocity);

    }

}

/*--------------------------------------------------------PLAYER_CONTROLLER TerrainAudioCollision()-------------------------------------------------*/
/// <summary>
/// <para> Performs collision check for TERRAIN_AUDIO component </para>
/// <para> TERRAIN_AUDIOコンポネントの当たり判定を計算する </para>
/// </summary>
/// 
void PLAYER_CONTROLLER::TerrainAudioCollision()
{
    // Check current gameObject for terrain audio component
    TERRAIN_AUDIO* receiver{ GetComponent<TERRAIN_AUDIO>() };
    if (!receiver || receiver->Property() != TERRAIN_AUDIO_PROPERTY::RECEIVER)
        return;

    // Check other gameObjects for terrain audio component
    std::vector<GAMEOBJECT*>emitters;
    for (auto& g : GAMEOBJECT_MANAGER::Instance()->GameObjects())
    {
        if (g.second.get() == parent)
            continue;
        if (g.second->GetComponent<TERRAIN_AUDIO>() != nullptr
            && g.second->GetComponent<TERRAIN_AUDIO>()->Data()->class_type == TERRAIN_AUDIO_PROPERTY::EMITTER)
            emitters.push_back(g.second.get());

    }

    // Prepare coordinates of collision
    TERRAIN_AUDIO_DATA_RECEIVER* rec{ (TERRAIN_AUDIO_DATA_RECEIVER*)receiver->Data()->property_data.get() };
    receivers.resize(rec->receiver_bones.size());

    MESH* mesh{ GetComponent<MESH>() };
    for (auto& names : ((TERRAIN_AUDIO_DATA_RECEIVER*)(receiver->Data()->property_data.get()))->receiver_bones)
    {
    }
    int index{};
    for (auto& v : receivers)
    {
        XMMATRIX bone_transform = mesh->GetBoneTransform(rec->receiver_bones[index]);
        XMVECTOR s, r, t;
        XMMatrixDecompose(&s, &r, &t, bone_transform);
        v.Load(t);
        ++index;
    }

    // Prepare a buffer to check collider state
    if (statuses.size() < 1)
        statuses.resize(receivers.size());

    MESH_COLLIDER* collider = GetComponent<MESH_COLLIDER>();

    // Perform rayCast on receivers
    for (auto& e : emitters)
    {
        TERRAIN_AUDIO* emitter{ e->GetComponent<TERRAIN_AUDIO>() };
        MESH* emitter_mesh{ e->GetComponent<MESH>() };
        for (int ind = 0; ind < 2; ++ind)
        {
            for (auto& b : emitter->Buffers())
            {
                if (b.buffer->IsDucking())
                    b.buffer->SetVolume(0.3f);
            }
            VECTOR3 start{ receivers[ind] };
            VECTOR3 end{ start };
            end.y -= 0.005f;
            COLLIDERS::RAYCASTDATA rcd{};
            bool collided{ RAYCAST(start, end, emitter_mesh, ((TERRAIN_AUDIO_DATA_EMITTER*)emitter->Data())->mesh_index, rcd) };
            bool triggered{};
            if (collided && statuses[ind] != collided)
                triggered = true;
            statuses[ind] = collided;
            if (triggered)
                emitter->Play();
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
    VECTOR3 position = GetComponent<TRANSFORM_3D>()->Translation();
    position.y += 5.0f;
    Camera::Instance()->SetTarget(position);
    MovementInput();
    RotationInput();
    JumpInput();
    SoundCollision();
    GravityControl();
    GroundCollision();
    WallCollision();
    VelocityControl();
    TerrainAudioCollision();
    AnimationSettings();

}

/*--------------------------------------------------------PLAYER_CONTROLLER Render()-------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void PLAYER_CONTROLLER::Render()
{
    VECTOR3 velocity, position;
    velocity = GetComponent<TRANSFORM_3D>()->Velocity();
    position = GetComponent<TRANSFORM_3D>()->Translation();
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
    VECTOR3 velocity = transform->Velocity();
    velocity.y = 0;
    INPUTMANAGER::KEYBOARD* kb{ INPUTMANAGER::Instance()->Keyboard().get() };
    if (velocity.Length() >= 0.05f)
        mesh->Model()->SetTake((int)PLAYER_ANIM::WALK);
    else
        mesh->Model()->SetTake((int)PLAYER_ANIM::T_POSE);

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

