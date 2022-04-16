#include "./Base Classes/Component.h"
#include "../ENGINE/Audio.h"
#include "./Base Classes/ComponentCreator.h"
#include "OBB_COLLIDER.h"
#include "TRANSFORM_3D.h"
#include "BGMComponent.h"

/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------BGMComponent Class------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------BGMComponent Constructor------------------------------------------------------*/

BGMComponent::BGMComponent(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<BGMComponent_Data*>(data);
}

/*-------------------------------------------------------------BGMComponent Initialize()------------------------------------------------------*/

HRESULT BGMComponent::Initialize()
{
    //audio = AUDIOENGINE::Instance()->Retrieve(data->name);
    for (auto& d : data->dataset)
    {
        AUDIOENGINE::Instance()->Insert(d->name, d->file_path);
    }
    if (data->collider_data)
    {
        collider = std::dynamic_pointer_cast<OBB_COLLIDER>(INSTANTIATE(parent, data->collider_data));
        collider->Initialize();
    }
    return S_OK;
}

/*-------------------------------------------------------------BGMComponent Execute()------------------------------------------------------*/

void BGMComponent::Execute()
{
    if (collider)
        collider->Execute(GetComponent<TRANSFORM_3D>()->TransformMatrix());
}

/*-------------------------------------------------------------BGMComponent Render()------------------------------------------------------*/

void BGMComponent::Render()
{
    if (collider)
        collider->Render();
}

/*-------------------------------------------------------------BGMComponent UI()------------------------------------------------------*/

void BGMComponent::UI()
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
                    AUDIOENGINE::Instance()->Insert(data->name, full_path);
                    data->dataset[ind]->file_path = full_path;
                    audioList.push_back(AUDIOENGINE::Instance()->Retrieve(data->name));
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
            data->collider_data = std::dynamic_pointer_cast<OBB_COLLIDER_DATA>(INSTANTIATE(COMPONENT_TYPE::OBB_COL));
            collider = std::dynamic_pointer_cast<OBB_COLLIDER>(INSTANTIATE(parent, data->collider_data));
            collider->Initialize();
        }
        if (collider)
        {
            collider->UI();
        }

        ImGui::TreePop();
    }

}

/*-------------------------------------------------------------BGMComponent GetComponentType()------------------------------------------------------*/

COMPONENT_TYPE BGMComponent::GetComponentType()
{
    return data->type;
}
