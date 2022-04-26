#pragma once
#include <string>
#include "SCENE.h"
class SceneDemo : public Scene
{

    bool state{ true };       // Paused or playing
public:
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void Finalize() override;

    void Pause();
    void Resume();
    /// <summary>
    /// Returns true if scene is not paused
    /// </summary>
    /// <returns></returns>
    bool GetState();
};