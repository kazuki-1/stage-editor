#include "Base Classes/Component.h"
#include "SphereCollider.h"
#include "Transform3D.h"
#include "Mesh.h"
#include "PlayerController.h"
int selected_mesh_s{};
int selected_bone_s{};

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SphereCollider_Data Class-----------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SphereCollider_Data Constructor-----------------------------------------*/

SphereCollider_Data::SphereCollider_Data()
{
    type = COMPONENT_TYPE::SPHERE_COL;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SphereCollider_Component Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SphereCollider_Component Constructor----------------------------------------------*/

SphereCollider_Component::SphereCollider_Component(GameObject* t, ComponentData* d)
{
    parent = t;
    this->data = static_cast<SphereCollider_Data*>(d);
}

/*---------------------------------------SphereCollider_Component Initialize()----------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SphereCollider_Data) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT SphereCollider_Component::Initialize()
{
    sphere = std::make_shared<DYNAMIC_SPHERE>(32, data->radius);
    collider = std::make_shared<COLLIDERS::SPHERE>(data->center, data->radius);
    //sphere->Initialize();
    collider->Initialize();
    return S_OK;
}

/*---------------------------------------SphereCollider_Component Execute()----------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SphereCollider_Component::Execute()
{
    Transform3D_Component* transform{ GetComponent<Transform3D_Component>() };
    XMMATRIX world{ XMMatrixIdentity() };
    // If not attached to bone
    if (data->bone_name == "")
    {
        collider->Execute(transform->TransformMatrix());
        world = { collider->MatrixOffset() * transform->TransformMatrix() };
    }
    else
    {
        collider->FitToBone(data->bone_name, data->mesh_index, GetComponent<Mesh_Component>()->Model().get());
        world = { collider->MatrixOffset() * collider->WorldMatrix() };
    }
    sphere->Execute(world);
}
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SphereCollider_Component::Execute(XMMATRIX transform)
{
    XMMATRIX world{ XMMatrixIdentity() };
    // If not attached to bone
    if (data->bone_name == "")
    {
        collider->Execute(transform);
        sphere->Execute(collider->MatrixOffset() * transform);
        world = { collider->MatrixOffset() * transform };
    }
    else
    {
        collider->FitToBone(data->bone_name, data->mesh_index, GetComponent<Mesh_Component>()->Model().get());
        world = { collider->MatrixOffset() * collider->WorldMatrix() };
    }
    //sphere->UpdateVertices(data->radius, &world);

}

/*---------------------------------------SphereCollider_Component Render()----------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void SphereCollider_Component::Render()
{
    sphere->Render();
}

/*---------------------------------------SphereCollider_Component UI()----------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void SphereCollider_Component::UI()
{
    if (ImGui::TreeNode("Sphere Collider"))
    {
        // Parameters
        ImGui::InputText("Collider Name", data->name, 256);
        ImGui::DragFloat3("Center : ", &data->center.x, 0.05f);
        ImGui::DragFloat("Radius", &data->radius, 0.05f);
            collider->SetCenter(data->center);
            collider->SetRadius(data->radius);
            collider->OffsetCollider(data->center);
            sphere->UpdateVertices(data->radius);
        // Bind to bone
        if (GetComponent<Mesh_Component>() != nullptr && GetComponent<Mesh_Component>()->Model() != nullptr)
        {
            MODEL_RESOURCES& mr{ *GetComponent<Mesh_Component>()->Model().get()->Resource().get() };
            if (ImGui::BeginCombo("Meshes", mr.Meshes[selected_mesh_s].Name.c_str()))
            {
                int ind{};
                for (auto& m : mr.Meshes)
                {
                    bool s{};
                    if (ImGui::Selectable(m.Name.c_str(), &s))
                        selected_mesh_s = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }
            MODEL_RESOURCES::SKELETON& bs{ mr.Meshes[selected_mesh_s].Bind_Pose };
            if (ImGui::BeginCombo("Bones", bs.Bones[selected_bone_s].Name.c_str()))
            {
                int ind{};
                for (auto& b : bs.Bones)
                {
                    bool s{};
                    if (ImGui::Selectable(b.Name.c_str(), &s))
                    {
                        selected_bone_s = ind;
                        data->mesh_index = selected_mesh_s;
                        data->bone_name = bs.Bones[selected_bone_s].Name;
                    }
                    ++ind;
                }
                ImGui::EndCombo();
            }
            //if (ImGui::Button("Set To Bone"))
            //{
            //    data->mesh_index = selected_mesh_s;
            //    data->bone_name = bs.Bones[selected_bone_s].Name;
            //}

        }
    ImGui::TreePop();
    }
}
/*---------------------------------------SphereCollider_Component Finalize()----------------------------------------------*/
/// <summary>
/// Called when component is destroyed
/// </summary>
void SphereCollider_Component::Finalize()
{
    sphere->Finalize();
}

/*---------------------------------------SphereCollider_Component DistanceToPlayer()----------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> プレイヤーとの距離を計算する </para>
/// </summary>
Vector3 SphereCollider_Component::DistanceToPlayer(PlayerController_Component* target)
{
    Vector3 player_position = target->GetComponent<Transform3D_Component>()->GetTranslation();
    return collider->Center() - player_position;
}

/*---------------------------------------SphereCollider_Component Collide()----------------------------------------------*/
/// <summary>
/// <para> Perform collision check with another gameObject </para>
/// <para> 引数であるゲームオブジェクトを当たり判定を計算</para>
/// </summary>
/// <returns></returns>
bool SphereCollider_Component::Collide(GameObject* target)
{
    SphereCollider_Component* other = target->GetComponent<SphereCollider_Component>();
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
bool SphereCollider_Component::Collide(Vector3 p)
{
    return collider->Collide(p);
}

/*---------------------------------------SphereCollider_Component ClosestPoint()----------------------------------------------*/

Vector3 SphereCollider_Component::GetClosestPoint(Vector3 target)
{
    return collider->Center();
}
/*---------------------------------------SphereCollider_Component Radius()----------------------------------------------*/

float SphereCollider_Component::Radius()
{
    return data->radius;
}

/*---------------------------------------SphereCollider_Component Center()----------------------------------------------*/

Vector3 SphereCollider_Component::Center()
{
    return data->center;
}

/*---------------------------------------SphereCollider_Component Collider()----------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>SphereCollider_Component::Collider()
{
    return collider;
}

/*---------------------------------------SphereCollider_Component Data()----------------------------------------------*/

SphereCollider_Data* SphereCollider_Component::Data()
{
    return data;
}

/*----------------------------------------------------------SphereCollider_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE SphereCollider_Component::GetComponentType()
{
    return data->type;
}