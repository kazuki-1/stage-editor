#include "../Headers/TERRAIN_AUDIO.h"
#include "../Headers/MESH.h"
std::string types[] = { "Emitter", "Receiver" };

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TERRAIN_AUDIO_DATA Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TERRAIN_AUDIO_DATA Constructor------------------------------------------------*/

TERRAIN_AUDIO_DATA::TERRAIN_AUDIO_DATA()
{
    type = COMPONENT_TYPE::TERRAIN_AUDIO;

}

/*-----------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------TERRAIN_AUDIO Class---------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------TERRAIN_AUDIO Constructor------------------------------------------------*/

TERRAIN_AUDIO::TERRAIN_AUDIO(GAMEOBJECT* g, COMPONENT_DATA* d)
{
    data = static_cast<TERRAIN_AUDIO_DATA*>(d);
    parent = g;
}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (OBB_COLLIDER_DATA)</para>
/// <para> 生成時に呼び出す。対象のデータを使って初期化する (OBB_COLLIDER_DATA) </para>
/// </summary>
/// <returns></returns>
HRESULT TERRAIN_AUDIO::Initialize()
{
    return S_OK;
}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> 毎フレームに呼び出す </para>
/// </summary>
void TERRAIN_AUDIO::Execute()
{

}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Called after Execute() to perform any render functions </para>
/// <para> Execute()後にレンダー関数を実行するように毎フレームに呼び出す </para>
/// </summary>
void TERRAIN_AUDIO::Render()
{

}

/*----------------------------------------------TERRAIN_AUDIO Initialize()------------------------------------------------*/
/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画 </para>
/// </summary>
void TERRAIN_AUDIO::UI()
{
    if (ImGui::TreeNode("Terrain Audio"))
    {
        // Component is based on MESH component, so if not inserted, display nothing.
        MESH* mesh = GetComponent<MESH>();
        if (!mesh || !mesh->Model())
        {
            ImGui::Text("Mesh Component required!");
            ImGui::TreePop();
            return;
        }
        static int ind{ 0 };
        if (ImGui::BeginCombo("Type", types[ind].c_str()))
        {
            int cur{};
            for (auto& s : types)
            {
                if (ImGui::Selectable(s.c_str()))
                    ind = cur;
                ++cur;
            }
            ImGui::EndCombo();
        }



        if (ImGui::Button("Create Parameters") && ind >= 0)
        {
            data->class_type = (TERRAIN_AUDIO_PROPERTY)ind;
            ind == (int)TERRAIN_AUDIO_PROPERTY::EMITTER ? data->property_data = std::make_shared<TERRAIN_AUDIO_DATA_EMITTER>() : data->property_data = std::make_shared<TERRAIN_AUDIO_DATA_RECEIVER>();
        }
        if (!data->property_data)
        {
            ImGui::TreePop();
            return;
        }


        switch (data->class_type)
        {
        case TERRAIN_AUDIO_PROPERTY::EMITTER:
        {
            if (ImGui::TreeNode("Emitter Properties"))
            {

                TERRAIN_AUDIO_DATA_EMITTER* current = static_cast<TERRAIN_AUDIO_DATA_EMITTER*>(data->property_data.get());
                ImGui::InputText("Terrain name", current->name, 256);
                ImGui::Text(std::to_string(current->mesh_index).c_str());
                ImGui::InputText("Audio Name", current->audio_data->name, 256);
                IMGUI::Instance()->InputText("Audio File", &current->audio_data->file_path);

                MODEL_RESOURCES* mr{ mesh->Model()->Resource().get() };
                ImGui::ListBoxHeader("Meshes");
                static int sel{ -1 };
                int index{};
                for (auto& m : mr->Meshes)
                {
                    if (ImGui::Selectable(m.Name.c_str()))
                    {
                        sel = index;
                        break;
                    }
                    index++;
                }
                ImGui::ListBoxFooter();


                ImGui::FileBrowser* browser{ IMGUI::Instance()->FileBrowser() };
                static bool fileOpenTA{};
                bool isEmpty{};
                if (ImGui::Button("Set Parameters"))
                {
                    if (current->audio_data->file_path == L"")
                    {
                        isEmpty = true;
                        browser->Open();
                        browser->SetTitle("Open audio file");
                        browser->SetTypeFilters({ ".wav", ".*" });
                        fileOpenTA = true;
                    }
                    // TODO : Link mesh to the terrain audio
                    if (!isEmpty)
                    {
                        current->mesh_index = sel;
                        std::wstring dir{ L"./Data/Audio/" };
                        std::filesystem::path filename(current->audio_data->file_path);
                        dir += filename.filename().wstring();
                        current->audio_data->file_path = dir;
                    }
                }
                if (fileOpenTA)
                {
                    browser->Display();
                    if (browser->HasSelected())
                    {
                        current->audio_data->file_path = browser->GetSelected().wstring();
                        browser->Close();
                    }
                }
                ImGui::TreePop();
            }
            break;
        }
        case TERRAIN_AUDIO_PROPERTY::RECEIVER:
        {
            if (ImGui::TreeNode("Receiver Properties"))
            {
                MESH* mesh{ GetComponent<MESH>() };
                TERRAIN_AUDIO_DATA_RECEIVER* current = static_cast<TERRAIN_AUDIO_DATA_RECEIVER*>(data->property_data.get());
                if (ImGui::BeginCombo("Insert Bone", "Select bone"))
                {
                    for (auto m : mesh->Model()->Resource()->Meshes)
                    {
                        bool selected{};
                        for (auto& b : m.Bind_Pose.Bones)
                        {
                            if (ImGui::Selectable(b.Name.c_str()))
                            {
                                current->receiver_bones.push_back(b.Name);
                                selected = true;
                                break;
                            }
                        }
                        if (selected)
                            break;
                    }
                    ImGui::EndCombo();
                }
                ImGui::ListBoxHeader("Bones");
                for (auto& s : current->receiver_bones)
                    ImGui::Text(s.c_str());
                ImGui::ListBoxFooter();
                ImGui::TreePop();
            }
        }
        break;


        }

        ImGui::TreePop();
    }
}

/*----------------------------------------------------------TERRAIN_AUDIO GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE TERRAIN_AUDIO::GetComponentType()
{
    return data->type;
}