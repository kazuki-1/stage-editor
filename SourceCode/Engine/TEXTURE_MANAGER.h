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
    /// Private�֐��B�V�����e�L�X�`�����}�b�v�ɒ���
    /// </summary>
    /// <param name=""></param>
    std::shared_ptr<TEXTURE>Insert(std::wstring file_path);
public:
    /// <summary>
    /// <para> Returns a texture from the map. Generates a new one if not found </para>
    /// <para> �}�b�v����e�L�X�`����߂�B������Ȃ��ꍇ�ɐV�����e�L�X�`���𐶐�</para>
    /// </summary>
    /// <returns></returns>
    std::shared_ptr<TEXTURE> Retrieve(std::wstring file_path);
    





};