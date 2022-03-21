#include "ResourceManager.h"
#include "MODEL.h"


std::shared_ptr<MODEL_RESOURCES> RESOURCEMANAGER::Load(std::string model_path)
{
    for (auto& a : ResourceMap)
    {
        if (model_path == a.first)
        {
            if (a.second.expired())
            {
                ResourceMap.erase(a.first);
                return Generate(model_path);
            }
            return a.second.lock();
        }
    }
    return Generate(model_path);
}


std::shared_ptr<MODEL_RESOURCES>RESOURCEMANAGER::Generate(std::string model_path)
{
    std::shared_ptr<MODEL_RESOURCES>m = std::make_shared<MODEL_RESOURCES>(DirectX11::Instance()->Device(), model_path);
    if (!m)
        assert(!"Failed to find model from map");
    ResourceMap.insert(std::make_pair(model_path, m));
    return m ? m : nullptr;
}


HRESULT RESOURCEMANAGER::Recreate(MODEL* m, std::string new_file_name)
{
    std::filesystem::path path{ new_file_name };
    path.replace_extension(".mrs");
    m->Resource()->Recreate(path.string());
    return S_OK;
}
