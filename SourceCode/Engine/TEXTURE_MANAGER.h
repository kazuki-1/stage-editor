#pragma once
#include "Singleton.h"
#include "Texture.h"
#include <map>
#include <memory>
#include <string>

class TEXTURE_MANAGER : public SINGLETON<TEXTURE_MANAGER>
{
    std::map<std::wstring, std::weak_ptr<TEXTURE>>textureMap;
    /// <summary>
    /// Private Function, inserts a new texture into the map
    /// Private関数。新しいテキスチャをマップに注入
    /// </summary>
    /// <param name=""></param>
    std::shared_ptr<TEXTURE>Insert(std::wstring file_path);
public:
    /// <summary>
    /// <para> Returns a texture from the map. Generates a new one if not found </para>
    /// <para> マップからテキスチャを戻る。見つからない場合に新しいテキスチャを生成</para>
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<TEXTURE> Retrieve(std::wstring file_path);
    





};