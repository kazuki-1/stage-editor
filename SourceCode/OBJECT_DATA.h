#pragma once
#include <vector>
#include <memory>
//#include "COLLIDERS.h"
#include "Components/Base Classes/COMPONENT_DATA.h"
class GAMEOBJECT;
class GAMEOBJECT_2D;
enum class OBJECT_TYPE
{
    WALL, FLOOR, BACKDROP, PLAYER, ENEMY
};
class OBJECT_DATA
{
    std::string name;
    std::vector<std::shared_ptr<COMPONENT_DATA>>dataset;
    friend class GAMEOBJECT;
    friend class GAMEOBJECT_2D;
public:
    OBJECT_DATA() {};
    OBJECT_DATA(std::string n) : name(n) {};
    std::vector<std::shared_ptr<COMPONENT_DATA>>Dataset() { return dataset; }
    const std::string& Name() { return name; }
    template <class T>
    void Create()
    {
        int id{};
        for (auto& d : dataset)
        {
            if (dynamic_cast<T*>(d.get()))
                ++id;
        }
        dataset.push_back(std::make_shared<T>());
        dataset.back()->id = id;
    }

    template <class T>
    T* Retrieve(int id = 0)
    {
        int ind{};
        for (auto& d : dataset)
        {
            T* t = dynamic_cast<T*>(d.get());
            if (t != nullptr && id == ind)
                return t;
            else if (t != nullptr && id != ind)
                ++id;
        }
        return nullptr;
    }

    template <class T>
    void serialize(T& t)
    {
        t(name, dataset);
    }

    int DataIndex(std::shared_ptr<COMPONENT_DATA>target)
    {
        int ind{};
        for (auto& d : dataset)
        {
            if (target == d)
                return ind;
            ++ind;
        }
    }
    void Remove(std::shared_ptr<COMPONENT_DATA>target)
    {
        dataset.erase(dataset.begin() + DataIndex(target));
    }
    void Remove(int ind)
    {
        dataset.erase(dataset.begin() + ind);
    }


};


