#pragma once
#include "Base Classes/Component.h"
#include "OBBCollider.h"
class TerrainAudio_Component;




enum class TerrainAudio_Property
{
    MIN = -1,
    EMITTER,
    RECEIVER
};


class TerrainAudio_Data_Base : public ComponentData
{
public:
    char name[256] = "";
    template <class T>
    void serialize(T& t)
    {
        t(name);
    }
};

class TerrainAudio_Data_Emitter : public TerrainAudio_Data_Base
{
public:
    std::shared_ptr<OBBCollider_Data>collider_data;
    std::shared_ptr<AudioData>audio_data;
    TerrainAudio_Data_Emitter() { audio_data = std::make_shared<AudioData>(); collider_data = std::make_shared<OBBCollider_Data>(); };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TerrainAudio_Data_Base>(this), collider_data, audio_data);
    }
};

class TerrainAudio_Data_Receiver : public TerrainAudio_Data_Base
{
public:
    TerrainAudio_Data_Receiver() {};
    struct Parameter
    {
        int animation_take{ 0 };
        int animation_frame{ 0 };
        template <class T>
        void serialize(T& t)
        {
            t(animation_take, animation_frame);
        }
    };
    std::vector<Parameter>parameters;

    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TerrainAudio_Data_Base>(this), parameters);
    };
};


class TerrainAudio_Data : public ComponentData
{
public:

    friend class TerrainAudio_Component;
    TerrainAudio_Property class_type{ -1 };
    std::shared_ptr<TerrainAudio_Data_Base>property_data;
    TerrainAudio_Data();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), class_type, property_data);
    }
};

class TerrainAudio_Internal_Base
{
public:
    friend class TerrainAudio_Component;
    virtual void Initialize(TerrainAudio_Component* parent, TerrainAudio_Data_Base* data) {}
    virtual void Finalize() {};
};


class TerrainAudio_Emitter : public TerrainAudio_Internal_Base
{
    friend class TerrainAudio_Component;


    std::shared_ptr<OBBCollider_Component>collider;
    struct AUDIO_BUFFER
    {
        std::shared_ptr<Audio>buffer;
        bool state{};
        int timer{};
    };
    std::vector<AUDIO_BUFFER>buffers;

public:
    void Initialize(TerrainAudio_Component* parent, TerrainAudio_Data_Base* data);
    void Finalize() override;;
};


class TerrainAudio_Receiver : public TerrainAudio_Internal_Base
{
public:
    int timer{};
};

class TerrainAudio_Component : public Component
{
    friend class TerrainAudio_Emitter;
    friend class TerrainAudio_Receiver;

    void ExecuteEmitter();
    void ExecuteReceiver();

    TerrainAudio_Data* data{};
    std::shared_ptr<TerrainAudio_Internal_Base> internal_component;
public:
    TerrainAudio_Component() {};
    TerrainAudio_Component(GameObject* g, ComponentData* d);
    /// <summary>
    /// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
    /// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called each frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// Called only when in UI
    /// </summary>
    void ExecuteUI();
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
    /// Called when component is destroyed
    /// </summary>
    void Finalize();
    /// <summary>
    /// <para> Cycles through the buffer for unplayed audio and play it</para>
    /// <para> バッファーの中にプレイしてないサウンドを探しプレイする</para>
    /// </summary>
    void Play();





    TerrainAudio_Data* Data();
    TerrainAudio_Property Property();
    //std::vector<AUDIO_BUFFER>Buffers();
    COMPONENT_TYPE GetComponentType() override;
};




CEREAL_REGISTER_TYPE(TerrainAudio_Data)
CEREAL_REGISTER_TYPE(TerrainAudio_Data_Emitter)
CEREAL_REGISTER_TYPE(TerrainAudio_Data_Receiver)