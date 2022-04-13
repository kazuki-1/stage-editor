#pragma once
#include "Base Classes/COMPONENT.h"

class SPRITE_2D_DATA : public COMPONENT_DATA
{
public:
    std::wstring image_path{};
    std::string image_name{};
    Vector2 size{1, 1};
    Vector2 uvPosition{};
    Vector2 uvSize{};
    Vector2 pivot{};
    Vector4 colour{1, 1, 1, 1};

    SPRITE_2D_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), image_path, image_name, size, uvPosition, uvSize, pivot, colour);
    }
};

class SPRITE_2D : public COMPONENT
{
    std::shared_ptr<SPRITE>sprite;
    SPRITE_2D_DATA* data;
public:
    SPRITE_2D() {};
    SPRITE_2D(GAMEOBJECT* t, COMPONENT_DATA* data);

    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (SPHERE_COLLIDER_DATA) </para>
    /// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iSPHERE_COLLIDER_DATA�j</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para>Called after Execute to perform any render functions </para>
    /// <para>Execute���s��ɕ`��p�֐����Ăяo���֐�</para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UI��`�悷��֐� </para>
    /// </summary>
    void UI() override;

    const std::wstring& ImagePath();
    const std::string& ImageName();
    Vector2& Size();
    Vector2& UVPosition();
    Vector2& UVSize();
    Vector2& Pivot();
    Vector4& Colour();
    SPRITE_2D_DATA* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(SPRITE_2D_DATA)
