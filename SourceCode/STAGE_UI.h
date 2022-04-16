#pragma once
#include "Engine/LIGHTING.h"
#include "OBJECT_DATA.h"
#include "CerealHeaders.h"


class STAGE_UI : public Singleton<STAGE_UI>
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
    void PlayUI();
    void RemoveGameObject(GameObject* g);
    template <class T>
    void serialize(T& t)
    {
        t(objectDataset);
    }

};

