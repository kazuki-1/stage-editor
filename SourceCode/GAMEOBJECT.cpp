#include "GameObject.h"
#include "GAMEOBJECT_2D.h"
#include "Engine/DROPMANAGER.h"
#include "Engine/Input.h"
#include "Components/Base Classes/ComponentCreator.h"
#include "Components/Base Classes/Component.h"
#include "Components/Transform3D.h"
#include "Components/Mesh.h"
#include "Components/PlayerController.h"
#include "Components/TerrainAudio.h"
#include "Engine/WWise/WWiseController.h"
std::string COMP_TYPE[] = {
    "BGM Component",
    "Capsule Collider",
    "Enemy Controller",
    "Environmental Audio",
    "Mesh Component",
    "Mesh Collider",
    "Dialogue Component",
    "OBB Collider",
    "Player Controller",
    "Point Light",
    "Sound Obstruction", 
    "Sphere Collider",
    "Spotlight",
    "Sprite 2D",
    "Terrain Audio "
};











bool popup{};
//int selected_comp{};


#pragma region MAIN_FUNCTIONS

/*--------------------------------GameObject Constructor----------------------------------*/

GameObject::GameObject(std::shared_ptr<OBJECT_DATA>d)
{
    data = d;
    //name_buffer = (char*)data->name.c_str();
    for (int a = 0; a < data->name.size(); ++a)
        name_buffer[a] = data->name[a];
}

/*--------------------------------GameObject Initialize()----------------------------------*/

HRESULT GameObject::Initialize()
{
    if (data->Dataset().size() <= 0)
        __InternalAddComponent(COMPONENT_TYPE::Transform3D_Component);
    else
    {
        for (auto& d : data->Dataset())
        {
            __InternalAddComponent(d->type, d);
        }
    }
    return S_OK;
}

/*--------------------------------GameObject Execute()----------------------------------*/

void GameObject::Execute()
{
    //if (DROPMANAGER::Instance()->Loaded() && GetComponent<Mesh_Component>() == nullptr)
    //{
    //    std::filesystem::path path(DROPMANAGER::Instance()->Path());

    //    if (path.extension() == ".mrs")
    //    {
    //        AddComponent(COMPONENT_TYPE::Mesh_Component);
    //        Mesh_Component* m{ GetComponent<Mesh_Component>() };
    //        m->Data()->model_path = path.string();
    //        std::string directory{ "./Data/Model/" };
    //        std::filesystem::path path(m->Data()->model_path);
    //        std::filesystem::path name(path.filename().string());
    //        std::string full_name{ name.string() };
    //        name.replace_extension("");
    //        directory += name.string() + "/" + full_name;
    //        m->Data()->model_path = directory;
    //        m->Data()->model_name = name.string();

    //        GetComponent<Mesh_Component>()->Initialize();
    //    }
    //}

    for (auto& c : components) {
        if (dynamic_cast<Transform3D_Component*>(c.get()))
            continue;
        c->Execute();
    }
    GetComponent<Transform3D_Component>()->Execute();

}

/*--------------------------------GameObject ExecuteUI()----------------------------------*/
void GameObject::ExecuteUI()
{
    if (DROPMANAGER::Instance()->Loaded() && GetComponent<Mesh_Component>() == nullptr)
    {
        std::filesystem::path path(DROPMANAGER::Instance()->Path());

        if (path.extension() == ".mrs")
        {
            AddComponent(COMPONENT_TYPE::MESH);
            Mesh_Component* m{ GetComponent<Mesh_Component>() };
            m->Data()->model_path = path.string();
            std::string directory{ "./Data/Model/" };
            std::filesystem::path path(m->Data()->model_path);
            std::filesystem::path name(path.filename().string());
            std::string full_name{ name.string() };
            name.replace_extension("");
            directory += name.string() + "/" + full_name;
            m->Data()->model_path = directory;
            m->Data()->model_name = name.string();

            GetComponent<Mesh_Component>()->Initialize();
        }
    }

    for (auto& c : components) {
        if (dynamic_cast<Transform3D_Component*>(c.get()))
            continue;
        if (dynamic_cast<PlayerController_Component*>(c.get()))
            continue;
        if (dynamic_cast<TerrainAudio_Component*>(c.get()))
        {
            dynamic_cast<TerrainAudio_Component*>(c.get())->ExecuteUI();
            continue;
        }
        c->Execute();
    }
    GetComponent<Transform3D_Component>()->ExecuteUI();

}

/*--------------------------------GameObject Render()----------------------------------*/

void GameObject::Render()
{
    for (auto& c : components)
    {
        if (dynamic_cast<Transform3D_Component*>(c.get()))
            continue;
        c->Render();
    }
    GetComponent<Transform3D_Component>()->Render();
}

/*--------------------------------GameObject Finalize()----------------------------------*/
/// <summary>
/// Called when deinitializing
/// </summary>
void GameObject::Finalize()
{
    for (auto& c : components)
        c->Finalize();
}

/*--------------------------------GameObject RenderUI()----------------------------------*/

void GameObject::RenderUI()
{
    ImGui::InputText("Name", name_buffer, 256);
    data->name = name_buffer;
    if (ImGui::Button("Add Component"))
        popup = true;
    if (popup)
    {
        ImGui::OpenPopup("Add Component");
        if (ImGui::BeginPopup("Add Component"))
        {
            if (popup)
            {
                ImGui::ListBoxHeader("Components");

                int ind{};
                for (auto& c : COMP_TYPE)
                {
                    bool s{};
                    if (ImGui::Selectable(c.c_str(), &s))
                    {
                        AddComponent((COMPONENT_TYPE)ind);
                        popup = false;
                        break;
                    }
                    ++ind;
                }
                ImGui::ListBoxFooter();
                if (!ImGui::IsItemHovered() && InputManager::Instance()->Mouse()->LButton().Triggered())
                {
                    popup = false;
                }

                ImGui::EndPopup();
            }
        }
    }
    int ind{1};
    for (auto& c : components)
    {
        c->UI();
        if(dynamic_cast<Transform3D_Component*>(c.get()) != 0)
            continue;
        std::string string{ "Remove " + COMP_TYPE[(int)c->GetComponentType()] };
        if (ImGui::Button(string.c_str()))
            RemoveComponent(ind);
        ++ind;
    }
    
}

#pragma endregion
#pragma region SUB_FUNCTIONS

/*--------------------------------GameObject Reset()----------------------------------*/

void GameObject::Reset()
{

}

/*--------------------------------GameObject Activate()----------------------------------*/

void GameObject::Activate()
{
    windowActive = true;
}

/*--------------------------------GameObject Deactivate()----------------------------------*/

void GameObject::Deactivate()
{
    windowActive = false;
}

#pragma endregion
#pragma region COMPONENT INSERTION

/*--------------------------------GameObject __InternalAddComponent()----------------------------------*/

void GameObject::__InternalAddComponent(COMPONENT_TYPE t)
{
    data->dataset.push_back(ComponentCreator::Instance()->Instantiate(t));
    components.push_back(INSTANTIATE(this, data->dataset.back()));
    components.back()->Initialize();

}
void GameObject::__InternalAddComponent(COMPONENT_TYPE t, std::shared_ptr<ComponentData>d)
{
    components.push_back(INSTANTIATE(this, d));
    components.back()->Initialize();

}

/*--------------------------------GameObject AddComponent()----------------------------------*/

void GameObject::AddComponent(COMPONENT_TYPE t)
{
    data->dataset.push_back(ComponentCreator::Instance()->Instantiate(t));

    components.push_back(INSTANTIATE(this, data->dataset.back()));
    components.back()->Initialize();

}

/*--------------------------------GameObject RemoveComponent()----------------------------------*/

void GameObject::RemoveComponent(int id)
{
    components[id]->Finalize();
    components.erase(components.begin() + id);
    data->Remove(id);
}

#pragma endregion
#pragma region GETTERS

/*--------------------------------GameObject Data()----------------------------------*/

std::shared_ptr<OBJECT_DATA>GameObject::Data()
{
    return data;
}

/*--------------------------------GameObject Components()----------------------------------*/

std::vector<std::shared_ptr<Component>>GameObject::Components()
{
    return components;
}

#pragma endregion
#pragma region GAMEOBJECT MANAGER

/*--------------------------------GameObjectManager Insert()----------------------------------*/

void GameObjectManager::Insert(std::string n, std::shared_ptr<OBJECT_DATA>d)
{
    gameObjects.insert(std::make_pair(n, std::make_shared<GameObject>(d)));
    gameObjects.find(n)->second->Initialize();
    WWiseController::Instance()->RegisterGameObject(gameObjects.find(n)->second.get());

}

/*--------------------------------GameObjectManager Insert2D()----------------------------------*/

void GameObjectManager::Insert2D(std::string n, std::shared_ptr<OBJECT_DATA>d)
{
    gameObjects.insert(std::make_pair(n, std::make_shared<GameObject_2D>(d)));
    gameObjects.find(n)->second->Initialize();
    WWiseController::Instance()->RegisterGameObject(gameObjects.find(n)->second.get());


}

/*--------------------------------GameObjectManager Retrieve()----------------------------------*/

std::shared_ptr<GameObject> GameObjectManager::Retrieve(std::string n)
{
    return gameObjects.find(n)->second;
}

/*--------------------------------GameObjectManager RetrieveName()----------------------------------*/

std::string GameObjectManager::RetrieveName(std::shared_ptr<GameObject>go)
{
    for (auto& g : gameObjects)
        if (g.second == go)
            return g.first;
    assert("Not Found");
    return "";
}

/*--------------------------------GameObjectManager Remove()----------------------------------*/

void GameObjectManager::Remove(std::string n)
{
    gameObjects.erase(n);
    
}
void GameObjectManager::Remove(std::shared_ptr<GameObject>gameObject)
{
    std::string target{ RetrieveName(gameObject) };
    gameObject->Finalize();
    Remove(target);
}

/*--------------------------------GameObjectManager Clear()----------------------------------*/

void GameObjectManager::Clear()
{
    gameObjects.clear();
}

/*--------------------------------GameObjectManager GetGameObjects()----------------------------------*/

std::map<std::string, std::shared_ptr<GameObject>>GameObjectManager::GetGameObjects()
{
    return gameObjects;
}

/*--------------------------------GameObjectManager Initialize()----------------------------------*/
/// <summary>
/// Called at the start of the stage. Initializes all gameObjects and their components
/// </summary>
void GameObjectManager::Initialize()
{
    for (auto& g : gameObjects)
    {
        g.second->Initialize();
        WWiseController::Instance()->RegisterGameObject(g.second.get());
    }
}

/*--------------------------------GameObjectManager Execute()----------------------------------*/
/// <summary>
/// Performs Game functions. Called every frame to perform functions
/// </summary>
void GameObjectManager::Execute()
{
    for (auto& g : gameObjects)
        g.second->Execute();
}

/*--------------------------------GameObjectManager ExecuteUI()----------------------------------*/
/// <summary>
/// Performs UI functions. Called every frame to perform functions. 
/// </summary>
void GameObjectManager::ExecuteUI()
{
    for (auto& g : gameObjects)
        g.second->ExecuteUI();
}

/*--------------------------------GameObjectManager Render()----------------------------------*/
/// <summary>
/// Renders objects. Called every frame to perform rendering
/// </summary>
void GameObjectManager::Render()
{
    for (auto& g : gameObjects)
        g.second->Render();

}

/*--------------------------------GameObjectManager RenderUI()----------------------------------*/
/// <summary>
/// Renders UI. Called every frame to perform rendering
/// </summary>
void GameObjectManager::RenderUI()
{
    for (auto& g : gameObjects)
        g.second->RenderUI();

}

/*--------------------------------GameObjectManager Finalize()----------------------------------*/
/// <summary>
/// Called at the end of program to finalize the manager
/// </summary>
void GameObjectManager::Finalize()
{
    for (auto& g : gameObjects)
        g.second->Finalize();
    gameObjects.clear();
}

/*--------------------------------GameObjectManager GetPlayer()----------------------------------*/

std::shared_ptr<GameObject>GameObjectManager::GetPlayer()
{
    for (auto& g : gameObjects)
    {
        if (g.second->GetComponent<PlayerController_Component>() != nullptr)
            return g.second;
    }
    return nullptr;
}



#pragma endregion
