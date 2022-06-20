#pragma once
#include "../../OBJECT_DATA.h"
#include "../../Engine/Singleton.h"
#include <vector>
#include <memory>




class DataManager : public Singleton<DataManager>
{
    std::vector<std::shared_ptr<OBJECT_DATA>>dataset;
    std::string file_path;


    friend class OBJECT_DATA;
public:
    /// <summary>
    /// Inserts the OBJECT_DATA into the dataset
    /// </summary>
    /// <param name="d"></param>
    void Insert(std::shared_ptr<OBJECT_DATA>d);
    /// <summary>
    /// Loads the file
    /// </summary>
    /// <param name="file_path"></param>
    void Load(std::string file_path);
    /// <summary>
    /// Use the data loaded and initializes all gameObjects
    /// </summary>
    void InsertAndInitialize();
    /// <summary>
    /// Clear the dataset
    /// </summary>
    void Finalize();
    /// <summary>
    /// Creates a stage data file
    /// </summary>
    /// <param name="output_path"></param>
    void OutputFile(std::string output_path);
    /// <summary>
    /// Removes the target from the dataset
    /// </summary>
    /// <param name="target"></param>
    void Remove(std::shared_ptr<OBJECT_DATA>target);
    /// <summary>
    /// Used when playing a scene data that isnt saved
    /// </summary>
    void CreateTemporaryData();
    /// <summary>
    /// Called at the end of the program to delete any temporary data
    /// </summary>
    void DestroyTemporaryData();
    void SetFilePath(std::string path);
    std::vector<std::shared_ptr<OBJECT_DATA>>Dataset();
    std::string GetCurrentScenePath();
};