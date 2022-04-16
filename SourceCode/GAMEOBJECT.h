#pragma once
#include "MODEL.h"
#include "Engine/Singleton.h"
#include "Engine/DXMath.h"
#include "OBJECT_DATA.h"
//#include "COLLIDERS.h"
//#include "Component.h"
class Component;

class GameObject
{
protected:
    std::shared_ptr<OBJECT_DATA>data;
    std::vector<std::shared_ptr<Component>>components;
    char name_buffer[256] = "";
    bool windowActive{};

    friend class OBJECT_DATA;
    void __InternalAddComponent(COMPONENT_TYPE t);
    void __InternalAddComponent(COMPONENT_TYPE t, std::shared_ptr<ComponentData>d);
public:
    GameObject() {};
    /// <summary>
    /// <para> Links the OBJECT_DATA to the gameObject </para>
    /// <para> OBJECT_DATAとゲームオブジェクトをリンク </para>
    /// </summary>
    GameObject(std::shared_ptr<OBJECT_DATA>d);
    ~GameObject() {}
    /// <summary>
    /// <para> Call after constructor </para>
    /// <para> 生成後にこれを呼び出す</para>
    /// </summary>
    /// <returns></returns>
    virtual HRESULT Initialize();
    /// <summary>
    /// <para> Called every frame </para>
    /// <para> 毎フレームに呼び出す </para>
    /// </summary>
    virtual void Execute();
    /// <summary>
    /// Called every frame
    /// </summary>
    virtual void ExecuteUI();
    /// <summary>
    /// <para> Called every frame to render all components </para>
    /// <para> コンポネントを描画するように毎フレームに呼び出す </para>
    /// </summary>
    virtual void Render();
    /// <summary>
    /// <para> Called every frame to render all component's UI </para>
    /// <para> コンポネントのUIを描画するように毎フレームに呼び出す </para>
    /// </summary>
    virtual void RenderUI();
    /// <summary>
    /// <para> Reset static members, called when switching to another gameObject </para>
    /// <para> Static変数をすべてリセットするこ。別のゲームオブジェクトを選択するときに呼び出す </para>
    /// </summary>
    void Reset();
    /// <summary>
    /// <para> Called to activate the gameObject window</para>
    /// <para> ゲームオブジェクトウィンドウを起動するように呼び出す</para>
    /// </summary>
    void Activate();
    /// <summary>
    /// <para> Called to deactivate the gameObject window</para>
    /// <para> ゲームオブジェクトウィンドウを閉じるように呼び出す</para>
    /// </summary>
    void Deactivate();
    virtual void AddComponent(COMPONENT_TYPE t);
    void RemoveComponent(int id);

    
    template <class T>
    T* GetComponent(int id = 0)
    {
        int ind{};
        for (auto& c : components)
        {
            T* t = dynamic_cast<T*>(c.get());
            if (t && ind == id)
                return t;
            else if (t)
                ++ind;
        }
        return nullptr;
    }
    std::vector<std::shared_ptr<Component>>Components();
    std::shared_ptr<OBJECT_DATA>Data();
};

class GameObjectManager : public Singleton<GameObjectManager>
{
    std::map<std::string, std::shared_ptr<GameObject>>gameObjects;
public:
    /// <summary>
    /// <para> Create a new gameObject and insert it into the map </para>
    /// <para> 新しいゲームオブジェクトを生成して、マップに登録する</para>
    /// </summary>
    /// <param name="n"> : Name of gameObject</param>
    /// <param name="d"> : OBJECT_DATA class, will be linked to generated gameObject</param>
    void Insert(std::string n, std::shared_ptr<OBJECT_DATA>d);
    /// <summary>
    /// <para> Create a new gameObject2D and insert it into the map </para>
    /// <para> 新しいゲームオブジェクト2Dを生成して、マップに登録する</para>
    /// </summary>
    /// <param name="n"> : Name of gameObject</param>
    /// <param name="d"> : OBJECT_DATA class, will be linked to generated gameObject</param>
    void Insert2D(std::string n, std::shared_ptr<OBJECT_DATA>d);
    void Remove(std::string name);
    void Remove(std::shared_ptr<GameObject>gameObject);
    void Clear();
    std::shared_ptr<GameObject>Retrieve(std::string);
    std::string RetrieveName(std::shared_ptr<GameObject>go);
    std::map<std::string, std::shared_ptr<GameObject>>GetGameObjects();


    /// <summary>
    /// Called at the start of the stage. Initializes all gameObjects and their components
    /// </summary>
    void Initialize();
    /// <summary>
    /// Performs Game functions. Called every frame to perform functions
    /// </summary>
    void Execute();
    /// <summary>
    /// Performs UI functions. Called every frame to perform functions. 
    /// </summary>
    void ExecuteUI();
    /// <summary>
    /// Renders objects. Called every frame to perform rendering
    /// </summary>
    void Render();
    /// <summary>
    /// Renders UI. Called every frame to perform rendering
    /// </summary>
    void RenderUI();
    /// <summary>
    /// Called at the end of program to finalize the manager
    /// </summary>
    void Finalize();

};


#define GAMEOBJECTS GameObjectManager::Instance()
