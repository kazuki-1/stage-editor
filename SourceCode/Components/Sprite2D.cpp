#include "Sprite2D.h"
#include "Transform2D.h"
namespace FS = std::filesystem;

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Sprite2D_Data Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Sprite2D_Data Constructor-----------------------------------------------*/

Sprite2D_Data::Sprite2D_Data()
{
    type = COMPONENT_TYPE::SPRITE_2D;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Sprite2D_Component Class----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Sprite2D_Component Constructor----------------------------------------------------*/


Sprite2D_Component::Sprite2D_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<Sprite2D_Data*>(data);
}

/*---------------------------------------Sprite2D_Component Initialize()----------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (Sprite2D_Data) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT Sprite2D_Component::Initialize()
{
    if (data->image_path == L"")
        return S_OK;
    sprite = std::make_shared<SPRITE>(data->image_path.c_str());
    return sprite ? S_OK : E_FAIL;
}

/*---------------------------------------Sprite2D_Component Render()----------------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void Sprite2D_Component::Render()
{
    Transform2D_Component* t = GetComponent<Transform2D_Component>();
    if (sprite)
        sprite->Render(t->Position(), data->size, data->uvPosition, data->uvSize, data->pivot, data->colour, t->Rotation());
}

/*---------------------------------------Sprite2D_Component UI()----------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void Sprite2D_Component::UI()
{
    if (ImGui::TreeNode("Sprite 2d"))
    {
        IMGUI::Instance()->InputText("Image Path", &data->image_path);
        if (ImGui::Button("Load Sprite"))
        {
            if (!sprite)
                sprite = std::make_shared<SPRITE>();
            std::wstring directory{ L"./Data/Images/" };
            FS::path path(data->image_path);
            FS::path name{ path.filename() };
            name.replace_extension("");
            directory +=  path.filename().wstring();
            data->image_path = directory;
            sprite->Initialize(data->image_path.c_str());
            D3D11_TEXTURE2D_DESC t2d;
            sprite->Texture()->QueryTextureDesc(&t2d);

            data->uvSize = { (float)t2d.Width, (float)t2d.Height };
        }
        ImGui::DragFloat2("Image Size", &data->size.x, 0.05f);
        ImGui::DragFloat2("UV Position", &data->uvPosition.x, 0.05f);
        ImGui::DragFloat2("UV Size", &data->uvSize.x, 0.05f);
        ImGui::DragFloat2("Pivot", &data->pivot.x, 0.05f);
        ImGui::ColorEdit4("Colour : ", &data->colour.x);
        ImGui::TreePop();
    }
}

/*---------------------------------------Sprite2D_Component ImagePath()----------------------------------------------------*/

const std::wstring& Sprite2D_Component::ImagePath()
{
    return data->image_path;
}

/*---------------------------------------Sprite2D_Component ImageName()----------------------------------------------------*/

const std::string& Sprite2D_Component::ImageName()
{
    return data->image_name;
}

/*---------------------------------------Sprite2D_Component Size()----------------------------------------------------*/

Vector2& Sprite2D_Component::Size()
{
    return data->size;
}

/*---------------------------------------Sprite2D_Component UVPosition()----------------------------------------------------*/

Vector2& Sprite2D_Component::UVPosition()
{
    return data->uvPosition;
}

/*---------------------------------------Sprite2D_Component UVSize()----------------------------------------------------*/

Vector2& Sprite2D_Component::UVSize()
{
    return data->uvSize;
}

/*---------------------------------------Sprite2D_Component Pivot()----------------------------------------------------*/

Vector2& Sprite2D_Component::Pivot()
{
    return data->pivot;
}

/*---------------------------------------Sprite2D_Component Colour()----------------------------------------------------*/

Vector4& Sprite2D_Component::Colour()
{
    return data->colour;
}

/*---------------------------------------Sprite2D_Component Data()----------------------------------------------------*/

Sprite2D_Data* Sprite2D_Component::Data()
{
    return data;
}

/*----------------------------------------------------------Sprite2D_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE Sprite2D_Component::GetComponentType()
{
    return data->type;
}