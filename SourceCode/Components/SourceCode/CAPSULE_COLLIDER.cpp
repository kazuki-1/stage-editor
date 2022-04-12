#include "../Headers/CAPSULE_COLLIDER.h"
#include "../Headers/MESH.h"
#include "../Headers/TRANSFORM_3D.h"
#include "../Headers/PLAYER_CONTROLLER.h"
int selected_mesh_c{};
int selected_bone_c{};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER DATA Class------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER DATA Constructor------------------------------------------*/

CAPSULE_COLLIDER_DATA::CAPSULE_COLLIDER_DATA()
{
    type = COMPONENT_TYPE::CAPSULE_COL;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER Class-----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER Constructor-----------------------------------------------*/

CAPSULE_COLLIDER::CAPSULE_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<CAPSULE_COLLIDER_DATA*>(data);
}

/*----------------------------------------------CAPSULE COLLIDER Initialize()------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (CAPSULE_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (CAPSULE COLLIDER DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT CAPSULE_COLLIDER::Initialize()
{
    // Creates a collider and its debug primitive using the data
    collider = std::make_shared<COLLIDERS::CAPSULE>(data->center, data->radius, data->height);
    collider->Initialize();
    capsule = std::make_shared<DYNAMIC_CAPSULE>();
    collider->SetRadius(data->radius);
    collider->SetHeight(data->height);
    collider->OffsetCollider(data->center);
    collider->RotateCollider(ToRadians(data->rotation));
    return collider ? S_OK : E_FAIL;
}

/*----------------------------------------------CAPSULE COLLIDER Execute()------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void CAPSULE_COLLIDER::Execute()
{
    // Updates the properties of the collider
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        // If not attached to bone
        collider->Execute(transform->TransformMatrix());
        world = collider->MatrixOffset() * transform->TransformMatrix();
        VECTOR3 t = ((COLLIDERS::CAPSULE*)collider.get())->Top();
    }
    else
    {
        // If attached to bone
        collider->FitToBone(data->bone_name, GetComponent<MESH>()->Model().get());
        //capsule->Execute(collider->WorldMatrix());
        world = collider->MatrixOffset() * collider->WorldMatrix();
    }
    capsule->UpdateVertices(data->radius, data->height, &world);
}
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void CAPSULE_COLLIDER::Execute(XMMATRIX transform)
{
    // Updates the properties of the collider
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        // If not attached to bone
        collider->Execute(transform);
        world = collider->MatrixOffset() * transform;
        VECTOR3 t = ((COLLIDERS::CAPSULE*)collider.get())->Top();
    }
    else
    {
        // If attached to bone
        collider->FitToBone(data->bone_name, GetComponent<MESH>()->Model().get());
        world = collider->MatrixOffset() * collider->WorldMatrix();
    }
    capsule->UpdateVertices(data->radius, data->height, &world);

}

/*----------------------------------------------CAPSULE COLLIDER Render()------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void CAPSULE_COLLIDER::Render()
{
    if (collider) {
        collider->Render();
        capsule->Render();
    }

}

/*----------------------------------------------CAPSULE COLLIDER UI()------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void CAPSULE_COLLIDER::UI()
{
    if (!collider)
        collider = std::make_shared<COLLIDERS::CAPSULE>(data->center, data->height, data->radius);
    if (ImGui::TreeNode("Capsule Collider"))
    {
        ImGui::InputText("Collider Name", data->name, 256);
        ImGui::DragFloat3("Center : ", &data->center.x, 0.05f);
        ImGui::DragFloat3("Rotation : ", &data->rotation.x, 0.05f);
        ImGui::DragFloat("Height : ", &data->height, 0.05f, 0.0f);
        ImGui::DragFloat("Radius", &data->radius, 0.05f, 0.0f);
        if (ImGui::Button("Set Data to collider"))
        {

            collider->SetRadius(data->radius);
            collider->SetHeight(data->height);
            collider->OffsetCollider(data->center);
            collider->RotateCollider(ToRadians(data->rotation));
        }

        if (GetComponent<MESH>() != nullptr && GetComponent<MESH>()->Model() != nullptr)
        {
            MODEL_RESOURCES& mr{ *GetComponent<MESH>()->Model().get()->Resource().get() };
            if (ImGui::BeginCombo("Meshes", mr.Meshes[selected_mesh_c].Name.c_str()))
            {
                int ind{};
                for (auto& m : mr.Meshes)
                {
                    bool s{};
                    if (ImGui::Selectable(m.Name.c_str(), &s))
                        selected_mesh_c = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }
            MODEL_RESOURCES::SKELETON& bs{ mr.Meshes[selected_mesh_c].Bind_Pose };
            if (ImGui::BeginCombo("Bones", bs.Bones[selected_bone_c].Name.c_str()))
            {
                int ind{};
                for (auto& b : bs.Bones)
                {
                    bool s{};
                    if (ImGui::Selectable(b.Name.c_str(), &s))
                        selected_bone_c = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Set To Bone"))
                data->bone_name = bs.Bones[selected_bone_c].Name;


        }
        ImGui::TreePop();
    }
}

/*----------------------------------------------CAPSULE COLLIDER DistanceToPlayer()------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> プレイヤーとの距離を計算する </para>    
/// </summary>
/// <returns></returns>
VECTOR3 CAPSULE_COLLIDER::DistanceToPlayer(PLAYER_CONTROLLER* target)
{
    VECTOR3 player_position = target->GetComponent<TRANSFORM_3D>()->Translation();
    return COLLIDERS::PointLineClosest(player_position, collider.get());
}

/*----------------------------------------------CAPSULE COLLIDER Collide()------------------------------------------*/
/// <summary>
/// <para> Perform collision check to the target gameObject </para>
/// <para> ゲームオブジェクトに当たり判定を計算 </para>
/// </summary
bool CAPSULE_COLLIDER::Collide(GAMEOBJECT* target)
{
    CAPSULE_COLLIDER* other = target->GetComponent<CAPSULE_COLLIDER>();
    if (other)
    {
        return collider->Collide(other->Collider().get());
    }
    return false;
}
/// <summary>
/// <para> Perform collision check based on coordinates </para>
/// <para> 座標点を基づいて当たり判定を計算 </para>
/// </summary>
/// <returns></returns>
bool CAPSULE_COLLIDER::Collide(VECTOR3 p)
{
    return collider->Collide(p);
}

/*----------------------------------------------CAPSULE COLLIDER BoneName()------------------------------------------*/
/// <summary>
/// Returns the name of the bone the collider is attached to
/// </summary>
/// <returns></returns>
const std::string& CAPSULE_COLLIDER::BoneName()
{
    return data->bone_name;
}

/*----------------------------------------------CAPSULE COLLIDER ColliderName()------------------------------------------*/
/// <summary>
/// Returns the name of the collider
/// </summary>
/// <returns></returns>
const std::string& CAPSULE_COLLIDER::ColliderName()
{
    return std::string(data->name);
}

/*----------------------------------------------CAPSULE COLLIDER Center()------------------------------------------*/
/// <summary>
/// Returns the center point of the collider
/// </summary>
/// <returns></returns>
const VECTOR3 CAPSULE_COLLIDER::Center()
{
    return data->center;
}

/*----------------------------------------------CAPSULE COLLIDER Radius()------------------------------------------*/
/// <summary>
/// Returns the radius of the collider
/// </summary>
/// <returns></returns>
const float CAPSULE_COLLIDER::Radius()
{
    return data->radius;
}

/*----------------------------------------------CAPSULE COLLIDER Height()------------------------------------------*/
/// <summary>
/// Returns the height of the collider
/// </summary>
/// <returns></returns>
const float CAPSULE_COLLIDER::Height()
{
    return data->height;
}

/*----------------------------------------------CAPSULE COLLIDER Data()------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
CAPSULE_COLLIDER_DATA* CAPSULE_COLLIDER::Data()
{
    return data;
}

/*----------------------------------------------CAPSULE COLLIDER Collider()------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>CAPSULE_COLLIDER::Collider()
{
    return collider;
}

/*----------------------------------------------CAPSULE COLLIDER GetComponentType()------------------------------------------*/

COMPONENT_TYPE CAPSULE_COLLIDER::GetComponentType()
{
    return data->type;
}
