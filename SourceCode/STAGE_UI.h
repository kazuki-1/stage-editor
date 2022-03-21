#pragma once
#include "Engine/LIGHTING.h"
#include "OBJECT_DATA.h"
#include "CerealHeaders.h"


class STAGE_UI : public SINGLETON<STAGE_UI>
{
    std::vector<std::shared_ptr<OBJECT_DATA>>objectDataset;
public:
    void RenderUI();
    void Render();
    void GameObjectWindow();
    void SceneUI();
    void HierarchyUI();
    void LightingUI();
    void OutputFile(std::string file_name);
    void MouseSelect();
    void MoveToTarget();
    void RemoveGameObject(GAMEOBJECT* g);
    template <class T>
    void serialize(T& t)
    {
        t(objectDataset);
    }

};

