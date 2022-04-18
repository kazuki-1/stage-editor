#include "Base Classes/Component.h"
#include "Mesh.h"
#include "Transform3D.h"

namespace FS = std::filesystem;

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Mesh_Data Class-----------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Mesh_Data Constructor-----------------------------------------------------------*/

Mesh_Data::Mesh_Data()
{
    type = COMPONENT_TYPE::MESH;
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Mesh_Component Class-----------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------Mesh_Component Constructor-----------------------------------------------------------*/

Mesh_Component::Mesh_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<Mesh_Data*>(data);
}

/*----------------------------------------------------------Mesh_Component Initialize()-----------------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (CapsuleCollider_Data)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (CAPSULE COLLIDER DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT Mesh_Component::Initialize()
{
    if (data->model_path != "")
    {
        model = std::make_shared<MODEL>();
        model->Initialize(data->model_path);
        model->SetTake(data->animationTake);
        model->SetScale({ 1, 1, 1 });
    }
    return S_OK;
}

/*----------------------------------------------------------Mesh_Component Execute()-----------------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void Mesh_Component::Execute()
{
    Transform3D_Component* transform = GetComponent<Transform3D_Component>();
    if (!model)
        return;

    model->SetTransformation(transform->Scale(), transform->Quaternion(), transform->Translation());
    model->UpdateTransform();
}

/*----------------------------------------------------------Mesh_Component Render()-----------------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void Mesh_Component::Render()
{
    if (!model)
        return;
    model->Render();
}

/*----------------------------------------------------------Mesh_Component UI()-----------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void Mesh_Component::UI()
{
    if (ImGui::TreeNode("Mesh"))
    {
        IMGUI::Instance()->InputText("Model Path", &data->model_path);
        if (ImGui::Button("Load Model"))
        {
            std::string directory{ "./Data/Model/" };
            FS::path path(data->model_path);
            FS::path name(path.filename().string());
            std::string full_name{ name.string() };
            name.replace_extension("");
            directory += name.string() + "/" + full_name;
            data->model_path = directory;
            data->model_name = name.string();

            model = std::make_shared<MODEL>();

            model->Initialize(data->model_path);
        }
        if (!model)
        {
            ImGui::TreePop();
            return;
        }
        std::vector<MODEL_RESOURCES::ANIMATION>& anims{ model->Resource()->Animations };
        ImGui::ListBoxHeader("Animations", { 300, 200 });
        for (int a = 0; a < model->Resource()->Animations.size(); ++a)
        {
            bool s{};
            if (ImGui::Selectable(anims[a].Name.c_str(), &s)) {
                data->animationTake = a;
                break;
            }
        }
        ImGui::ListBoxFooter();
        model->SetTake(data->animationTake);
        ImGui::Checkbox("Plays Animation", &data->playAnimationFlag);
        data->playAnimationFlag ? model->ResumeAnim() : model->PauseAnim();

        ImGui::TreePop();
    }
}

/*----------------------------------------------------------Mesh_Component ModelPath()-----------------------------------------------------------*/
/// <summary>
/// Returns the path of the model loaded
/// </summary>
/// <returns></returns>
const std::string& Mesh_Component::ModelPath()
{
    return data->model_path;
}

/*----------------------------------------------------------Mesh_Component ModelName()-----------------------------------------------------------*/
/// <summary>
/// Returns the name of the model loaded
/// </summary>
/// <returns></returns>
const std::string& Mesh_Component::ModelName()
{
    return data->model_name;
}

/*----------------------------------------------------------Mesh_Component AnimationTake()-----------------------------------------------------------*/
/// <summary>
/// Returns the index of animation played
/// </summary>
/// <returns></returns>
int Mesh_Component::AnimationTake()
{
    return data->animationTake;
}

/*----------------------------------------------------------Mesh_Component PlayAnimationFlag()-----------------------------------------------------------*/
/// <summary>
/// Returns true if model is set to play animation
/// </summary>
/// <returns></returns>
bool Mesh_Component::PlayAnimationFlag()
{
    return data->playAnimationFlag;
}

/*----------------------------------------------------------Mesh_Component Model()-----------------------------------------------------------*/
/// <summary>
/// Returns the Model class of this component
/// </summary>
/// <returns></returns>
std::shared_ptr<MODEL>Mesh_Component::Model()
{
    return model;
}

/*----------------------------------------------------------Mesh_Component Data()-----------------------------------------------------------*/
/// <summary>
/// Returns the Data class of this class
/// </summary>
/// <returns></returns>
Mesh_Data* Mesh_Component::Data()
{
    return data;
}

/*----------------------------------------------------------Mesh_Component GetBoneTransform()-----------------------------------------------------------*/
/// <summary>
/// <para> Retrieves the world transform for the bone </para>
/// <para> 求められているボーンのワールド行列を返す </para>
/// </summary>
/// <returns></returns>
XMMATRIX Mesh_Component::GetBoneTransform(std::string name)
{
    Transform3D_Component* transform{ GetComponent<Transform3D_Component>() };


    int64_t index{ -1 };
    XMMATRIX bone, global;
    for (auto& m : model->Resource()->Meshes)
    {
        for (auto& b : m.Bind_Pose.Bones)
        {
            if (b.Name == name)
            {
                index = b.n_Index;
                break;
            }
        }
    }
    if (index != -1)
    {
        int kf{ model->NextFrame() };
        MODEL_RESOURCES::ANIMATION& an{ model->Resource()->Animations[model->CurrentTake()] };

        int size{ (int)an.Keyframes.size() - 1 };
        int take{ model->CurrentFrame() };
        kf = (std::min)((std::max)(kf, 0), (int)an.Keyframes.size() - 1);
        XMFLOAT4X4 temp = model->Resource()->Animations[model->CurrentTake()].Keyframes[kf].Nodes.at(index).g_Transform;
        bone = XMLoadFloat4x4(&temp);
    }
    global = transform->TransformMatrix();;
    XMFLOAT4X4 temp{ model->Resource()->Axises.AxisCoords };

    bone *= XMLoadFloat4x4(&temp);
    bone *= global;
    return bone;
}

/*----------------------------------------------------------Mesh_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE Mesh_Component::GetComponentType()
{
    return data->type;
}