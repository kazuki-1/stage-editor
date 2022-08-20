#pragma once
#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>
#include "../../External/ImGui/imgui.h"
#include "../../External/ImGui/imgui_impl_dx11.h"
#include "../../External/ImGui/imgui_impl_win32.h"
#include "../../External/ImGui/imfilebrowser.h"
#include <Library/Singleton.h>
#include "DROPMANAGER.h"
#include "DirectX11.h"
using namespace DirectX;

class IMGUI : public Singleton<IMGUI>
{
    ImGuiContext* imContext;
    ImGui::FileBrowser fileBrowser;
    ImGui::FileBrowser fileCreator;
    ImVec4 default_colour;
    ImVec4 highlighted_colour;
    ImVec4 colour;
public:
    // Called when initializing window handler to initialize imgui_win32
    HRESULT Initialize(HWND hwnd);

    // Called when initializing DirectX to initialize imgui_dx11
    HRESULT Initialize(DirectX11* dx);

    // Called at the start of every frame
    void Execute();

    // Called at the end of every frame
    void Render();

    // Cleans the objects created by ImGui
    void End();

    // Creates a popup based on error_string. Perform status(bool) check outside and put the pointer of it here
    void CreatePopup(std::string error_string, bool* status);

    // Drag and drop string
    void InputText(std::string label, std::string* str);

    // Drag and drop wstring
    void InputText(std::string label, std::wstring* str);

    // Display file browser, output string
    void DisplayBrowser(std::string* output_string, bool* openFlag);

    // Display file browser, output wstring
    void DisplayBrowser(std::wstring* output_string, bool* openFlag);


    ImGui::FileBrowser* FileBrowser();
    ImGui::FileBrowser* FileCreator();

};