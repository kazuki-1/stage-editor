#pragma once
#include "Base Classes/Component.h"

class Sprite2D_Data : public ComponentData
{
public:
    std::wstring image_path{};
    std::string image_name{};
    Vector2 size{1, 1};
    Vector2 uvPosition{};
    Vector2 uvSize{};
    Vector2 pivot{};
    Vector4 colour{1, 1, 1, 1};

    Sprite2D_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), image_path, image_name, size, uvPosition, uvSize, pivot, colour);
    }
};

class Sprite2D_Component : public Component
{
    std::shared_ptr<SPRITE>sprite;
    Sprite2D_Data* data;
public:
    Sprite2D_Component() {};
    Sprite2D_Component(GameObject* t, ComponentData* data);

    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (Sprite2D_Data) </para>
    /// <para>�R���|�l���g�������Ăяo���܂��B�Ή������R���|�l���g�f�[�^��p���Ă��̃R���|�l���g������������iSPHERE_COLLIDER_DATA�j</para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para>Called after Execute to perform any render functions </para>
    /// <para>Execute���s��ɕ`��p�֐����Ăяo���֐�</para>
    /// </summary>s
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
    Sprite2D_Data* Data();
    COMPONENT_TYPE GetComponentType() override;

};

CEREAL_REGISTER_TYPE(Sprite2D_Data)
