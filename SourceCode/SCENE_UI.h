#pragma once
#include "SCENE.h"


class SCENE_UI : public SCENE
{
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void Finalize() override;
};