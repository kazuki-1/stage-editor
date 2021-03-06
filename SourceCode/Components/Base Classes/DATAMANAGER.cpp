#include "DataManager.h"
#include "../../GAMEOBJECT.h"
#include <assert.h>
#include <filesystem>
#include <fstream>
int cur_id{};

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DATAMANGER Class---------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DataManager Load()-------------------------------------------------------------------*/

void DataManager::Load(std::string n)
{
    // Check current dataset to see if empty
    // Clear if empty
    dataset.clear();
    GAMEOBJECTS->Finalize();

    // Prepare file path
    std::filesystem::path path(n);
    if (!std::filesystem::exists(n))
        assert(!"No file found");

    // Load file
    std::ifstream ifs(path, std::ios::in | std::ios::binary);   
    cereal::BinaryInputArchive bin(ifs);
    bin(dataset);
    ifs.close();
    //InsertAndInitialize();
}

/*--------------------------------------------------DataManager InsertAndInitialize()-------------------------------------------------------------------*/

void DataManager::InsertAndInitialize()
{
    if (file_path != "")
    {
        Finalize();
        Load(file_path);
    }

    GameObjectManager::Instance()->GetGameObjects().clear();
    for (auto& d : dataset)
    {
        GameObjectManager::Instance()->Insert(d->Name(), d);
    }
}

/*--------------------------------------------------DataManager InsertAndInitialize()-------------------------------------------------------------------*/

void DataManager::Finalize()
{
    dataset.clear();
}

/*--------------------------------------------------DataManager OutputFile()-------------------------------------------------------------------*/

void DataManager::OutputFile(std::string output_path)
{
    // Prepare file path
    std::filesystem::path path(output_path);
    path.replace_extension(".stg");

    // Replace current file if same file exists
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);

    // Create file
    std::ofstream ofs(path, std::ios::binary);
    cereal::BinaryOutputArchive boa(ofs);
    boa(dataset);
    ofs.close();
}

/*--------------------------------------------------DataManager Dataset()-------------------------------------------------------------------*/

std::vector<std::shared_ptr<OBJECT_DATA>>DataManager::Dataset()
{
    return dataset;
}

/*--------------------------------------------------DataManager GetCurrentScenePath()-------------------------------------------------------------------*/

std::string DataManager::GetCurrentScenePath()
{
    return file_path;
}


/*--------------------------------------------------DataManager Insert()-------------------------------------------------------------------*/

void DataManager::Insert(std::shared_ptr<OBJECT_DATA>d)
{
    // In case the id already exists

    for (auto& d : dataset)
    {
        if (cur_id == d->id)
            ++cur_id;
    }
    d->id = cur_id;
    d->name += "##" + std::to_string(cur_id);
    dataset.push_back(d);
    ++cur_id;
}

/*--------------------------------------------------DataManager Remove()-------------------------------------------------------------------*/

void DataManager::Remove(std::shared_ptr<OBJECT_DATA>target)
{
    int ind{};
    for (auto& d : dataset)
    {
        if (d == target)
        {
            dataset.erase(dataset.begin() + ind);
            return;
        }
        ++ind;
    }
}

/*--------------------------------------------------DataManager CreateTemporaryData()-------------------------------------------------------------------*/
/// <summary>
/// Used when playing a scene data that isnt saved
/// </summary>
void DataManager::CreateTemporaryData()
{
    // Directory path
    std::filesystem::path path(std::string("./Temporary"));
    if (!std::filesystem::exists(path))
        std::filesystem::create_directories(path);

    std::string full_path = path.string() + "/temp.stg";
    OutputFile(full_path);
    SetFilePath(full_path);


}

/*--------------------------------------------------DataManager DestroyTemporaryData()-------------------------------------------------------------------*/
/// <summary>
/// Called at the end of the program to delete any temporary data
/// </summary>
void DataManager::DestroyTemporaryData()
{
    std::filesystem::path path("./Temporary/temp.stg");
    if (std::filesystem::exists(path))
        std::filesystem::remove(path);
}

/*--------------------------------------------------DataManager SetFilePath()-------------------------------------------------------------------*/

void DataManager::SetFilePath(std::string path)
{
    file_path = path;
}