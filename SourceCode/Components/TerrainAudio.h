#pragma once
#include "Base Classes/Component.h"

enum class TerrainAudio_Property
{
    MIN = -1,
    EMITTER,
    RECEIVER
};
class TerrainAudio_Component;


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
    int mesh_index{ -1 };
    std::shared_ptr<AudioData>audio_data{};
    TerrainAudio_Data_Emitter() { audio_data = std::make_shared<AudioData>(); };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TerrainAudio_Data_Base>(this), mesh_index, audio_data);
    }
};

class TerrainAudio_Data_Receiver : public TerrainAudio_Data_Base
{
public:
    TerrainAudio_Data_Receiver() {};
    std::vector<std::string>receiver_bones;
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TerrainAudio_Data_Base>(this), receiver_bones);
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





class TerrainAudio_Component : public Component
{
    struct AUDIO_BUFFER
    {
        std::shared_ptr<AUDIO>buffer;
        bool state{};
        int timer{};
    };
    TerrainAudio_Data* data{};
    std::vector<AUDIO_BUFFER>buffers;
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
    /// <para> Cycles through the buffer for unplayed audio and play it</para>
    /// <para> バッファーの中にプレイしてないサウンドを探しプレイする</para>
    /// </summary>
    void Play();
    TerrainAudio_Data* Data();
    TerrainAudio_Property Property();
    std::vector<AUDIO_BUFFER>Buffers();
    COMPONENT_TYPE GetComponentType() override;
};




CEREAL_REGISTER_TYPE(TerrainAudio_Data)
CEREAL_REGISTER_TYPE(TerrainAudio_Data_Emitter)
CEREAL_REGISTER_TYPE(TerrainAudio_Data_Receiver)