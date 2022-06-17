#include "TextureManager.h"
#include "DirectX11.h"


/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------TextureManager Class----------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------TextureManager Insert()---------------------------------------------------------------*/

std::shared_ptr<Texture>TextureManager::Insert(std::wstring file_path)
{
    if (file_path == L"Default_Diffuse")
        return GenerateEmpty();
    else if (file_path == L"Default_Normal")
        return GenerateNormal();

    std::string path{};
    std::shared_ptr<Texture>entity;
    
    // ModelEditor makes sure that if there are no textures loaded, 
    // it will be defaulted to Default Diffuse or Default Normal, but just in case
    if (file_path == L"") {
        entity = std::make_shared<Texture>(DirectX11::Instance()->Device());
        textureMap.emplace(file_path, entity);
    }
    entity = std::make_shared<Texture>(file_path, DirectX11::Instance()->Device());
    textureMap.emplace(file_path, entity);



    return entity;
}

/*-------------------------------------------TextureManager GenerateEmpty()---------------------------------------------------------------*/

std::shared_ptr<Texture>TextureManager::GenerateEmpty()
{
    std::shared_ptr<Texture>entity;
    entity = std::make_shared<Texture>(0xFFFFFFFF);
    textureMap.emplace(L"Default_Diffuse", entity);
    return entity;
}

/*-------------------------------------------TextureManager GenerateNormal()---------------------------------------------------------------*/

std::shared_ptr<Texture>TextureManager::GenerateNormal()
{
    std::shared_ptr<Texture>entity;
    entity = std::make_shared<Texture>(0xFFFF7F7F);
    textureMap.emplace(L"Default_Normal", entity);
    return entity;
}

/*-------------------------------------------TextureManager Retrieve()---------------------------------------------------------------*/

std::shared_ptr<Texture>TextureManager::Retrieve(std::wstring file_path)
{
    // Returns empty texture if path is blank
    if (file_path == L"")
        return Retrieve(L"Default_Diffuse");


    // Search the map for existing texture
    // マップに既存のテキスチャを検査
    for (auto& t : textureMap)
    {
        if (file_path == t.first)
        {
            if (t.second.expired())
            {
                textureMap.erase(file_path);
                break;
            }
            return t.second.lock();
        }
    }

    // Generate new texture if not found
    // 見つからない場合に新しいテキスチャを生成
    return Insert(file_path);
}