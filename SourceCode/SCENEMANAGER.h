#pragma once
#include "Engine/Singleton.h"
#include "SCENE.h"
#include <memory>
#include <d3d11.h>
#include <map>
#include <string>


class SCENEMANAGER : public SINGLETON<SCENEMANAGER>
{
    std::map<std::string, std::shared_ptr<SCENE>>scenes;
    std::shared_ptr<SCENE> cur_Scene;
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
    void ChangeScene(std::string name);
    /// <summary>
    /// Inserts an scene entity into the map
    /// </summary>
    /// <param name="name"></param>
    /// <param name="s"></param>
    void Insert(std::string name, std::shared_ptr<SCENE>s);
    /// <summary>
    /// Returns the scene mapped with the name
    /// </summary>
    /// <param name="name"></param>
    /// <returns></returns>
    std::shared_ptr<SCENE>Retrieve(std::string name);

};