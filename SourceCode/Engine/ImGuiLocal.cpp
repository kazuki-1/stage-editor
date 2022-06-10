#include "IMGUI.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------IMGUI class------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------IMGUI Initialize------------------------------------------------------*/
// Called when initializing window handler to initialize imgui_win32
HRESULT IMGUI::Initialize(HWND hwnd)
{
    imContext = { ImGui::CreateContext() };
    ImGui::SetCurrentContext(imContext);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    default_colour = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
    highlighted_colour = { default_colour.x * 1.1f, default_colour.w * 1.1f , default_colour.x * 1.1f , default_colour.w };
    colour = default_colour;
    fileCreator = ImGui::FileBrowser(ImGuiFileBrowserFlags_CreateNewDir | ImGuiFileBrowserFlags_EnterNewFilename);
    return ImGui_ImplWin32_Init(hwnd) ? S_OK : E_FAIL;
}

// Called when initializing DirectX to initialize imgui_dx11
HRESULT IMGUI::Initialize(DirectX11* dx)
{
    return ImGui_ImplDX11_Init(dx->Device(), dx->DeviceContext()) ? S_OK : E_FAIL;
}

/*------------------------------------------IMGUI Execute()------------------------------------------------------*/
// Called at the start of every frame
void IMGUI::Execute()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

/*------------------------------------------IMGUI Render()------------------------------------------------------*/
// Called at the end of every frame
void IMGUI::Render()
{
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (!(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable))
        return;
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

/*------------------------------------------IMGUI End()------------------------------------------------------*/
// Cleans the objects created by ImGui
void IMGUI::End()
{
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext(imContext);
};

/*------------------------------------------IMGUI CreatePopup()------------------------------------------------------*/
// Creates a popup based on error_string. Perform status(bool) check outside and put the pointer of it here
void IMGUI::CreatePopup(std::string error_string, bool* status)
{
    if (!*status)
        return;
    ImGui::OpenPopup("Error");

    if (ImGui::BeginPopupModal("Error")) {
        ImGui::Text(error_string.c_str());
        if (ImGui::Button("Ok")) {
            ImGui::CloseCurrentPopup();

            *status = false;
        }
        ImGui::EndPopup();
    }

}

/*------------------------------------------IMGUI InputText()------------------------------------------------------*/
// Drag and drop string
void IMGUI::InputText(std::string label, std::string* str)
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, default_colour);
    ImGui::InputText(label.c_str(), (char*)str->c_str(), 256);
    if (ImGui::IsItemHovered())
    {
        //colour = highlighted_colour;
        ImVec2 min, max;
        min = ImGui::GetItemRectMin();
        max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(0, 255, 255, 100));
        if (DROPMANAGER::Instance()->Loaded())
            *str = DROPMANAGER::Instance()->Path();
    }
    else
        colour = default_colour;
    ImGui::PopStyleColor();
}

// Drag and drop wstring
void IMGUI::InputText(std::string label, std::wstring* str)
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, default_colour);
    ImGui::InputText(label.c_str(), (char*)str->c_str(), 256);
    if (ImGui::IsItemHovered())
    {
        //colour = highlighted_colour;
        ImVec2 min, max;
        min = ImGui::GetItemRectMin();
        max = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(0, 255, 255, 100));
        if (DROPMANAGER::Instance()->Loaded())
            *str = DROPMANAGER::Instance()->WPath();
    }
    else
        colour = default_colour;
    ImGui::PopStyleColor();
}

/*------------------------------------------IMGUI DisplayBrowser()------------------------------------------------------*/
// Display file browser, output string
void IMGUI::DisplayBrowser(std::string* output_string, bool* openFlag)
{
    if (openFlag)
    {
        fileBrowser.Display();
        if (fileBrowser.HasSelected())
        {
            *output_string = fileBrowser.GetSelected().string();
            fileBrowser.Close();
            *openFlag = false;
        }
    }
}

void IMGUI::DisplayBrowser(std::wstring* output_string, bool* openFlag)
{
    if (openFlag)
    {
        fileBrowser.Display();
        if (fileBrowser.HasSelected())
        {
            *output_string = fileBrowser.GetSelected().wstring();
            fileBrowser.Close();
            *openFlag = false;
        }
    }
}

/*------------------------------------------IMGUI FileBrowser()------------------------------------------------------*/

ImGui::FileBrowser* IMGUI::FileBrowser()
{
    return &fileBrowser;
}

/*------------------------------------------IMGUI FileCreator()------------------------------------------------------*/

ImGui::FileBrowser* IMGUI::FileCreator()
{
    return &fileCreator;
}
