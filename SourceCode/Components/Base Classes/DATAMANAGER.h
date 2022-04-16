#pragma once
#include "../../OBJECT_DATA.h"
#include "../../Engine/Singleton.h"
#include <vector>
#include <memory>




class DataManager : public Singleton<DataManager>
{
    std::vector<std::shared_ptr<OBJECT_DATA>>dataset;

public:
    void Insert(std::shared_ptr<OBJECT_DATA>d);
    void Load(std::string file_path);
    void InsertAndInitialize();
    void OutputFile(std::string output_path);
    void Remove(std::shared_ptr<OBJECT_DATA>target);
    std::vector<std::shared_ptr<OBJECT_DATA>>Dataset();
};