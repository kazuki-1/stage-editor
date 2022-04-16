#include "DataManager.h"
#include "../../GAMEOBJECT.h"
#include <filesystem>
#include <fstream>

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DATAMANGER Class---------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DataManager Load()-------------------------------------------------------------------*/

void DataManager::Load(std::string n)
{
    // Check current dataset to see if empty
    // Clear if empty
    if (dataset.size() > 0)
        dataset.clear();
    if (GameObjectManager::Instance()->GetGameObjects().size() >= 0)
        GameObjectManager::Instance()->Clear();

    // Prepare file path
    std::filesystem::path path(n);
    if (!std::filesystem::exists(n))
        assert(!"No file found");

    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    cereal::BinaryInputArchive bin(ifs);
    bin(dataset);
    ifs.close();
    InsertAndInitialize();
}

/*--------------------------------------------------DataManager InsertAndInitialize()-------------------------------------------------------------------*/

void DataManager::InsertAndInitialize()
{
    GameObjectManager::Instance()->GetGameObjects().clear();
    for (auto& d : dataset)
    {
        //GameObjectManager::Instance()->Insert(d->Name(), d);
        GameObjectManager::Instance()->Insert(d->Name(), d);
    }
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

/*--------------------------------------------------DataManager Insert()-------------------------------------------------------------------*/

void DataManager::Insert(std::shared_ptr<OBJECT_DATA>d)
{
    dataset.push_back(d);
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