#pragma once
#include "../../Engine/DXMath.h"
#include "../../Engine/CerealHeaders.h"
#include "../../Engine/COLLISION.h"
#include "../../Engine/Sprite.h"
#include "../../Engine/DEBUG_PRIMITIVE.h"
#include "ComponentData.h"
#include "EFFECTS.h"
#include "../../Engine/Audio.h"
#include "../../Engine/IMGUI.h"
#include "../../GAMEOBJECT.h"
using namespace Math;
class GameObject;
class DebugController;
class PlayerController_Component;





#pragma region BASE_COMPONENT

/// <summary>
/// Base class for components
/// </summary>
class Component
{
protected:
    GameObject* parent{};
public:
    Component() {};
    Component(GameObject* t, std::shared_ptr<ComponentData>data) {};
    /// <summary>
    /// <para> Virtual Function. Called when component is created </para>
    /// <para> 仮想関数。生成時に呼び出せます</para>
    /// </summary>
    /// <returns></returns>
    virtual HRESULT Initialize() { return S_OK; }
    /// <summary>
    /// <para> Virtual Function. Called every frame </para>
    /// <para> 仮想関数。フレームごっこに呼び出せる </para>
    /// </summary>
    virtual void Execute() {};
    /// <summary>
    /// <para> Virtual  Function. Called after Execute() to perform any render functions </para>
    /// <para> 仮想関数。Execute実行後に描画用関数を呼び出す関数</para>
    /// </summary>
    virtual void Render() {};
    /// <summary>
    /// <para> Virtual Function. Renders the UI of the component </para>
    /// <para> 仮想関数。UIを描画する関数 </para>
    /// </summary>
    virtual void UI() {};
    /// <summary>
    /// <para> Returns the GameObject of which this component is stored in </para>
    /// <para> このコンポネントの親となるゲームオブジェクトを戻る　</para>
    /// </summary>
    /// <returns></returns> 
    GameObject* Parent() { return parent; };

    /// <summary>
    /// Returns a component of the type specified from the GameObject. Returns nullptr if said component does not exist
    /// </summary>
    /// <param name="id">Used for when a GameObject has several components of the same type. Generally starts from 0.</param>
    /// <returns></returns>
    template <class T>
    T* GetComponent(int id = 0)
    {
        return parent->GetComponent<T>(id);
    }
    /// <summary>
    /// <para> Virtual function. Returns the COMPONENT_TYPE enum of the component</para>
    /// <para> 仮想関数。コンポネントのCOMPONENT_TYPEのenumを返す </para>
    /// </summary>
    /// <returns></returns>
    virtual COMPONENT_TYPE GetComponentType() { return COMPONENT_TYPE::BASE; }
};
/// <summary>
/// Base Audio Component. Should only be used when creating a new audio component and nothing else.
/// </summary>
class BASE_AUDIO_COMPONENT : public Component
{
protected:
    std::shared_ptr<AUDIO>audio;
    //AudioData* data;
public:
    BASE_AUDIO_COMPONENT() {};
    BASE_AUDIO_COMPONENT(GameObject* g, ComponentData* d) {};
    std::shared_ptr<AUDIO>Audio() { return audio; }
    //AudioData* Data() { return data; }
    //std::wstring FilePath() { return data->file_path; }
    //bool LoopFlag() { return data->loopFlag; }
    //float Volume() { return data->volume; }
};
/// <summary>
/// <para> Base class for characters such as Player and Enemy. </para>
/// <para> プレイヤーやエネミーのようなキャラクターの基礎クラス</para>
/// </summary>
class BaseCharacterComponent : public Component
{
protected:
    //CharacterData* data;
    virtual void UpdateRotation() {};
    virtual void AnimationSettings() {};
    virtual void Hit() {};
    virtual void Kill() {};

};
/// <summary>
/// Base collider component. Should only used when creating a new collider component and nothing else
/// </summary>
class BaseColliderComponent : public Component
{
public:
    virtual std::shared_ptr<COLLIDERS::COLLIDER_BASE>Collider() { return 0; }
    virtual bool Collide(GameObject* target) { return false; };
    virtual bool Collide(Vector3 p) { return false; }
    virtual void Execute(XMMATRIX transform) {};
    virtual Vector3 DistanceToPlayer(PlayerController_Component* target) { return {}; }
};


#pragma endregion




