#include "Base Classes/Component.h"
#include "SPHERE_COLLIDER.h"
#include "TRANSFORM_3D.h"
#include "MESH.h"
#include "PLAYER_CONTROLLER.h"
int selected_mesh_s{};
int selected_bone_s{};

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPHERE_COLLIDER_DATA Class-----------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPHERE_COLLIDER_DATA Constructor-----------------------------------------*/

SPHERE_COLLIDER_DATA::SPHERE_COLLIDER_DATA()
{
    type = COMPONENT_TYPE::SPHERE_COL;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPHERE_COLLIDER Class----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPHERE_COLLIDER Constructor----------------------------------------------*/

SPHERE_COLLIDER::SPHERE_COLLIDER(GameObject* t, ComponentData* d)
{
    parent = t;
    this->data = static_cast<SPHERE_COLLIDER_DATA*>(d);
}

/*---------------------------------------SPHERE_COLLIDER Initialize()----------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT SPHERE_COLLIDER::Initialize()
{
    sphere = std::make_shared<DYNAMIC_SPHERE>(32, data->radius);
    collider = std::make_shared<COLLIDERS::SPHERE>(data->center, data->radius);
    //sphere->Initialize();
    collider->Initialize();
    return S_OK;
}

/*---------------------------------------SPHERE_COLLIDER Execute()----------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SPHERE_COLLIDER::Execute()
{
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        collider->Execute(transform->TransformMatrix());
        sphere->Execute(collider->MatrixOffset() * transform->TransformMatrix());
        world = { collider->MatrixOffset() * transform->TransformMatrix() };
        //sphere->UpdateVertices(data->radius, &world);
    }
    else
    {
        collider->FitToBone(data->bone_name, GetComponent<MESH>()->Model().get());
        world = { collider->MatrixOffset() * collider->WorldMatrix() };
    }
    sphere->UpdateVertices(data->radius, &world);
}
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void SPHERE_COLLIDER::Execute(XMMATRIX transform)
{
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        collider->Execute(transform);
        sphere->Execute(collider->MatrixOffset() * transform);
        world = { collider->MatrixOffset() * transform };
        //sphere->UpdateVertices(data->radius, &world);
    }
    else
    {
        collider->FitToBone(data->bone_name, GetComponent<MESH>()->Model().get());
        world = { collider->MatrixOffset() * collider->WorldMatrix() };
    }
    sphere->UpdateVertices(data->radius, &world);

}

/*---------------------------------------SPHERE_COLLIDER Render()----------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void SPHERE_COLLIDER::Render()
{
    sphere->Render();
}

/*---------------------------------------SPHERE_COLLIDER UI()----------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void SPHERE_COLLIDER::UI()
{
    if (ImGui::TreeNode("Sphere Collider"))
    {
        ImGui::InputText("Collider Name", data->name, 256);
        ImGui::DragFloat3("Center : ", &data->center.x, 0.05f);
        ImGui::DragFloat("Radius", &data->radius, 0.05f);
        if (ImGui::Button("Set Data to collider"))
        {
            collider->SetCenter(data->center);
            collider->SetRadius(data->radius);
            collider->OffsetCollider(data->center);
        }
        if (GetComponent<MESH>() != nullptr && GetComponent<MESH>()->Model() != nullptr)
        {
            MODEL_RESOURCES& mr{ *GetComponent<MESH>()->Model().get()->Resource().get() };
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
                        selected_bone_s = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Set To Bone"))
                data->bone_name = bs.Bones[selected_bone_s].Name;


        }
        ImGui::TreePop();

    }
}

/*---------------------------------------SPHERE_COLLIDER DistanceToPlayer()----------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> プレイヤーとの距離を計算する </para>
/// </summary>
Vector3 SPHERE_COLLIDER::DistanceToPlayer(PLAYER_CONTROLLER* target)
{
    Vector3 player_position = target->GetComponent<TRANSFORM_3D>()->Translation();
    return collider->Center() - player_position;
}

/*---------------------------------------SPHERE_COLLIDER Collide()----------------------------------------------*/
/// <summary>
/// <para> Perform collision check with another gameObject </para>
/// <para> 引数であるゲームオブジェクトを当たり判定を計算</para>
/// </summary>
/// <returns></returns>
bool SPHERE_COLLIDER::Collide(GameObject* target)
{
    SPHERE_COLLIDER* other = target->GetComponent<SPHERE_COLLIDER>();
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
bool SPHERE_COLLIDER::Collide(Vector3 p)
{
    return collider->Collide(p);
}

/*---------------------------------------SPHERE_COLLIDER Radius()----------------------------------------------*/

float SPHERE_COLLIDER::Radius()
{
    return data->radius;
}

/*---------------------------------------SPHERE_COLLIDER Center()----------------------------------------------*/

Vector3 SPHERE_COLLIDER::Center()
{
    return data->center;
}

/*---------------------------------------SPHERE_COLLIDER Collider()----------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>SPHERE_COLLIDER::Collider()
{
    return collider;
}

/*---------------------------------------SPHERE_COLLIDER Data()----------------------------------------------*/

SPHERE_COLLIDER_DATA* SPHERE_COLLIDER::Data()
{
    return data;
}

/*----------------------------------------------------------SPHERE_COLLIDER GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE SPHERE_COLLIDER::GetComponentType()
{
    return data->type;
}