#include "./Base Classes/Component.h"
#include "../ENGINE/Audio.h"
#include "./Base Classes/ComponentCreator.h"
#include "OBBCollider.h"
#include "Transform3D.h"
#include "BGMComponent.h"

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------BGM_Component Class------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------BGM_Component Constructor------------------------------------------------------*/

BGM_Component::BGM_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<BGMComponent_Data*>(data);
}

/*-------------------------------------------------------------BGM_Component Initialize()------------------------------------------------------*/

HRESULT BGM_Component::Initialize()
{
    //audio = AudioEngine::Instance()->Retrieve(data->name);
    for (auto& d : data->dataset)
    {
        AudioEngine::Instance()->Insert(d->name, d->file_path);
    }
    if (data->collider_data)
    {
        collider = std::dynamic_pointer_cast<OBBCollider_Component>(INSTANTIATE(parent, data->collider_data));
        collider->Initialize();
    }
    return S_OK;
}

/*-------------------------------------------------------------BGM_Component Execute()------------------------------------------------------*/

void BGM_Component::Execute()
{
    if (collider)
        collider->Execute(GetComponent<Transform3D_Component>()->TransformMatrix());
}

/*-------------------------------------------------------------BGM_Component Render()------------------------------------------------------*/

void BGM_Component::Render()
{
    if (collider)
        collider->Render();
}

/*-------------------------------------------------------------BGM_Component UI()------------------------------------------------------*/

void BGM_Component::UI()
{
    if (ImGui::TreeNode("BGM"))
    {
        if (ImGui::Button("Add BGM"))
        {
            data->dataset.push_back(std::dynamic_pointer_cast<AudioData>(INSTANTIATE(COMPONENT_TYPE::AUDIO)));
            
        }
        static int ind{-1};
        ImGui::ListBoxHeader("BGM List");
        
        int selected{};
        for (auto& d : data->dataset)
        {
            if (ImGui::Selectable(d->name))
            {
                ind = selected;
                break;
            }
            ++selected;
        }
        ImGui::ListBoxFooter();
        if (data->dataset.size() > 0 && ind >= 0 )
        {
            ImGui::FileBrowser* browser{ IMGUI::Instance()->FileBrowser() };
            ImGui::InputText("Audio name", data->dataset[ind]->name, 256);
            IMGUI::Instance()->InputText("Audio file path", &data->dataset[ind]->file_path);
            bool isEmpty{};
            static bool fileOpenBGM{};
            if (ImGui::Button("Insert audio"))
            {
                // Open file browser if no file path
                if (data->dataset[ind]->file_path == L"")
                {
                    browser->Open();
                    browser->SetTitle("Open audio file");
                    browser->SetTypeFilters({ ".wav", ".*" });
                    fileOpenBGM = true;
                    isEmpty = true;
                }
                if (!isEmpty)
                {
                    std::filesystem::path path(data->dataset[ind]->file_path);
                    std::filesystem::path name(path.filename());
                    std::wstring full_path = L"Data/Audio/" + name.filename().wstring();
                    AudioEngine::Instance()->Insert(data->name, full_path);
                    data->dataset[ind]->file_path = full_path;
                    audioList.push_back(AudioEngine::Instance()->Retrieve(data->name));
                    int index{};
                    name.replace_extension("");
                    for (auto& c : name.string())
                    {
                        data->dataset[ind]->name[index] = c;
                        ++index;
                    }
                }
            }
            // File browser updates
            if (fileOpenBGM)
            {
                browser->Display();
                if (browser->HasSelected())
                {
                    data->file_path = browser->GetSelected().wstring();
                    fileOpenBGM = false;
                    browser->Close();
                }
            }
        }
        if (ImGui::Button("Create Area of effect"))
        {
            data->collider_data = std::dynamic_pointer_cast<OBBCollider_Data>(INSTANTIATE(COMPONENT_TYPE::OBB_COL));
            collider = std::dynamic_pointer_cast<OBBCollider_Component>(INSTANTIATE(parent, data->collider_data));
            collider->Initialize();
        }
        if (collider)
        {
            collider->UI();
        }

        ImGui::TreePop();
    }

}

/*-------------------------------------------------------------BGM_Component GetComponentType()------------------------------------------------------*/

COMPONENT_TYPE BGM_Component::GetComponentType()
{
    return data->type;
}
