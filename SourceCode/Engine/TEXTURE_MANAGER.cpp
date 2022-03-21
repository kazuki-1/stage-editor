#include "TEXTURE_MANAGER.h"
#include "DirectX11.h"

std::shared_ptr<TEXTURE>TEXTURE_MANAGER::Insert(std::wstring file_path)
{
    std::shared_ptr<TEXTURE>entity;
    if(file_path == L"")
        entity = std::make_shared<TEXTURE>(DirectX11::Instance()->Device());
    else
        entity = std::make_shared<TEXTURE>(file_path, DirectX11::Instance()->Device());
    textureMap.emplace(file_path, entity);
    return entity;
}

std::shared_ptr<TEXTURE>TEXTURE_MANAGER::Retrieve(std::wstring file_path)
{
    // Search the map for existing texture
    // �}�b�v�Ɋ����̃e�L�X�`��������
    for (auto& t : textureMap)
    {
        if (file_path == t.first)
        {
            if (t.second.expired())
            {
                textureMap.erase(file_path);
                continue;
            }
            return t.second.lock();
        }
    }

    // Generate new texture if not found
    // ������Ȃ��ꍇ�ɐV�����e�L�X�`���𐶐�
    return Insert(file_path);
}