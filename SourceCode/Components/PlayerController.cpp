#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../GAMEOBJECT.h"
#include "Transform3D.h"
#include "PlayerController.h"
#include "Mesh.h"
#include "MeshCollider.h"
#include "EnvironmentalAudio.h"
#include "CapsuleCollider.h"
#include "TerrainAudio.h"
#include "Dialogue.h"
#include "../DialogueController.h"
std::vector<bool>statuses;
std::vector<Vector3>receivers;
//std::shared_ptr<DYNAMIC_SPHERE>sphere;


/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------PlayerController_Component Class-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------PlayerController_Component Constructor-------------------------------------------------*/

PlayerController_Component::PlayerController_Component(GameObject* g, ComponentData* d)
{
    parent = g;
    data = static_cast<PlayerController_Data*>(d);

}

/*--------------------------------------------------------PlayerController_Component MovementInput()-------------------------------------------------*/
/// <summary>
/// <para> WASD movement input </para>
/// <para> WASD 移動入力 </para>
/// </summary>
void PlayerController_Component::MovementInput()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    static Vector3 d_Rot{ Camera::Instance()->Rotation() };
    static float move{};
    Vector2 k_Axis{ INPUTMANAGER::Instance()->Keyboard()->AxisX() };
    if (!k_Axis.x && !k_Axis.y)
    {
        move = 0;
        return;
    }
    Vector2 limit{ -20.0f, 20.0f };
    if (k_Axis.x || k_Axis.y)
        move += 0.005f;
    move = Math::Clamp(move, 0, 0.2f);
    fabs(move) < 0.0001f ? move = 0 : move = move;
    Vector3 f;
    f = GetComponent<Transform3D_Component>()->Forward();
    f = { f.x, 0, f.z };
    f.Normalize();
    Vector3 output{};
    output.x = (f * move).x;
    output.z = (f * move).z;
    transform->SetVelocity(output);
}

/*--------------------------------------------------------PlayerController_Component RotationInput()-------------------------------------------------*/
/// <summary>
/// <para> WASD rotation input </para>
/// <para> WASD 回転入力 </para>
/// </summary>
void PlayerController_Component::RotationInput()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    Vector2 ax{ INPUTMANAGER::Instance()->Keyboard()->AxisX() };
    if (!ax.x && !ax.y)
    {
        //isRotating = false;
        return;
    }
    float y_ax{ Camera::Instance()->Rotation().y };



    Vector3 d_Front{ 0, 0, 1 }, d_Right{ 1, 0, 0 };
    Vector3 input{ ax.x, 0, ax.y };
    float t_Angle{ Vector3::GetAngle(d_Front, input) };
    if (d_Right.Dot(input) < 0)
        t_Angle *= -1;
    float new_Angle = y_ax + t_Angle;
    Vector3 rotation = transform->Rotation();
    rotation.y = new_Angle;
    transform->SlerpRotation(rotation);





}

/*--------------------------------------------------------PlayerController_Component JumpInput()-------------------------------------------------*/
/// <summary>
/// <para> SPACE jump input</para>
/// <para> SPACE ジャンプ入力</para>
/// </summary>    
void PlayerController_Component::JumpInput()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    if (INPUTMANAGER::Instance()->Keyboard()->Triggered(VK_SPACE))
    {
        Vector3 velocity{ transform->GetVelocity() };
        Vector3 position{ transform->GetTranslation() };
        velocity.y += 5.0f;
        position.y += 0.3f;
        transform->SetTranslation(position);
        transform->SetVelocity(velocity);

    }
}

/*--------------------------------------------------------PlayerController_Component SoundCollision()-------------------------------------------------*/
/// <summary>
/// <para> Perform collision check with EnvironmentalAudio_Component gameObjects </para>
/// <para> ENVIRONMENTAL_AUDIOゲームオブジェクトと当たり判定を計算 </para>
/// </summary>
void PlayerController_Component::SoundCollision()
{
    // Retrieve the components for use
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    for (auto& g : GameObjectManager::Instance()->GetGameObjects())
    {
        // Attempt to retrieve existing EnvironmentalAudio_Component
        EnvironmentalAudio_Component* audio = g.second->GetComponent<EnvironmentalAudio_Component>();
        if (!audio)
            continue;

        bool collided{};
        for (auto& c : audio->Colliders())
        {
            if (c->Collider()->Collide(transform->GetTranslation()))
            {
                
                if (!audio->Audio()->IsPlaying())
                    audio->Audio()->Play();

                // Calculate the distance between the player character and the sound source
                Vector3 closest_dist{ c->Collider()->DistanceToPlayer(this) };
                closest_dist -= transform->GetTranslation();
                float length{ closest_dist.Length() };
                length = max(c->Data()->minimum_distance, length);

                // Adjusts the volume according to the distance
                float volume = 1.0f - (length / c->Collider()->Collider()->Size());
                volume = min(volume, audio->GetData()->maximum_volume);

                // Audio ducking function
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

/*--------------------------------------------------------PlayerController_Component VelocityControl()-------------------------------------------------*/

void PlayerController_Component::VelocityControl()
{
}

/*--------------------------------------------------------PlayerController_Component GravityControl()----------------------------------------------------*/
/// <summary>
/// <para> Performs gravity control </para>
/// <para> 重力による落下処理の調整を行う </para>
/// </summary>
void PlayerController_Component::GravityControl()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    Vector3 velocity = transform->GetVelocity();
    velocity.y -= 0.1f;
    velocity.y = min(velocity.y, 15.0f);
    transform->SetVelocity(velocity);
}

/*--------------------------------------------------------PlayerController_Component GroundCollision()----------------------------------------------------*/
/// <summary>
/// <para> Calculates the velocity based on collision with anything below the player </para>
/// <para> 他のモデルが衝突によって加速度の調整を行う </para>
/// </summary>
void PlayerController_Component::GroundCollision()
{
    // This is only for mesh colliders
    MeshCollider_Component* collider{ GetComponent<MeshCollider_Component>() };
    if (!collider)
        return;

    // Prepare parameters for rayCasting
    Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
    Transform3D_Component* transform{ GetComponent<Transform3D_Component>() };
    Vector3 start, end;
    start = transform->GetTranslation();
    start.y += 1.001f;
    end = transform->GetTranslation();
    end.y += transform->GetVelocity().y ;

    COLLIDERS::RAYCASTDATA rcd{};
    bool collided{ COLLIDERS::RAYCAST_MANAGER::Instance()->Collide(start, end, mesh, rcd) };


    // Offsets position when collided
    if (collided)
    {
        Vector3 normal, position;
        normal = rcd.normal;
        transform->SetTranslation(rcd.position);
        Vector3 velocity;
        velocity = transform->GetVelocity();
        velocity.y = 0;
        transform->SetVelocity(velocity);


        //XMMATRIX test{ XMMatrixScaling(1, 1, 1) * XMMatrixTranslationFromVector(rcd.position.XMV()) };
        //sphere->UpdateVertices(0.1f,&test );

    } else {
        int i = 0;
    }

}

/*--------------------------------------------------------PlayerController_Component WallCollision()----------------------------------------------------*/
/// <summary>
/// <para> Calculates the velocity based on collision with anything in front the player </para>
/// <para> 他のモデルが衝突によって加速度の調整を行う </para>
/// </summary>
void PlayerController_Component::WallCollision()
{
    // This is only for mesh colliders
    MeshCollider_Component* collider{ GetComponent<MeshCollider_Component>() };
    if (!collider)
        return;

    // Prepare parameters for rayCasting
    Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
    Transform3D_Component* transform{ GetComponent<Transform3D_Component>() };

    Vector2 velocity{ transform->GetVelocity().x, transform->GetVelocity().z };

    if (velocity.Length() <  0.01f)
        return;

    Vector3 start, end;
    start = transform->GetTranslation();
    end = transform->GetTranslation();
    end.x += transform->GetVelocity().x;
    end.z += transform->GetVelocity().z;
    COLLIDERS::RAYCASTDATA rcd{};
    bool collided{ COLLIDERS::RAYCAST_MANAGER::Instance()->Collide(start, end, mesh, rcd) };

    // Update trigger state of mesh collider

    // Offsets position when collided
    if (collided)
    {
        Vector3 normal, position;
        normal = rcd.normal;
        //rcd.position ;
        transform->SetTranslation(rcd.position + normal * 0.2f);
        Vector3 velocity;
        velocity = transform->GetVelocity();
        velocity.y = 0;
        transform->SetVelocity(velocity);

    }

}

/*--------------------------------------------------------PlayerController_Component TerrainAudioCollision()-------------------------------------------------*/
/// <summary>
/// <para> Performs collision check for TerrainAudio_Component component </para>
/// <para> TERRAIN_AUDIOコンポネントの当たり判定を計算する </para>
/// </summary>
/// 
void PlayerController_Component::TerrainAudioCollision()
{
    // Check current gameObject for terrain audio component
    //TerrainAudio_Component* receiver{ GetComponent<TerrainAudio_Component>() };
    //if (!receiver || receiver->Property() != TerrainAudio_Property::RECEIVER)
    //    return;

    //// Check other gameObjects for terrain audio component
    //std::vector<GameObject*>emitters;
    //for (auto& g : GameObjectManager::Instance()->GetGameObjects())
    //{
    //    if (g.second.get() == parent)
    //        continue;
    //    if (g.second->GetComponent<TerrainAudio_Component>() != nullptr
    //        && g.second->GetComponent<TerrainAudio_Component>()->Data()->class_type == TerrainAudio_Property::EMITTER)
    //        emitters.push_back(g.second.get());

    //}

    //// Prepare coordinates of collision
    //TerrainAudio_Data_Receiver* rec{ (TerrainAudio_Data_Receiver*)receiver->Data()->property_data.get() };
    //receivers.resize(rec->receiver_bones.size());

    //Mesh_Component* mesh{ GetComponent<Mesh_Component>() };
    //int index{};
    //for (auto& v : receivers)
    //{
    //    XMMATRIX bone_transform = mesh->GetBoneTransform(rec->receiver_bones[index]);
    //    XMVECTOR s, r, t;
    //    XMMatrixDecompose(&s, &r, &t, bone_transform);
    //    v.Load(t);
    //    ++index;
    //}

    //// Prepare a buffer to check collider state


    //// Perform rayCast on receivers
    //for (auto& e : emitters)
    //{
    //    std::vector<TerrainAudio_Component*>emitters = e->GetListOfComponent<TerrainAudio_Component>();
    //    for (auto& entity : emitters)
    //    {
    //        //TerrainAudio_Component* emitter{ entity->GetComponent<TerrainAudio_Component>() };
    //        Mesh_Component* emitter_mesh{ entity->GetComponent<Mesh_Component>() };
    //        for (int ind = 0; ind < receivers.size(); ++ind)
    //        {
    //            // Ducking function
    //            for (auto& b : entity->Buffers())
    //            {
    //                if (b.buffer->IsDucking())
    //                    b.buffer->SetVolume(0.3f);
    //            }

    //            // Prepare parameters for rayCasting
    //            Vector3 start{ receivers[ind] };
    //            Vector3 end{ start };
    //            TerrainAudio_Data_Emitter* data{ (TerrainAudio_Data_Emitter*)(entity->Data()->property_data.get()) };

    //            // Offsets the start and end vectors for error compensation
    //            end.y -= 0.009f;
    //            start.y += 0.1f;
    //            COLLIDERS::RAYCASTDATA rcd{};
    //            bool collided{ RAYCAST(start, end, emitter_mesh, data->dataset[0]->mesh_index, rcd)};

    //            // Only triggers the sound effect upon triggering the collider
    //            bool triggered{};
    //            if (collided && statuses[ind] != collided)
    //                triggered = true;
    //            statuses[ind] = collided;
    //            if (triggered)
    //                entity->Play();
    //        }
    //    }
    //}
}

/*--------------------------------------------------------PlayerController_Component Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT PlayerController_Component::Initialize()
{

    //sphere = std::make_shared<DYNAMIC_SPHERE>();

    return S_OK;

}

/*--------------------------------------------------------PlayerController_Component Execute()-------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void PlayerController_Component::Execute()
{
    Vector3 position = GetComponent<Transform3D_Component>()->GetTranslation();
    position.y += 5.0f;
    Camera::Instance()->SetTarget(position);
    inDialogue = DialogueController::Instance()->GetStatus();

    if (inDialogue)
        return;

    MovementInput();
    RotationInput();
    JumpInput();
    SoundCollision();
    GravityControl();
    GroundCollision();
    WallCollision();
    VelocityControl();
    //TerrainAudioCollision();
    AnimationSettings();
    NPCDialogueTrigger();



}

/*--------------------------------------------------------PlayerController_Component Render()-------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void PlayerController_Component::Render()
{
}

/*--------------------------------------------------------PlayerController_Component UI()-------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void PlayerController_Component::UI()
{
    if (ImGui::TreeNode("Player Controller"))
    {
        ImGui::InputText("Name : ", data->name, 256);

        ImGui::TreePop();
    }
}

/*--------------------------------------------------------PlayerController_Component AnimationSettings()-------------------------------------------------*/
/// <summary>
/// <para> All input functions are called here </para>
/// <para> すべての入力関数を呼び出す</para>
/// </summary>
void PlayerController_Component::Input()
{

}

/*--------------------------------------------------------PlayerController_Component Input()-------------------------------------------------*/
/// <summary>
/// <para> Perform animation switching </para>
/// <para> アニメーション変更処理 </para>
/// </summary>
void PlayerController_Component::AnimationSettings()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    Mesh_Component* mesh = GetComponent<Mesh_Component>();
    Vector3 velocity = transform->GetVelocity();
    velocity.y = 0;
    if (velocity.Length() >= 0.05f)
        mesh->Model()->SetTake((int)PlayerAnim::Walking);
    else
        mesh->Model()->SetTake((int)PlayerAnim::T_Pose);

}

/*--------------------------------------------------------PlayerController_Component Hit()-------------------------------------------------*/
/// <summary>
/// <para> Called when hit </para>
/// <para> 攻撃されたら呼び出す </para>
/// </summary>
void PlayerController_Component::Hit()
{

}

/*--------------------------------------------------------PlayerController_Component Kill()-------------------------------------------------*/
/// <summary>
/// <para> Called when killed </para>
/// <para> 殺されたら呼び出す </para>
/// </summary>
void PlayerController_Component::Kill()
{

}

/*--------------------------------------------------------PlayerController_Component UpdateRotation()-------------------------------------------------*/
/// <summary>
/// <para> Updates rotation</para>
/// <para> 回転を更新</para>
/// </summary>
void PlayerController_Component::UpdateRotation()
{

}

/*--------------------------------------------------------PlayerController_Component NPCDialogueTrigger()-------------------------------------------------*/

void PlayerController_Component::NPCDialogueTrigger()
{
    if (inDialogue)
        return;
    std::vector<GameObject*>npcs;
    for (auto& g : GameObjectManager::Instance()->GetGameObjects())
    {
        if (g.second->GetComponent<Dialogue_Component>())
            npcs.push_back(g.second.get());
    }
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    Vector3 player_pos = transform->GetTranslation();
    for (auto npc : npcs)
    {
        Vector3 target_pos{ npc->GetComponent<Transform3D_Component>()->GetTranslation() };

        Vector3 distance = Vector3::Distance(player_pos, target_pos);
        Vector3 n_TargetPos{Vector3::Normalize(target_pos)}, n_PlayerPos{Vector3::Normalize(player_pos)};
        float angle_diff = Vector3::GetAngle(n_PlayerPos, n_TargetPos);
        float length{ Vector3::Length(distance) };
        if (angle_diff < ToRadians(90) && length < 10.0f)
        {
            if (INPUTMANAGER::Instance()->Keyboard()->Triggered('F'))
            {
                npc->GetComponent<Dialogue_Component>()->Trigger();
                inDialogue = true;
            }
        }
        
    }


}

/*--------------------------------------------------------PlayerController_Component GetComponentType()-------------------------------------------------*/

COMPONENT_TYPE PlayerController_Component::GetComponentType()
{
    return data->type;
}