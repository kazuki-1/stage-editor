#pragma once
#include "Singleton.h"
#include <string>
#include <fstream>
#include <memory>
#include "DirectX11.h"
#include <map>
#include "MODEL_RESOURCE.h"
class MODEL;

class RESOURCEMANAGER : public SINGLETON<RESOURCEMANAGER>
{
public:
    std::map<std::string, std::weak_ptr<MODEL_RESOURCES>>ResourceMap;
    /// <summary>
    /// Resource Loader. Loads the resource from the existing map (Identified by file path) or creates and inserts a new Model Resource into the ResourceMap
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<MODEL_RESOURCES>Load(std::string model_path);

    std::shared_ptr<MODEL_RESOURCES>Generate(std::string model_path);
    /// <summary>
    /// Recreates the Model file
    /// </summary>
    /// <param name="m"> : Model to recreate</param>
    /// <param name="new_file_name"> : New File name</param>
    /// <returns></returns>
    HRESULT Recreate(MODEL* m, std::string new_file_name);

    void Finalize()
    {
    }

    ~RESOURCEMANAGER()
    {
    }

};