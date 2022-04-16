#pragma once
#include "Scene.h"


class SceneUI : public Scene
{
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    //void Finalize() override;
};