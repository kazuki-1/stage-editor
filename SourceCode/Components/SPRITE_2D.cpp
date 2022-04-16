#include "SPRITE_2D.h"
#include "TRANSFORM_2D.h"
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


SPRITE_2D::SPRITE_2D(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<SPRITE_2D_DATA*>(data);
}

/*---------------------------------------SPRITE_2D Initialize()----------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
/// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iSPHERE_COLLIDER_DATA�j</para>
/// </summary>
/// <returns></returns>
HRESULT SPRITE_2D::Initialize()
{
    if (data->image_path == L"")
        return S_OK;
    sprite = std::make_shared<SPRITE>(data->image_path.c_str());
    return sprite ? S_OK : E_FAIL;
}

/*---------------------------------------SPRITE_2D Render()----------------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute���s��ɕ`��p�֐����Ăяo���֐�</para>
/// </summary>
void SPRITE_2D::Render()
{
    TRANSFORM_2D* t = GetComponent<TRANSFORM_2D>();
    if (sprite)
        sprite->Render(t->Position(), data->size, data->uvPosition, data->uvSize, data->pivot, data->colour, t->Rotation());
}

/*---------------------------------------SPRITE_2D UI()----------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UI��`�悷��֐� </para>
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

Vector2& SPRITE_2D::Size()
{
    return data->size;
}

/*---------------------------------------SPRITE_2D UVPosition()----------------------------------------------------*/

Vector2& SPRITE_2D::UVPosition()
{
    return data->uvPosition;
}

/*---------------------------------------SPRITE_2D UVSize()----------------------------------------------------*/

Vector2& SPRITE_2D::UVSize()
{
    return data->uvSize;
}

/*---------------------------------------SPRITE_2D Pivot()----------------------------------------------------*/

Vector2& SPRITE_2D::Pivot()
{
    return data->pivot;
}

/*---------------------------------------SPRITE_2D Colour()----------------------------------------------------*/

Vector4& SPRITE_2D::Colour()
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