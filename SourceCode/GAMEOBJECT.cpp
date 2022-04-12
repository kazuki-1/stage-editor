#include "GAMEOBJECT.h"
#include "GAMEOBJECT_2D.h"
#include "Engine/DROPMANAGER.h"
#include "Engine/Input.h"
#include "Components/Base Classes/COMPONENT_CREATOR.h"
#include "Components/Base Classes/COMPONENT.h"
#include "Components/Headers/TRANSFORM_3D.h"
#include "Components/Headers/MESH.h"
#include "Components/Headers/PLAYER_CONTROLLER.h"
std::string COMP_TYPE[] = { "Mesh", "Sphere Collider", "Capsule Collider", "OBB Collider", "Mesh Collider", "Point Light", "Spotlight", "BGM", "Environmental Audio", "Terrain Audio", "Player Controller", "Enemy Controller", "Sprite 2D"};
bool popup{};
//int selected_comp{};


#pragma region MAIN_FUNCTIONS

/*--------------------------------GAMEOBJECT Constructor----------------------------------*/

GAMEOBJECT::GAMEOBJECT(std::shared_ptr<OBJECT_DATA>d)
{
    data = d;
    //name_buffer = (char*)data->name.c_str();
    for (int a = 0; a < data->name.size(); ++a)
        name_buffer[a] = data->name[a];
}

/*--------------------------------GAMEOBJECT Initialize()----------------------------------*/

HRESULT GAMEOBJECT::Initialize()
{
    if (data->Dataset().size() <= 0)
        __InternalAddComponent(COMPONENT_TYPE::TRANSFORM_3D);
    else
    {
        for (auto& d : data->Dataset())
        {
            __InternalAddComponent(d->type, d);
        }
    }
    return S_OK;
}

/*--------------------------------GAMEOBJECT Execute()----------------------------------*/

void GAMEOBJECT::Execute()
{
    //if (DROPMANAGER::Instance()->Loaded() && GetComponent<MESH>() == nullptr)
    //{
    //    std::filesystem::path path(DROPMANAGER::Instance()->Path());

    //    if (path.extension() == ".mrs")
    //    {
    //        AddComponent(COMPONENT_TYPE::MESH);
    //        MESH* m{ GetComponent<MESH>() };
    //        m->Data()->model_path = path.string();
    //        std::string directory{ "./Data/Model/" };
    //        std::filesystem::path path(m->Data()->model_path);
    //        std::filesystem::path name(path.filename().string());
    //        std::string full_name{ name.string() };
    //        name.replace_extension("");
    //        directory += name.string() + "/" + full_name;
    //        m->Data()->model_path = directory;
    //        m->Data()->model_name = name.string();

    //        GetComponent<MESH>()->Initialize();
    //    }
    //}

    for (auto& c : components) {
        if (dynamic_cast<TRANSFORM_3D*>(c.get()))
            continue;
        c->Execute();
    }
    GetComponent<TRANSFORM_3D>()->Execute();

}

/*--------------------------------GAMEOBJECT ExecuteUI()----------------------------------*/
void GAMEOBJECT::ExecuteUI()
{
    if (DROPMANAGER::Instance()->Loaded() && GetComponent<MESH>() == nullptr)
    {
        std::filesystem::path path(DROPMANAGER::Instance()->Path());

        if (path.extension() == ".mrs")
        {
            AddComponent(COMPONENT_TYPE::MESH);
            MESH* m{ GetComponent<MESH>() };
            m->Data()->model_path = path.string();
            std::string directory{ "./Data/Model/" };
            std::filesystem::path path(m->Data()->model_path);
            std::filesystem::path name(path.filename().string());
            std::string full_name{ name.string() };
            name.replace_extension("");
            directory += name.string() + "/" + full_name;
            m->Data()->model_path = directory;
            m->Data()->model_name = name.string();

            GetComponent<MESH>()->Initialize();
        }
    }

    for (auto& c : components) {
        if (dynamic_cast<TRANSFORM_3D*>(c.get()))
            continue;
        if (dynamic_cast<PLAYER_CONTROLLER*>(c.get()))
            continue;
        c->Execute();
    }
    GetComponent<TRANSFORM_3D>()->Execute();

}


/*--------------------------------GAMEOBJECT Render()----------------------------------*/

void GAMEOBJECT::Render()
{
    for (auto& c : components)
    {
        if (dynamic_cast<TRANSFORM_3D*>(c.get()))
            continue;
        c->Render();
    }
    GetComponent<TRANSFORM_3D>()->Render();
}

/*--------------------------------GAMEOBJECT RenderUI()----------------------------------*/

void GAMEOBJECT::RenderUI()
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
                if (!ImGui::IsItemHovered() && INPUTMANAGER::Instance()->Mouse()->LButton().Triggered())
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
        if(dynamic_cast<TRANSFORM_3D*>(c.get()) != 0)
            continue;
        std::string string{ "Remove " + COMP_TYPE[(int)c->GetComponentType()] };
        if (ImGui::Button(string.c_str()))
            RemoveComponent(ind);
        ++ind;
    }
    
}

#pragma endregion
#pragma region SUB_FUNCTIONS

/*--------------------------------GAMEOBJECT Reset()----------------------------------*/

void GAMEOBJECT::Reset()
{

}

/*--------------------------------GAMEOBJECT Activate()----------------------------------*/

void GAMEOBJECT::Activate()
{
    windowActive = true;
}

/*--------------------------------GAMEOBJECT Deactivate()----------------------------------*/

void GAMEOBJECT::Deactivate()
{
    windowActive = false;
}

#pragma endregion
#pragma region COMPONENT INSERTION

/*--------------------------------GAMEOBJECT __InternalAddComponent()----------------------------------*/

void GAMEOBJECT::__InternalAddComponent(COMPONENT_TYPE t)
{
    data->dataset.push_back(COMPONENT_CREATOR::Instance()->Instantiate(t));
    components.push_back(INSTANTIATE(this, data->dataset.back()));
    components.back()->Initialize();

}
void GAMEOBJECT::__InternalAddComponent(COMPONENT_TYPE t, std::shared_ptr<COMPONENT_DATA>d)
{
    components.push_back(INSTANTIATE(this, d));
    components.back()->Initialize();

}

/*--------------------------------GAMEOBJECT AddComponent()----------------------------------*/

void GAMEOBJECT::AddComponent(COMPONENT_TYPE t)
{
    data->dataset.push_back(COMPONENT_CREATOR::Instance()->Instantiate(t));
    components.push_back(INSTANTIATE(this, data->dataset.back()));
    components.back()->Initialize();

}

/*--------------------------------GAMEOBJECT RemoveComponent()----------------------------------*/

void GAMEOBJECT::RemoveComponent(int id)
{
    components.erase(components.begin() + id);
    data->Remove(id);
}

#pragma endregion
#pragma region GETTERS

/*--------------------------------GAMEOBJECT Data()----------------------------------*/

std::shared_ptr<OBJECT_DATA>GAMEOBJECT::Data()
{
    return data;
}

/*--------------------------------GAMEOBJECT Components()----------------------------------*/

std::vector<std::shared_ptr<COMPONENT>>GAMEOBJECT::Components()
{
    return components;
}

#pragma endregion
#pragma region GAMEOBJECT MANAGER

/*--------------------------------GAMEOBJECT_MANAGER Insert()----------------------------------*/

void GAMEOBJECT_MANAGER::Insert(std::string n, std::shared_ptr<OBJECT_DATA>d)
{
    gameObjects.insert(std::make_pair(n, std::make_shared<GAMEOBJECT>(d)));
    gameObjects.find(n)->second->Initialize();

}

/*--------------------------------GAMEOBJECT_MANAGER Insert2D()----------------------------------*/

void GAMEOBJECT_MANAGER::Insert2D(std::string n, std::shared_ptr<OBJECT_DATA>d)
{
    gameObjects.insert(std::make_pair(n, std::make_shared<GAMEOBJECT_2D>(d)));
    gameObjects.find(n)->second->Initialize();

}

/*--------------------------------GAMEOBJECT_MANAGER Retrieve()----------------------------------*/

std::shared_ptr<GAMEOBJECT> GAMEOBJECT_MANAGER::Retrieve(std::string n)
{
    return gameObjects.find(n)->second;
}

/*--------------------------------GAMEOBJECT_MANAGER RetrieveName()----------------------------------*/

std::string GAMEOBJECT_MANAGER::RetrieveName(std::shared_ptr<GAMEOBJECT>go)
{
    for (auto& g : gameObjects)
        if (g.second == go)
            return g.first;
    assert("Not Found");
    return "";
}

/*--------------------------------GAMEOBJECT_MANAGER Remove()----------------------------------*/

void GAMEOBJECT_MANAGER::Remove(std::string n)
{
    gameObjects.erase(n);
    
}
void GAMEOBJECT_MANAGER::Remove(std::shared_ptr<GAMEOBJECT>gameObject)
{
    std::string target{ RetrieveName(gameObject) };
    Remove(target);
}

/*--------------------------------GAMEOBJECT_MANAGER Clear()----------------------------------*/

void GAMEOBJECT_MANAGER::Clear()
{
    gameObjects.clear();
}

/*--------------------------------GAMEOBJECT_MANAGER GameObjects()----------------------------------*/

std::map<std::string, std::shared_ptr<GAMEOBJECT>>GAMEOBJECT_MANAGER::GameObjects()
{
    return gameObjects;
}

/*--------------------------------GAMEOBJECT_MANAGER Initialize()----------------------------------*/
/// <summary>
/// Called at the start of the stage. Initializes all gameObjects and their components
/// </summary>
void GAMEOBJECT_MANAGER::Initialize()
{
    for (auto& g : gameObjects)
        g.second->Initialize();
}

/*--------------------------------GAMEOBJECT_MANAGER Execute()----------------------------------*/
/// <summary>
/// Performs Game functions. Called every frame to perform functions
/// </summary>
void GAMEOBJECT_MANAGER::Execute()
{
    for (auto& g : gameObjects)
        g.second->Execute();
}

/*--------------------------------GAMEOBJECT_MANAGER ExecuteUI()----------------------------------*/
/// <summary>
/// Performs UI functions. Called every frame to perform functions. 
/// </summary>
void GAMEOBJECT_MANAGER::ExecuteUI()
{
    for (auto& g : gameObjects)
        g.second->ExecuteUI();
}

/*--------------------------------GAMEOBJECT_MANAGER Render()----------------------------------*/
/// <summary>
/// Renders objects. Called every frame to perform rendering
/// </summary>
void GAMEOBJECT_MANAGER::Render()
{
    for (auto& g : gameObjects)
        g.second->Render();

}

/*--------------------------------GAMEOBJECT_MANAGER RenderUI()----------------------------------*/
/// <summary>
/// Renders UI. Called every frame to perform rendering
/// </summary>
void GAMEOBJECT_MANAGER::RenderUI()
{
    for (auto& g : gameObjects)
        g.second->RenderUI();

}

/*--------------------------------GAMEOBJECT_MANAGER Finalize()----------------------------------*/
/// <summary>
/// Called at the end of program to finalize the manager
/// </summary>
void GAMEOBJECT_MANAGER::Finalize()
{
    gameObjects.clear();
}

#pragma endregion
