#pragma once
#include "../Engine/Singleton.h"
#include "SCENE.h"
#include <memory>
#include <d3d11.h>
#include <map>
#include <string>

enum class ScenesEnum
{
    Scene_UI,
    Scene_Demo, 
    End
};




class SceneManager : public Singleton<SceneManager>
{
    std::map<ScenesEnum, std::shared_ptr<Scene>>scenes;
    std::shared_ptr<Scene> cur_Scene;
    ScenesEnum current_Enum;
    bool isPaused{ true };
public:
    /// <summary>
    /// Initializes the scene manager by inserting neccessary scenes into the map
    /// </summary>
    /// <param name="s"></param>
    /// <returns></returns>
    HRESULT Initialize();
    /// <summary>
    /// Called every frame to update the scene
    /// </summary>
    void Execute();
    /// <summary>
    /// Called every frame to perform rendering of the frame
    /// </summary>
    void Render();
    /// <summary>
    /// Finalizes the class, usually unneeded as smart pointers are used
    /// </summary>
    void Finalize();
    /// <summary>
    /// Changes the scenes to the one that is mapped to the name
    /// </summary>
    /// <param name="name"></param>
    void ChangeScene(ScenesEnum name);
    /// <summary>
    /// Inserts an scene entity into the map
    /// </summary>
    /// <param name="name"></param>
    /// <param name="s"></param>
    void Insert(ScenesEnum name, std::shared_ptr<Scene>s);
    /// <summary>
    /// Transition into SceneDemo
    /// </summary>
    void Play();
    /// <summary>
    /// Pauses the demo scene
    /// </summary>
    void Pause();
    /// <summary>
    /// Stops the demo scene and transition back into SceneUI
    /// </summary>
    void Stop();
    /// <summary>
    /// Returns the scene mapped with the name
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    std::shared_ptr<Scene>Retrieve(ScenesEnum name);
    /// <summary>
    /// Returns the enum of this current scene
    /// </summary>
    /// <returns></returns>
    ScenesEnum CurrentSceneEnum();
    /// <summary>
    /// Returns true if scene is paused
    /// </summary>
    /// <returns></returns>
    bool PauseState();

};