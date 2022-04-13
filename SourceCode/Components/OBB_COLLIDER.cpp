#include "Base Classes/COMPONENT.h"
#include "OBB_COLLIDER.h"
#include "TRANSFORM_3D.h"
#include "MESH.h"
#include "PLAYER_CONTROLLER.h"
int selected_mesh_o{};
int selected_bone_o{};
bool update_o{};

/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBB_COLLIDER_DATA Class--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBB_COLLIDER_DATA Constructor--------------------------------------------*/

OBB_COLLIDER_DATA::OBB_COLLIDER_DATA()
{
    type = COMPONENT_TYPE::OBB_COL;
}


/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBB_COLLIDER Class--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBB_COLLIDER Constructor--------------------------------------------*/

OBB_COLLIDER::OBB_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<OBB_COLLIDER_DATA*>(data);
}

/*-------------------------------------------------OBB_COLLIDER Initialize()--------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT OBB_COLLIDER::Initialize()
{
    cube = std::make_shared<DYNAMIC_CUBE>();
    collider = std::make_shared<COLLIDERS::OBB>(data->min, data->max);
    return collider ? S_OK : E_FAIL;
}

/*-------------------------------------------------OBB_COLLIDER Execute()--------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void OBB_COLLIDER::Execute()
{
    TRANSFORM_3D* m{ GetComponent<TRANSFORM_3D>() };
    cube->SetTarget(m->Translation());
    if (data->bone_name == "") {
        collider->Execute(m->TransformMatrix());
        cube->Execute(m->TransformMatrix());
    }
    else {
        MODEL* m = GetComponent<MESH>()->Model().get();
        collider->FitToBone(data->bone_name, m);
        cube->Execute(collider->WorldMatrix());
    }
    cube->UpdateVertices(collider->Points());
}
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void OBB_COLLIDER::Execute(XMMATRIX transform)
{
    XMVECTOR s, r, t;
    XMMatrixDecompose(&s, &r, &t, transform);
    Vector3 target{};
    target.Load(t);
    if (data->bone_name == "") {
        collider->Execute(transform);
        cube->Execute(transform);
    }
    else {
        MODEL* m = GetComponent<MESH>()->Model().get();
        collider->FitToBone(data->bone_name, m);
        cube->Execute(collider->WorldMatrix());
    }
    cube->UpdateVertices(collider->Points());

}

/*-------------------------------------------------OBB_COLLIDER Render()--------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void OBB_COLLIDER::Render()
{
    if (collider) {
        collider->Render();
        cube->Render();
    }
}

/*-------------------------------------------------OBB_COLLIDER UI()--------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void OBB_COLLIDER::UI()
{
    if (!collider)
        collider = std::make_shared<COLLIDERS::OBB>(data->min, data->max);
    if (ImGui::TreeNode("OBB Collider"))
    {
        ImGui::InputText("Collider Name", data->name, 256);
        ImGui::DragFloat3("Min", &data->min.x, 0.05f);
        ImGui::DragFloat3("Max", &data->max.x, 0.05f);
        ImGui::DragFloat3("Offset : ", &data->offset.x, 0.05f);
        if (ImGui::Button("Set Data to collider"))
        {
            collider->SetMin(data->min);
            collider->SetMax(data->max);
            collider->OffsetCollider(data->offset);

            update_o = true;
        }
        MESH* m{ GetComponent<MESH>() };



        if (m && m->Model())
        {
            MODEL_RESOURCES& mr{ *GetComponent<MESH>()->Model().get()->Resource().get() };
            if (ImGui::BeginCombo("Meshes", mr.Meshes[selected_mesh_o].Name.c_str()))
            {
                int ind{};
                for (auto& m : mr.Meshes)
                {
                    bool s{};
                    if (ImGui::Selectable(m.Name.c_str(), &s))
                        selected_mesh_o = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }
            MODEL_RESOURCES::SKELETON& bs{ mr.Meshes[selected_mesh_o].Bind_Pose };
            if (ImGui::BeginCombo("Bones", bs.Bones[selected_bone_o].Name.c_str()))
            {
                int ind{};
                for (auto& b : bs.Bones)
                {
                    bool s{};
                    if (ImGui::Selectable(b.Name.c_str(), &s))
                        selected_bone_o = ind;
                    ++ind;
                }
                ImGui::EndCombo();
            }

            if (ImGui::Button("Set To Bone"))
                data->bone_name = bs.Bones[selected_bone_o].Name;
        }
        ImGui::TreePop();
    }

}

/*-------------------------------------------------OBB_COLLIDER DistanceToPlayer()--------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> プレイヤーとの距離を計算する </para>
/// </summary>
Vector3 OBB_COLLIDER::DistanceToPlayer(PLAYER_CONTROLLER* target)
{
    Vector3 player_position = target->GetComponent<TRANSFORM_3D>()->Translation();
    return collider->Center() - player_position;

}

/*-------------------------------------------------OBB_COLLIDER Collide()--------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> プレイヤーとの距離を計算する </para>
/// </summary>
bool OBB_COLLIDER::Collide(GAMEOBJECT* target)
{
    OBB_COLLIDER* other = target->GetComponent<OBB_COLLIDER>();
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
bool OBB_COLLIDER::Collide(Vector3 p)
{
    return collider->Collide(p);
}

/*-------------------------------------------------OBB_COLLIDER ColliderName()--------------------------------------------*/
/// <summary>
/// Returns the name of the collider
/// </summary>
/// <returns></returns>
const std::string& OBB_COLLIDER::ColliderName()
{
    return std::string(data->name);
}

/*-------------------------------------------------OBB_COLLIDER BoneName()--------------------------------------------*/
/// <summary>
/// Returns the name of the bone that the collider is attached to
/// </summary>
/// <returns></returns>
const std::string& OBB_COLLIDER::BoneName()
{
    return data->bone_name;
}

/*-------------------------------------------------OBB_COLLIDER Collider()--------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>OBB_COLLIDER::Collider()
{
    return collider;
}

/*-------------------------------------------------OBB_COLLIDER Min()--------------------------------------------*/
/// <summary>
/// Returns the minimum point of the collider
/// </summary>
/// <returns></returns>
Vector3 OBB_COLLIDER::Min()
{
    return data->min;
}

/*-------------------------------------------------OBB_COLLIDER Max()--------------------------------------------*/
/// <summary>
/// Returns the maximum point of the collider
/// </summary>
/// <returns></returns>
Vector3 OBB_COLLIDER::Max()
{
    return data->max;
}

/*-------------------------------------------------OBB_COLLIDER Data()--------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
OBB_COLLIDER_DATA* OBB_COLLIDER::Data()
{
    return data;
}

/*-------------------------------------------------OBB_COLLIDER GetComponentType()--------------------------------------------*/
COMPONENT_TYPE OBB_COLLIDER::GetComponentType()
{
    return data->type;
}