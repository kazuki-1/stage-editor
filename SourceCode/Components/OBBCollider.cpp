#include "OBBCollider.h"
#include "Transform3D.h"
#include "Mesh.h"
#include "PlayerController.h"
int selected_mesh_o{};
int selected_bone_o{};

/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBBCollider_Data Class--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBBCollider_Data Constructor--------------------------------------------*/

OBBCollider_Data::OBBCollider_Data()
{
    type = COMPONENT_TYPE::OBB_COL;
}


/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBBCollider_Component Class--------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------OBBCollider_Component Constructor--------------------------------------------*/

OBBCollider_Component::OBBCollider_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<OBBCollider_Data*>(data);
}

/*-------------------------------------------------OBBCollider_Component Initialize()--------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBBCollider_Data)</para>
/// <para> �������ɌĂяo���B�Ώۂ̃f�[�^���g���ď��������� (OBBCollider_Data) </para>
/// </summary>
/// <returns></returns>
HRESULT OBBCollider_Component::Initialize()
{
    cube = std::make_shared<DYNAMIC_CUBE>();
    collider = std::make_shared<COLLIDERS::OBB>(Vector3{}, Vector3{});
    return collider ? S_OK : E_FAIL;
}

/*-------------------------------------------------OBBCollider_Component Execute()--------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void OBBCollider_Component::Execute()
{
    Transform3D_Component* m{ GetComponent<Transform3D_Component>() };
    cube->SetTarget(m->GetTranslation());
    cube->SetWorldTransform(m->TransformMatrix());
    // Attached to bone
    if (data->bone_name == "") 
    {
        collider->Execute(m->TransformMatrix());
        cube->Execute(m->TransformMatrix());
    }
    else 
    {
        std::shared_ptr<MODEL> m = GetComponent<Mesh_Component>()->Model();
        collider->FitToBone(data->bone_name, data->mesh_index, m.get());
        cube->Execute(collider->WorldMatrix());
    }
    // Updates the vertices of the debug cube
    cube->UpdateVertices(COLLIDERS::OBB::GeneratePoints(m->TransformMatrix(), m->GetTranslation(), data->size));
    
}

/// <summary>
/// <para> Called each frame </para>
/// <para> ���t���[���ɌĂяo�� </para>
/// </summary>
void OBBCollider_Component::Execute(XMMATRIX transform)
{

    


    XMVECTOR s, r, t;
    XMMatrixDecompose(&s, &r, &t, transform);
    Vector3 target{};
    target.Load(t);
    cube->SetWorldTransform(transform);
    if (data->bone_name == "")
    {
        collider->Execute(transform);
        cube->Execute(transform);
    }
    else 
    {
        MODEL* m = GetComponent<Mesh_Component>()->Model().get();
        collider->FitToBone(data->bone_name, data->mesh_index, m);
        cube->Execute(collider->WorldMatrix());
    }
    cube->UpdateVertices(COLLIDERS::OBB::GeneratePoints(transform, data->offset, data->size));

}

/*-------------------------------------------------OBBCollider_Component Render()--------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()��Ƀ����_�[�֐������s����悤�ɖ��t���[���ɌĂяo�� </para>
/// </summary>
void OBBCollider_Component::Render()
{
    if (collider)
    {
        collider->Render();
        cube->Render();
    }
}

/*-------------------------------------------------OBBCollider_Component UI()--------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UI��`�� </para>
/// </summary>
void OBBCollider_Component::UI()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();


    if (!collider)
        collider = std::make_shared<COLLIDERS::OBB>(transform->GetTranslation(), Vector3());
    if (ImGui::TreeNode("OBB Collider"))
    {
        // Parameters
        ImGui::InputText("Collider Name", data->name, 256);
        ImGui::DragFloat3("Size", &data->size.x, 0.05f, 0.0f);
        ImGui::DragFloat3("Offset : ", &data->offset.x, 0.05f);
        if (ImGui::Button("Set Data to collider"))
        {
            collider->SetSize(data->size);
            collider->OffsetCollider(data->offset);

        }
        Mesh_Component* m{ GetComponent<Mesh_Component>() };


        // Bind to bone
        if (m && m->Model())
        {
            MODEL_RESOURCES& mr{ *GetComponent<Mesh_Component>()->Model().get()->Resource() };
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
            {
                data->bone_name = bs.Bones[selected_bone_o].Name;
                data->mesh_index = selected_mesh_o;
            }
        }
        ImGui::TreePop();
    }

}

/*-------------------------------------------------OBBCollider_Component Finalize()--------------------------------------------*/
/// <summary>
/// Called when component is destroyed
/// </summary>
void OBBCollider_Component::Finalize()
{
    cube->Finalize();
}

/*-------------------------------------------------OBBCollider_Component DistanceToPlayer()--------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> �v���C���[�Ƃ̋������v�Z���� </para>
/// </summary>
Vector3 OBBCollider_Component::DistanceToPlayer(PlayerController_Component* target)
{
    Vector3 player_position = target->GetComponent<Transform3D_Component>()->GetTranslation();
    return collider->Center() - player_position;

}

/*-------------------------------------------------OBBCollider_Component Collide()--------------------------------------------*/
/// <summary>
/// <para> Calculates the distance to the player </para>
/// <para> �v���C���[�Ƃ̋������v�Z���� </para>
/// </summary>
bool OBBCollider_Component::Collide(GameObject* target)
{
    OBBCollider_Component* other = target->GetComponent<OBBCollider_Component>();
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
bool OBBCollider_Component::Collide(Vector3 p)
{
    return collider->Collide(p);
}

/*-------------------------------------------------OBBCollider_Component ColliderName()--------------------------------------------*/
/// <summary>
/// Returns the name of the collider
/// </summary>
/// <returns></returns>
const std::string& OBBCollider_Component::ColliderName()
{
    return std::string(data->name);
}

/*-------------------------------------------------OBBCollider_Component BoneName()--------------------------------------------*/
/// <summary>
/// Returns the name of the bone that the collider is attached to
/// </summary>
/// <returns></returns>
const std::string& OBBCollider_Component::BoneName()
{
    return data->bone_name;
}

/*-------------------------------------------------OBBCollider_Component Collider()--------------------------------------------*/

std::shared_ptr<COLLIDERS::COLLIDER_BASE>OBBCollider_Component::Collider()
{
    return collider;
}

/*-------------------------------------------------OBBCollider_Component Min()--------------------------------------------*/
/// <summary>
/// Returns the minimum point of the collider
/// </summary>
/// <returns></returns>
Vector3 OBBCollider_Component::Min()
{
    return data->size;
}

/*-------------------------------------------------OBBCollider_Component Max()--------------------------------------------*/
/// <summary>
/// Returns the maximum point of the collider
/// </summary>
/// <returns></returns>
Vector3 OBBCollider_Component::Max()
{
    return data->size;
}

/*-------------------------------------------------OBBCollider_Component Data()--------------------------------------------*/
/// <summary>
/// Returns the data class that stores all the data used in this component
/// </summary>
/// <returns></returns>
OBBCollider_Data* OBBCollider_Component::Data()
{
    return data;
}

/*-------------------------------------------------OBBCollider_Component GetComponentType()--------------------------------------------*/
COMPONENT_TYPE OBBCollider_Component::GetComponentType()
{
    return data->type;
}