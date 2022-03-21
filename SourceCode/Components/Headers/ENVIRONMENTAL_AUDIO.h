#pragma once
#include "../Base Classes/COMPONENT.h"

class ENVIRONMENTAL_AUDIO;
class TRANSFORM_3D_DATA;
class TRANSFORM_3D;
class LOCALIZED_COLLIDER;
/// <summary>
/// Stores the data for the localized collider
/// </summary>
class LOCALIZED_COLLIDER_DATA 
{
    LOCALIZED_COLLIDER* parent{};
public:
    std::shared_ptr<TRANSFORM_3D_DATA>transform;
    std::shared_ptr<COMPONENT_DATA>collider_data;
    // Minimum distance to play max volume
    // 最大音量を出力する最短距離
    float minimum_distance{};
    LOCALIZED_COLLIDER_DATA() {};
    LOCALIZED_COLLIDER_DATA(COMPONENT_TYPE t);
    /// <summary>
    /// <para> Called when creating from data. Links both data to its respective data </para>
    /// <para> データロードするときに呼び出す。データをリンクする</para>
    /// </summary>
    /// <param name="t3d"> : TRANSFORD_3D_DATA</param>
    /// <param name="cd"> : collider Data</param>
    /// <returns></returns>
    HRESULT Initialize(std::shared_ptr<COMPONENT_DATA>t3d, std::shared_ptr<COMPONENT_DATA>cd);
    template <class T>
    void serialize(T& t)
    {
        t(transform, collider_data, minimum_distance);
    }
};
/// <summary>
/// <para> As we are using the collider components, it needs its own localized transform component, so we group them together </para>
/// <para> コライダーコンポネントを使ってるため、当然自身のTRANSFORMコンポネントも必要されたのでこちらはクラス化する</para>
/// </summary>
class LOCALIZED_COLLIDER
{
    std::shared_ptr<BASE_COLLIDER_COMPONENT>collider;
    std::shared_ptr<TRANSFORM_3D>transform;
    LOCALIZED_COLLIDER_DATA* data;
    ENVIRONMENTAL_AUDIO* parent;
    friend ENVIRONMENTAL_AUDIO;
public:
    /// <summary>
    /// <para> Use this when creating a localized collider from existing data </para>
    /// <para> 既存データを使ってLocalized Colliderを生成時に呼び出す</para>
    /// </summary>
    LOCALIZED_COLLIDER(ENVIRONMENTAL_AUDIO* g, LOCALIZED_COLLIDER_DATA* data);
    /// <summary>
    /// <para> Use this when creating a localized collider</para>
    /// <para> LOCALIZED_COLLIDERを生成時に呼び出す</para>
    /// </summary>
    LOCALIZED_COLLIDER(GAMEOBJECT* g, LOCALIZED_COLLIDER_DATA* data);
    /// <summary>
    /// <para> Called when created from data </para>
    /// <para> データから生成時に呼び出す </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute();
    /// <summary>
    /// <para> Perform rendering functions </para>
    /// <para> 描画関数を呼び出す </para>
    /// </summary>
    void Render();
    /// <summary>
    /// <para> Renders the UI </para>
    /// <para> UIを描画 </para>
    /// </summary>
    void UI();

    std::shared_ptr<BASE_COLLIDER_COMPONENT>Collider();
    std::shared_ptr<TRANSFORM_3D>Transform();

    LOCALIZED_COLLIDER_DATA* Data();


};




class ENVIRONMENTAL_AUDIO_DATA : public AUDIO_DATA
{

public:
    // For storing each collider data for exporting purposes
    // エキスパート用のコライダーデータを格納するため
    std::vector<std::shared_ptr<LOCALIZED_COLLIDER_DATA>>collider_dataset;
    ENVIRONMENTAL_AUDIO_DATA() { type = COMPONENT_TYPE::ENV_AUDIO; };
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<AUDIO_DATA>(this), collider_dataset);
    }
};
class ENVIRONMENTAL_AUDIO : public BASE_AUDIO_COMPONENT
{
    // We are using the collider component instead of the code based component for convenience
    // 便利さを優先するためコードベースコライダーではなくコンポネントベースのコライダーを使う
    std::vector < std::shared_ptr<LOCALIZED_COLLIDER>>colliders;
    ENVIRONMENTAL_AUDIO_DATA* data;

    /// <summary>
    /// <para> Creates a localized collider and insert it into the dataset </para>
    /// <para> LOCALIZED_COLLIDERを生成し、データセットに登録 </para>
    /// </summary>
    void CreateLocalizedCollider(COMPONENT_TYPE t);
    friend LOCALIZED_COLLIDER;
public:
    ENVIRONMENTAL_AUDIO() {};
    ENVIRONMENTAL_AUDIO(GAMEOBJECT* t, COMPONENT_DATA* d);
    /// <summary>
    /// <para> Called when created from data </para>
    /// <para> データから生成時に呼び出す </para>
    /// </summary>
    /// <returns></returns>
    HRESULT Initialize() override;
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    void Execute() override;
    /// <summary>
    /// <para> Perform rendering functions </para>
    /// <para> 描画関数を呼び出す </para>
    /// </summary>
    void Render() override;
    /// <summary>
    /// <para> Renders the UI </para>
    /// <para> UIを描画 </para>
    /// </summary>
    void UI() override;

    COMPONENT_TYPE GetComponentType() override;
    std::vector<std::shared_ptr<LOCALIZED_COLLIDER>>Colliders();

};


CEREAL_REGISTER_TYPE(ENVIRONMENTAL_AUDIO_DATA)
