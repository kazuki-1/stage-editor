#pragma once
#include "../Base Classes/COMPONENT.h"

class MESH_DATA : public COMPONENT_DATA
{
public:
    std::string model_path{};
    std::string model_name{};
    int animationTake{};
    bool playAnimationFlag{ true };

    MESH_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), model_path, model_name, animationTake, playAnimationFlag);
    }

};

class MESH : public COMPONENT
{
    std::shared_ptr<MODEL>model{};
    MESH_DATA* data;
public:
    MESH() {};
    MESH(GAMEOBJECT* t, COMPONENT_DATA* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (MESH_DATA)</para>
    /// <para> �������ɌĂяo���B�Ώۂ̃f�[�^���g���ď��������� (MESH_DATA) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> ���t���[���ɌĂяo�� </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Called after Execute() to perform any render functions </para>
    /// <para> Execute()��Ƀ����_�[�֐������s����悤�ɖ��t���[���ɌĂяo�� </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UI��`�� </para>
    /// </summary>
    void UI() override;
    /// <summary>
    /// Returns the path of the model loaded
    /// </summary>
    /// <returns></returns>
    const std::string& ModelPath();
    /// <summary>
    /// Returns the name of the model loaded
    /// </summary>
    /// <returns></returns>
    const std::string& ModelName();
    /// <summary>
    /// Returns the index of animation played
    /// </summary>
    /// <returns></returns>
    int AnimationTake();
    /// <summary>
    /// Returns true if model is set to play animation
    /// </summary>
    /// <returns></returns>
    bool PlayAnimationFlag();
    /// <summary>
    /// Returns the Model class of this component
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<MODEL>Model();
    /// <summary>
    /// Returns the Data class of this class
    /// </summary>
    /// <returns></returns>
    MESH_DATA* Data();
    /// <summary>
    /// <para> Retrieves the world transform for the bone </para>
    /// <para> ���߂��Ă���{�[���̃��[���h�s���Ԃ� </para>
    /// </summary>
    /// <returns></returns>
    XMMATRIX GetBoneTransform(std::string name);




};

CEREAL_REGISTER_TYPE(MESH_DATA)
