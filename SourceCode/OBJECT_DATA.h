#pragma once
#include <vector>
#include <memory>
//#include "COLLIDERS.h"
#include "Components/Base Classes/ComponentData.h"
class GameObject;
class GameObject_2D;
class DataManager;
enum class OBJECT_TYPE
{
    WALL, FLOOR, BACKDROP, PLAYER, ENEMY
};
class OBJECT_DATA
{
    std::string name;
    std::vector<std::shared_ptr<ComponentData>>dataset;

    int id{};
    friend class DataManager;
    friend class GameObject;
    friend class GameObject_2D;
public:
    OBJECT_DATA() {};
    OBJECT_DATA(std::string n) : name(n) {};
    std::vector<std::shared_ptr<ComponentData>>Dataset() { return dataset; }
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

    int DataIndex(std::shared_ptr<ComponentData>target)
    {
        int ind{};
        for (auto& d : dataset)
        {
            if (target == d)
                return ind;
            ++ind;
        }
    }
    void Remove(std::shared_ptr<ComponentData>target)
    {
        dataset.erase(dataset.begin() + DataIndex(target));
    }
    void Remove(int ind)
    {
        dataset.erase(dataset.begin() + ind);
    }


    template <class T>
    void serialize(T& t)
    {
        t(name, dataset, id);
    }


};


