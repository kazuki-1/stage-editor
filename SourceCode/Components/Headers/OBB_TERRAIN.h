#pragma once
#include "../Base Classes/COMPONENT.h"
class OBB_COLLIDER;



// UNUSED
class OBB_TERRAIN_DATA : public COMPONENT_DATA
{
public:
    std::shared_ptr<AUDIO_DATA>audioData;
    std::shared_ptr<OBB_COLLIDER_DATA>colliderData;
    OBB_TERRAIN_DATA();
    bool overlap;
    bool triggerAudio;

    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), audioData, colliderData, overlap, triggerAudio);
    }
};

// UNUSED
class OBB_TERRAIN : public COMPONENT
{
    std::shared_ptr<OBB_COLLIDER>collider;
    std::shared_ptr<AUDIO>audio;
    OBB_TERRAIN_DATA* data;


public:
    OBB_TERRAIN() {};
    OBB_TERRAIN(GAMEOBJECT* g, COMPONENT_DATA* d);




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


























CEREAL_REGISTER_TYPE(OBB_TERRAIN_DATA)