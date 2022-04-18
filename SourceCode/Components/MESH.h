#pragma once
#include "Base Classes/Component.h"

class Mesh_Data : public ComponentData
{
public:
    std::string model_path{};
    std::string model_name{};
    int animationTake{};
    bool playAnimationFlag{ true };

    Mesh_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), model_path, model_name, animationTake, playAnimationFlag);
    }

};

class Mesh_Component : public Component
{
    std::shared_ptr<MODEL>model{};
    Mesh_Data* data;
public:
    Mesh_Component() {};
    Mesh_Component(GameObject* t, ComponentData* data);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (Mesh_Data)</para>
    /// <para> 生成時に呼び出す。対象のデータを使って初期化する (Mesh_Data) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Called after Execute() to perform any render functions </para>
    /// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI for this component </para>
    /// <para> UIを描画 </para>
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
    Mesh_Data* Data();
    /// <summary>
    /// <para> Retrieves the world transform for the bone </para>
    /// <para> 求められているボーンのワールド行列を返す </para>
    /// </summary>
    /// <returns></returns>
    XMMATRIX GetBoneTransform(std::string name);
    COMPONENT_TYPE GetComponentType() override;



};

CEREAL_REGISTER_TYPE(Mesh_Data)
