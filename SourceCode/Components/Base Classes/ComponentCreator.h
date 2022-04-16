#pragma once
#include <functional>
#include <map>
#include "../../Engine/Singleton.h"
enum class COMPONENT_TYPE;
class Component;
class GameObject;
class ComponentData;



class ComponentCreator : public Singleton<ComponentCreator>
{
    std::map<COMPONENT_TYPE, std::function<std::shared_ptr<Component>(GameObject* g, std::shared_ptr<ComponentData>)>>componentTypes;
    std::map<COMPONENT_TYPE, std::function<std::shared_ptr<ComponentData>()>>dataTypes;
    void RegisterComponent(COMPONENT_TYPE t, std::function<std::shared_ptr<Component>(GameObject*, std::shared_ptr<ComponentData>)> constructor);
    void RegisterData(COMPONENT_TYPE t, std::function<std::shared_ptr<ComponentData>()>constructor);
public:
    /// <summary>
    /// <para> Register all the types of component and its respective component data into the map </para>
    /// <para> �e�R���|�l���g�y�ъY������R���|�l���g���}�b�v�ɓo�^����</para>
    /// </summary>
    void Initialize();
    /// <summary>
    /// <para> Instantiate a component based on the data type</para>
    /// </summary>
    /// <param name="g : ">GameObject parent, basically the gameobject where you are registering the component to</param>
    /// <param name="data"></param>
    /// <returns></returns>
    std::shared_ptr<Component>Instantiate(GameObject* g, std::shared_ptr<ComponentData>data);
    /// <summary>
    /// <para> Instantiate a component data based on the component type </para>
    /// <para> �����ɂ���R���|�l���g�^�C�v����Â��R���|�l���g�f�[�^�𐶐��@</para>
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<ComponentData>Instantiate(COMPONENT_TYPE t);
};

#define INSTANTIATE ComponentCreator::Instance()->Instantiate
