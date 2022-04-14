#include "STAGE_UI.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/DROPMANAGER.h"
#include "Engine/IMGUI.h"
#include "Engine/DEBUG_MANAGER.h"
#include "GAMEOBJECT.h"
#include "Components/Base Classes/DATAMANAGER.h"
#include "Components/TRANSFORM_3D.h"
#include "Components/MESH.h"
#include "Scenes/SCENEMANAGER.h"
#include "Engine/Text.h"
static int cur{};
std::shared_ptr<GAMEOBJECT>selected_model;
std::shared_ptr<GAMEOBJECT>selected_item;
std::string load_file_path;
std::string output;
char input_test[256] = { "" };
std::string gameObjectTypes[] = { "GameObject", "GameObject2D" };
std::shared_ptr<SPRITE>sprite;



/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------STAGE_UI Class--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------STAGE_UI RenderUI()--------------------------------------------------------------------*/

void STAGE_UI::RenderUI()
{
    static bool popupCheck{ false };
    static int id{ 1 };
    static std::string input_name{};

    // GameObject creation
    const std::string ID{ "##" + std::to_string(id) };
    ImGui::Begin("Create GameObject");
    ImGui::InputText("Name", input_test, 256);
    std::string empty{ "" };
    if (ImGui::Button("Create GameObject"))
    {
        popupCheck = true;
    }
    if (popupCheck)
    {
        ImGui::OpenPopup("Create GameObject");
        if (ImGui::BeginPopup("Create GameObject"))
        {
            ImGui::ListBoxHeader("GameObject Type");
            std::string path{ input_name + ID };
            std::string full = input_test;
            full += ID;
            int ind{};
            // Create GameObject or GameObject 2D
            for (auto& s : gameObjectTypes)
            {
                if (ImGui::Selectable(s.c_str()))
                {
                    DATAMANAGER::Instance()->Insert(std::make_shared<OBJECT_DATA>(full));
                    if (ind)
                    {
                        GAMEOBJECT_MANAGER::Instance()->Insert2D(DATAMANAGER::Instance()->Dataset().back()->Name(), DATAMANAGER::Instance()->Dataset().back());
                    }
                    else
                        GAMEOBJECT_MANAGER::Instance()->Insert(DATAMANAGER::Instance()->Dataset().back()->Name(), DATAMANAGER::Instance()->Dataset().back());
                    popupCheck = false;
                    ++id;
                    break;
                }
                ++ind;
            }
            ImGui::ListBoxFooter();
            ImGui::EndPopup();

        }
    }
    // Reset Camera to object, does not work with GameObject 2D
    if (ImGui::Button("Reset Camera"))
    {
        if (!selected_item)
            Camera::Instance()->ResetToTarget({ 0, 0, 0 });
        else
            Camera::Instance()->ResetToTarget(selected_item->GetComponent<TRANSFORM_3D>()->Translation());
    }





    static std::shared_ptr<GAMEOBJECT>last_item = selected_item;
    // Resets static variables
    if (last_item != selected_item)
    {
        selected_item->Reset();
        last_item = selected_item;
    }




    ImGui::End();
    GameObjectWindow();
    HierarchyUI();
    SceneUI();
    PlayUI();
}

/*--------------------------------------------------------STAGE_UI GameObjectWindow()--------------------------------------------------------------------*/

void STAGE_UI::GameObjectWindow()
{
    // Render individual gameObject windows
    if (!selected_item)
        return;
    DEBUG_MANAGER::Instance()->SetTarget(selected_item.get());

    ImGui::Begin("Details");
    ImGui::SetWindowSize({ 400, 900 });
    selected_item->RenderUI();
    ImGui::End();
}

/*--------------------------------------------------------STAGE_UI SceneUI()--------------------------------------------------------------------*/
bool text_test{};
void STAGE_UI::SceneUI()
{
    // Data saving and loading
    if (ImGui::Begin("Scene Settings"))
    {
        // ImGui file browsers
        static bool fileLoad{}, fileSave{};
        ImGui::FileBrowser* creator, * browser;
        creator = IMGUI::Instance()->FileCreator();
        browser = IMGUI::Instance()->FileBrowser();

        // File loading
        if (ImGui::Button("Load file"))
        {
            browser->SetTitle("Open file");
            browser->SetTypeFilters({ ".stg" , ".*"});
            browser->Open();
            fileLoad = true;
        }
        browser->Display();
        if (fileLoad)
        {
            if (browser->HasSelected())
            {
                std::string file_path = browser->GetSelected().string();
                DATAMANAGER::Instance()->Load(file_path);
                browser->Close();
            }
        }
        // File saving
        if (ImGui::Button("Save scene as"))
        {
            creator->Open();
            creator->SetTitle("Save scene as");
            fileSave = true;
        }
        if (fileSave)
        {
            creator->Display();
            if (creator->HasSelected())
            {
                std::string file_path = creator->GetSelected().string();
                DATAMANAGER::Instance()->OutputFile(file_path);
                creator->Close();
            }
        }


        if (ImGui::Button("Create text"))
            text_test = text_test ? false : true;
        ImGui::End();
    }

    if (!selected_item)
        DEBUG_MANAGER::Instance()->ClearTarget();

}

/*--------------------------------------------------------STAGE_UI HierarchyUI()--------------------------------------------------------------------*/

void STAGE_UI::HierarchyUI()
{
    // UI containing the gameObjects
    if (ImGui::Begin("GameObjects"))
    {
        if (GAMEOBJECT_MANAGER::Instance()->GameObjects().size() > 0)
        {
            ImGui::ListBoxHeader("GameObjects", { 250, 600 });

            static std::string selected{ GAMEOBJECT_MANAGER::Instance()->GameObjects().begin()->second->Data()->Name() };

            for (auto& o : GAMEOBJECT_MANAGER::Instance()->GameObjects())
            {
                bool select{ selected == GAMEOBJECT_MANAGER::Instance()->GameObjects().begin()->second->Data()->Name() };
                if (selected_item == o.second)
                    o.second->Activate();
                else
                    o.second->Deactivate();
                if (ImGui::Selectable(o.second->Data()->Name().c_str(), &select))
                    selected_item = o.second;

            }
            ImGui::ListBoxFooter();
            MouseSelect();
            if ((ImGui::Button("Remove Item") || INPUTMANAGER::Instance()->Keyboard()->Triggered(VK_DELETE)) && selected_item)
            {
                std::shared_ptr<OBJECT_DATA>cur_Data = selected_item->Data();
                GAMEOBJECT_MANAGER::Instance()->Remove(selected_item);
                DATAMANAGER::Instance()->Remove(cur_Data);
                selected_item = {};
            }
        }



        ImGui::End();
    }
}

/*--------------------------------------------------------STAGE_UI Render()--------------------------------------------------------------------*/

void STAGE_UI::Render()
{
    // Rendering everything in the scene
    for (auto& o : GAMEOBJECT_MANAGER::Instance()->GameObjects())
    {
        o.second->ExecuteUI();
        //o.second->RenderUI();
        o.second->Render();
    }

    if(text_test)
        TextManager::Instance()->PrintOutSequence("The monkey throws the apple", { 0, 0 }, { 1, 1 });

    //sprite->Render({}, { 1.0f, 1.0f }, {}, { 1920, 961 });
    DEBUG_MANAGER::Instance()->Execute();
    DEBUG_MANAGER::Instance()->Render();

}

/*--------------------------------------------------------STAGE_UI OutputFile()--------------------------------------------------------------------*/

void STAGE_UI::OutputFile(std::string file_name)
{
    // File saving
    std::filesystem::path path(file_name.c_str());
    path.replace_extension(".stg");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);
    std::ofstream ofs(path.string(), std::ios::out | std::ios::binary);
    cereal::BinaryOutputArchive out(ofs);
    out(objectDataset);

}

/*--------------------------------------------------------STAGE_UI MouseSelect()--------------------------------------------------------------------*/

void STAGE_UI::MouseSelect()
{
    // Perform ray casting on the mouse and select the object 
    if (DATAMANAGER::Instance()->Dataset().size() < 1)
        return;
    if (!INPUTMANAGER::Instance()->Keyboard()->Held(VK_CONTROL))
    {
        return;
    }
    Vector3 cur_pos;
    cur_pos.x = INPUTMANAGER::Instance()->Mouse()->fPosition().x;
    cur_pos.y = INPUTMANAGER::Instance()->Mouse()->fPosition().y;
    D3D11_VIEWPORT vp;
    UINT num{ 1 };
    DirectX11::Instance()->DeviceContext()->RSGetViewports(&num, &vp);
    XMVECTOR Near, Far;
    float min_dist{ FLT_MAX };
    for (auto& o : GAMEOBJECT_MANAGER::Instance()->GameObjects())
    {
        if (!o.second->GetComponent<MESH>())
            continue;
        cur_pos.z = 0;
        Near = XMVector3Unproject(cur_pos.XMV(), vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth, DirectX11::Instance()->ProjectionMatrix(), Camera::Instance()->ViewMatrix(), o.second->GetComponent<TRANSFORM_3D>()->TransformMatrix());
        cur_pos.z = 1.0f;
        Far = XMVector3Unproject(cur_pos.XMV(), vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth, DirectX11::Instance()->ProjectionMatrix(), Camera::Instance()->ViewMatrix(), o.second->GetComponent<TRANSFORM_3D>()->TransformMatrix());

        Vector3 N, F;
        N.Load(Near);
        F.Load(Far);



        COLLIDERS::RAYCASTDATA rcd{};
        Vector3 cam{ Camera::Instance()->EyePosition() }, tar{ o.second->GetComponent<TRANSFORM_3D>()->Translation() };
        Vector3 dist = tar - cam;
        float f_dist{ dist.Length() };


        if (INPUTMANAGER::Instance()->Mouse()->LButton().Triggered())
        {
            if (COLLIDERS::RayCast(N, F, o.second->GetComponent<MESH>()->Model().get(), rcd))
            {
                if (f_dist > min_dist)
                    continue;
                min_dist = f_dist;
                selected_model = o.second;
                break;
            }
            else
            {
                selected_model = {};
            }
        }
    }
    for (auto& o : GAMEOBJECT_MANAGER::Instance()->GameObjects())
    {
        if (selected_model == o.second)
        {
            selected_item = selected_model;
        }
    }
}

/*--------------------------------------------------------STAGE_UI MoveToTarget()--------------------------------------------------------------------*/

void STAGE_UI::MoveToTarget()
{




}

/*--------------------------------------------------------STAGE_UI PlayUI()--------------------------------------------------------------------*/

void STAGE_UI::PlayUI()
{
    ImGui::Begin("Control");
    if (ImGui::Button("Play"))
    {
        SCENEMANAGER::Instance()->Play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        SCENEMANAGER::Instance()->Pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        SCENEMANAGER::Instance()->Stop();
    }
    ImGui::End();

}

/*--------------------------------------------------------STAGE_UI RemoveGameObject()--------------------------------------------------------------------*/

void STAGE_UI::RemoveGameObject(GAMEOBJECT* g)
{
}
