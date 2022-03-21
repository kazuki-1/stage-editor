#pragma once
#include <functional>
#include <map>
#include "../../Engine/Singleton.h"
enum class COMPONENT_TYPE;
class COMPONENT;
class GAMEOBJECT;
class COMPONENT_DATA;



class COMPONENT_CREATOR : public SINGLETON<COMPONENT_CREATOR>
{
    std::map<COMPONENT_TYPE, std::function<std::shared_ptr<COMPONENT>(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>)>>componentTypes;
    std::map<COMPONENT_TYPE, std::function<std::shared_ptr<COMPONENT_DATA>()>>dataTypes;
    void RegisterComponent(COMPONENT_TYPE t, std::function<std::shared_ptr<COMPONENT>(GAMEOBJECT*, std::shared_ptr<COMPONENT_DATA>)> constructor);
    void RegisterData(COMPONENT_TYPE t, std::function<std::shared_ptr<COMPONENT_DATA>()>constructor);
public:
    /// <summary>
    /// <para> Register all the types of component and its respective component data into the map </para>
    /// <para> 各コンポネント及び該当するコンポネントをマップに登録する</para>
    /// </summary>
    void Initialize();
    /// <summary>
    /// <para> Instantiate a component based on the data type</para>
    /// </summary>
    /// <param name="g : ">GameObject parent, basically the gameobject where you are registering the component to</param>
    /// <param name="data"></param>
    /// <returns></returns>
    std::shared_ptr<COMPONENT>Instantiate(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>data);
    /// <summary>
    /// <para> Instantiate a component data based on the component type </para>
    /// <para> 引数にあるコンポネントタイプを基づきコンポネントデータを生成　</para>
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<COMPONENT_DATA>Instantiate(COMPONENT_TYPE t);
};

#define INSTANTIATE COMPONENT_CREATOR::Instance()->Instantiate
