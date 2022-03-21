#include "DATAMANAGER.h"
#include "../../GAMEOBJECT.h"
#include <filesystem>
#include <fstream>

/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DATAMANGER Class---------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------DATAMANAGER Load()-------------------------------------------------------------------*/

void DATAMANAGER::Load(std::string n)
{
    // Check current dataset to see if empty
    // Clear if empty
    if (dataset.size() > 0)
        dataset.clear();
    if (GAMEOBJECT_MANAGER::Instance()->GameObjects().size() >= 0)
        GAMEOBJECT_MANAGER::Instance()->Clear();

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

/*--------------------------------------------------DATAMANAGER InsertAndInitialize()-------------------------------------------------------------------*/

void DATAMANAGER::InsertAndInitialize()
{
    GAMEOBJECT_MANAGER::Instance()->GameObjects().clear();
    for (auto& d : dataset)
    {
        //GAMEOBJECT_MANAGER::Instance()->Insert(d->Name(), d);
        GAMEOBJECT_MANAGER::Instance()->Insert(d->Name(), d);
    }
}

/*--------------------------------------------------DATAMANAGER OutputFile()-------------------------------------------------------------------*/

void DATAMANAGER::OutputFile(std::string output_path)
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

/*--------------------------------------------------DATAMANAGER Dataset()-------------------------------------------------------------------*/

std::vector<std::shared_ptr<OBJECT_DATA>>DATAMANAGER::Dataset()
{
    return dataset;
}

/*--------------------------------------------------DATAMANAGER Insert()-------------------------------------------------------------------*/

void DATAMANAGER::Insert(std::shared_ptr<OBJECT_DATA>d)
{
    dataset.push_back(d);
}

/*--------------------------------------------------DATAMANAGER Remove()-------------------------------------------------------------------*/

void DATAMANAGER::Remove(std::shared_ptr<OBJECT_DATA>target)
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