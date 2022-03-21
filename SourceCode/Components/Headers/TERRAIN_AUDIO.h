#pragma once
#include "../Base Classes/COMPONENT.h"

enum class TERRAIN_AUDIO_PROPERTY
{
    EMITTER, 
    RECEIVER
};
class TERRAIN_AUDIO;


class TERRAIN_AUDIO_DATA_BASE : public COMPONENT_DATA
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
    int material_index{ -1 };
    std::shared_ptr<AUDIO_DATA>audio_data{};
    TERRAIN_AUDIO_DATA_EMITTER() { audio_data = std::make_shared<AUDIO_DATA>(); };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<TERRAIN_AUDIO_DATA_BASE>(this), material_index, audio_data);
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


class TERRAIN_AUDIO_DATA : public COMPONENT_DATA
{
public:
    
    friend class TERRAIN_AUDIO;
    TERRAIN_AUDIO_PROPERTY class_type;
    std::shared_ptr<TERRAIN_AUDIO_DATA_BASE>property_data;
    TERRAIN_AUDIO_DATA();
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), class_type, property_data);
    }
};





class TERRAIN_AUDIO : public COMPONENT
{
    TERRAIN_AUDIO_DATA* data{};
public:
    TERRAIN_AUDIO() {};
    TERRAIN_AUDIO(GAMEOBJECT* g, COMPONENT_DATA* d);
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
    COMPONENT_TYPE GetComponentType() override;
};




CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA)
CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA_EMITTER)
CEREAL_REGISTER_TYPE(TERRAIN_AUDIO_DATA_RECEIVER)