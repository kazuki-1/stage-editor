#include "../Base Classes/COMPONENT.h"
#include "../Headers/MESH.h"
#include "../Headers/TRANSFORM_3D.h"

namespace FS = std::filesystem;

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------MESH_DATA Class-----------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------MESH_DATA Constructor-----------------------------------------------------------*/

MESH_DATA::MESH_DATA()
{
    type = COMPONENT_TYPE::MESH;
}


/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------MESH Class-----------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------MESH Constructor-----------------------------------------------------------*/

MESH::MESH(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<MESH_DATA*>(data);
}

/*----------------------------------------------------------MESH Initialize()-----------------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (CAPSULE_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (CAPSULE COLLIDER DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT MESH::Initialize()
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

/*----------------------------------------------------------MESH Execute()-----------------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void MESH::Execute()
{
    TRANSFORM_3D* transform = GetComponent<TRANSFORM_3D>();
    if (!model)
        return;

    model->SetTransformation(transform->Scale(), transform->Quaternion(), transform->Translation());
    model->UpdateTransform();
}

/*----------------------------------------------------------MESH Render()-----------------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void MESH::Render()
{
    if (!model)
        return;
    model->Render();
}

/*----------------------------------------------------------MESH UI()-----------------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void MESH::UI()
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

/*----------------------------------------------------------MESH ModelPath()-----------------------------------------------------------*/
/// <summary>
/// Returns the path of the model loaded
/// </summary>
/// <returns></returns>
const std::string& MESH::ModelPath()
{
    return data->model_path;
}

/*----------------------------------------------------------MESH ModelName()-----------------------------------------------------------*/
/// <summary>
/// Returns the name of the model loaded
/// </summary>
/// <returns></returns>
const std::string& MESH::ModelName()
{
    return data->model_name;
}

/*----------------------------------------------------------MESH AnimationTake()-----------------------------------------------------------*/
/// <summary>
/// Returns the index of animation played
/// </summary>
/// <returns></returns>
int MESH::AnimationTake()
{
    return data->animationTake;
}

/*----------------------------------------------------------MESH PlayAnimationFlag()-----------------------------------------------------------*/
/// <summary>
/// Returns true if model is set to play animation
/// </summary>
/// <returns></returns>
bool MESH::PlayAnimationFlag()
{
    return data->playAnimationFlag;
}

/*----------------------------------------------------------MESH Model()-----------------------------------------------------------*/
/// <summary>
/// Returns the Model class of this component
/// </summary>
/// <returns></returns>
std::shared_ptr<MODEL>MESH::Model()
{
    return model;
}

/*----------------------------------------------------------MESH Data()-----------------------------------------------------------*/
/// <summary>
/// Returns the Data class of this class
/// </summary>
/// <returns></returns>
MESH_DATA* MESH::Data()
{
    return data;
}

/*----------------------------------------------------------MESH GetBoneTransform()-----------------------------------------------------------*/
/// <summary>
/// <para> Retrieves the world transform for the bone </para>
/// <para> 求められているボーンのワールド行列を返す </para>
/// </summary>
/// <returns></returns>
XMMATRIX MESH::GetBoneTransform(std::string name)
{
    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };


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
