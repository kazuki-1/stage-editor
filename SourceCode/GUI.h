#pragma once
#include "Engine/IMGUI.h"
#include "Engine/Singleton.h"

class GUI : public SINGLETON<GUI>
{
    char model_name[256]{ "Input" }, animation_path[256]{ "Input" }, created_path[256]{ "Input" }, created_anim_name[256]{ "Input" }, material_name[256]{ "Input" };
    bool fbx, mrs;
    HRESULT hr{ E_FAIL };


public:

    void Initialize();
    void Execute();
    void Render();
    void Finalize();



};


