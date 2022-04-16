#include "CapsuleCollider.h"
#include "MESH.h"
#include "TRANSFORM_3D.h"
#include "PLAYER_CONTROLLER.h"
int selected_mesh_c{};
int selected_bone_c{};

/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER DATA Class------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER DATA Constructor------------------------------------------*/

CapsuleCollider_Data::CapsuleCollider_Data()
{
    type = COMPONENT_TYPE::CAPSULE_COL;
}

/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER Class-----------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------CAPSULE COLLIDER Constructor-----------------------------------------------*/

CapsuleCollider::CapsuleCollider(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<CapsuleCollider_Data*>(data);
}

/*----------------------------------------------CAPSULE COLLIDER Initialize()------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (CapsuleCollider_Data)</para>
/// <para> �������ɌĂяo���B�Ώۂ̃f�[�^���g���ď��������� (CAPSULE COLLIDER DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT CapsuleCollider::Initialize()
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
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void CapsuleCollider::Execute()
{
    // Updates the properties of the collider
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        // If not attached to bone
        collider->Execute(transform->TransformMatrix());
        world = collider->MatrixOffset() * transform->TransformMatrix();
        Vector3 t = ((COLLIDERS::CAPSULE*)collider.get())->Top();
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
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void CapsuleCollider::Execute(XMMATRIX transform)
{
    // Updates the properties of the collider
    XMMATRIX world{ XMMatrixIdentity() };
    if (data->bone_name == "")
    {
        // If not attached to bone
        collider->Execute(transform);
        world = collider->MatrixOffset() * transform;
        Vector3 t = ((COLLIDERS::CAPSULE*)collider.get())->Top();
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
/// <para> Execute()��Ƀ����_�[�֐������s����悤�ɖ��t���[���ɌĂяo�� </para>
/// </summary>
void CapsuleCollider::Render()
{
    if (collider) {
        collider->Render();
        capsule->Render();
    }

}

/*----------------------------------------------CAPSULE COLLIDER UI()------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UI��`�� </para>
/// </summary>
void CapsuleCollider::UI()
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
/// <para> �v���C���[�Ƃ̋������v�Z���� </para>    
/// </summary>
/// <returns></returns>
Vector3 CapsuleCollider::DistanceToPlayer(PLAYER_CONTROLLER* target)
{
    Vector3 player_position = target->GetComponent<TRANSFORM_3D>()->Translation();
    return COLLIDERS::PointLineClosest(player_position, collider.get());
}

/*----------------------------------------------CAPSULE COLLIDER Collide()------------------------------------------*/
/// <summary>
/// <para> Perform collision check to the target gameObject </para>
/// <para> �Q�[���I�u�W�F�N�g�ɓ����蔻����v�Z </para>
/// </summary
bool CapsuleCollider::Collide(GameObject* target)
{
    CapsuleCollider* other = target->GetComponent<CapsuleCollider>();
    if (other)
    {
        return collider->Collide(other->Collider().get());
    }
    return false;
}
/// <summary>
/// <para> Perform collision check based on coordinates </para>
/// <para> ���W�_����Â��ē����蔻����v�Z </para>
/// </summary>
/// <returns></returns>
bool CapsuleCollider::Collide(Vector3 p)
{
    return collider->Collide(p);
}

/*----------------------------------------------CAPSULE COLLIDER BoneName()------------------------------------------*/
/// <summary>
/// Returns the name of the bone the collider is attached to
/// </summary>
/// <returns></returns>
const std::string& CapsuleCollider::BoneName()
{
    return data->bone_name;
}

/*----------------------------------------------CAPSULE COLLIDER ColliderName()------------------------------------------*/
/// <summary>
/// Returns the name of the collider
/// </summary>
/// <returns></returns>
const std::string& CapsuleCollider::ColliderName()
{
    return std::string(data->name);
}

/*----------------------------------------------CAPSULE COLLIDER Center()------------------------------------------*/
/// <summary>
/// Returns the center point of the collider
/// </summary>
/// <returns></returns>
const Vector3 CapsuleCollider::Center()
{
    return data->center;
}

/*----------------------------------------------CAPSULE COLLIDER Radius()------------------------------------------*/
/// <summary>
/// Returns the radius of the collider
/// </summary>
/// <returns></returns>
const float CapsuleCollider::Radius()
{
    return data->radius;
}

/*----------------------------------------------CAPSULE COLLIDER Height()------------------------------------------*/
/// <summary>
/// Returns the height of the collider
/// </summary>
/// <returns></returns>
const float CapsuleCollider::Height()
{
    return data->height;
}

/*----------------------------------------------CAPSULE COLLIDER Data()------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
CapsuleCollider_Data* CapsuleCollider::Data()
{
    return data;
}

/*----------------------------------------------CAPSULE COLLIDER Collider()------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>CapsuleCollider::Collider()
{
    return collider;
}

/*----------------------------------------------CAPSULE COLLIDER GetComponentType()------------------------------------------*/

COMPONENT_TYPE CapsuleCollider::GetComponentType()
{
    return data->type;
}