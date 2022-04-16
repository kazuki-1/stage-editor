#pragma once
#include "Base Classes/Component.h"

enum class TERRAIN_AUDIO_PROPERTY
{
    MIN = -1,
    EMITTER,
    RECEIVER
};
class TERRAIN_AUDIO;


class TERRAIN_AUDIO_DATA_BASE : public ComponentData
{
public:
    char name[256] = "";
    template <class T>
    void serialize(T& t)
    {
        t(name);
    }
};

class TERRAIN_AUDIO_DATA_EMITTER : public TERRAIN_AUDIO_DATA_BASE
{
public:
    int mesh_index{ -1 };
    std::shared_ptr<AudioData>audio_data{};
    TERRAIN_AUDIO_DATA_EMITTER() { audio_data = std::make_shared<AudioData>(); };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TERRAIN_AUDIO_DATA_BASE>(this), mesh_index, audio_data);
    }
};

class TERRAIN_AUDIO_DATA_RECEIVER : public TERRAIN_AUDIO_DATA_BASE
{
public:
    TERRAIN_AUDIO_DATA_RECEIVER() {};
    std::vector<std::string>receiver_bones;
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TERRAIN_AUDIO_DATA_BASE>(this), receiver_bones);
    };
};


class TERRAIN_AUDIO_DATA : public ComponentData
{
public:

    friend class TERRAIN_AUDIO;
    TERRAIN_AUDIO_PROPERTY class_type{ -1 };
    std::shared_ptr<TERRAIN_AUDIO_DATA_BASE>property_data;
    TERRAIN_AUDIO_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), class_type, property_data);
    }
};





class TERRAIN_AUDIO : public Component
{
    struct AUDIO_BUFFER
    {
        std::shared_ptr<AUDIO>buffer;
        bool state{};
        int timer{};
    };
    TERRAIN_AUDIO_DATA* data{};
    std::vector<AUDIO_BUFFER>buffers;
public:
    TERRAIN_AUDIO() {};
    TERRAIN_AUDIO(GameObject* g, ComponentData* d);
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
    TERRAIN_AUDIO_DATA* Data();
    TERRAIN_AUDIO_PROPERTY Property();
    std::vector<AUDIO_BUFFER>Buffers();
    COMPONENT_TYPE GetComponentType() override;
};




CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA)
CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA_EMITTER)
CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA_RECEIVER)