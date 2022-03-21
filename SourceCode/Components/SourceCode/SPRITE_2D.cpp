#include "../Headers/SPRITE_2D.h"
#include "../Headers/TRANSFORM_2D.h"
namespace FS = std::filesystem;

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPRITE_2D_DATA Class-----------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPRITE_2D_DATA Constructor-----------------------------------------------*/

SPRITE_2D_DATA::SPRITE_2D_DATA()
{
    type = COMPONENT_TYPE::SPRITE_2D;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPRITE_2D Class----------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------SPRITE_2D Constructor----------------------------------------------------*/


SPRITE_2D::SPRITE_2D(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<SPRITE_2D_DATA*>(data);
}

/*---------------------------------------SPRITE_2D Initialize()----------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（SPHERE_COLLIDER_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT SPRITE_2D::Initialize()
{
    if (data->image_path == L"")
        return S_OK;
    sprite = std::make_shared<SPRITE>(data->image_path.c_str(), DirectX11::Instance()->Device());
    return sprite ? S_OK : E_FAIL;
}

/*---------------------------------------SPRITE_2D Render()----------------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void SPRITE_2D::Render()
{
    TRANSFORM_2D* t = GetComponent<TRANSFORM_2D>();
    if (sprite)
        sprite->Render(t->Position().XMF2(), data->size.XMF2(), data->uvPosition.XMF2(), data->uvSize.XMF2(), data->pivot.XMF2(), data->colour.XMF4(), t->Rotation());
}

/*---------------------------------------SPRITE_2D UI()----------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void SPRITE_2D::UI()
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
            sprite->Initialize(data->image_path.c_str(), DirectX11::Instance()->Device());
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

/*---------------------------------------SPRITE_2D ImagePath()----------------------------------------------------*/

const std::wstring& SPRITE_2D::ImagePath()
{
    return data->image_path;
}

/*---------------------------------------SPRITE_2D ImageName()----------------------------------------------------*/

const std::string& SPRITE_2D::ImageName()
{
    return data->image_name;
}

/*---------------------------------------SPRITE_2D Size()----------------------------------------------------*/

VECTOR2& SPRITE_2D::Size()
{
    return data->size;
}

/*---------------------------------------SPRITE_2D UVPosition()----------------------------------------------------*/

VECTOR2& SPRITE_2D::UVPosition()
{
    return data->uvPosition;
}

/*---------------------------------------SPRITE_2D UVSize()----------------------------------------------------*/

VECTOR2& SPRITE_2D::UVSize()
{
    return data->uvSize;
}

/*---------------------------------------SPRITE_2D Pivot()----------------------------------------------------*/

VECTOR2& SPRITE_2D::Pivot()
{
    return data->pivot;
}

/*---------------------------------------SPRITE_2D Colour()----------------------------------------------------*/

VECTOR4& SPRITE_2D::Colour()
{
    return data->colour;
}

/*---------------------------------------SPRITE_2D Data()----------------------------------------------------*/

SPRITE_2D_DATA* SPRITE_2D::Data()
{
    return data;
}

/*----------------------------------------------------------SPRITE_2D GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE SPRITE_2D::GetComponentType()
{
    return data->type;
}