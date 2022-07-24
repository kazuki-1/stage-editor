#include "ShaderManager.h"
#include "../RenderBuffer.h"
#include "BaseShader3D.h"
#include "DebugShader3D.h"
#include "OutlineShader.h"
#include "PhongShader.h"
#include "Shader2D.h"
#include "SkyboxShader.h"
#include "ShadowMapper.h"
#include "LineShader.h"

/*-----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------ShaderManager Class------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------ShaderManager Initialize()-----------------------------------------*/

HRESULT ShaderManager::Initialize()
{
    shaders.emplace(ShaderTypes::Skybox, std::make_shared<SkyboxShader>());
    shaders.emplace(ShaderTypes::Shader_2D, std::make_shared<Shader2D>());
    shaders.emplace(ShaderTypes::PhongShader, std::make_shared<PhongShader>());
    //shaders.emplace(ShaderTypes::Base_3D, std::make_shared<BaseShader3D>());
    shaders.emplace(ShaderTypes::Debug_3D, std::make_shared<DebugShader3D>());
    shaders.emplace(ShaderTypes::LineShader, std::make_shared<LineShader>());
    //shaders.emplace(ShaderTypes::Outline, std::make_shared<OutlineShader>());
    //shaders.emplace(ShaderTypes::ShadowMapper, std::make_shared<ShadowMapper>());

    for (auto& shader : shaders)
        shader.second->Initialize();
    return S_OK;

}

/*---------------------------------------------Shader Retrieve()--------------------------------------------------------------------*/

std::shared_ptr<Shader>ShaderManager::Retrieve(ShaderTypes type)
{
    return shaders.find(type)->second;
}

/*---------------------------------------------Shader Register()--------------------------------------------------------------------*/

void ShaderManager::Register(ShaderTypes type, OBJECT* object)
{
    Retrieve(type)->Register(object);
}

/*---------------------------------------------Shader Deregister()--------------------------------------------------------------------*/

void ShaderManager::Deregister(ShaderTypes type, OBJECT* object)
{
    Retrieve(type)->Deregister(object);
}


/*---------------------------------------------Shader Render()--------------------------------------------------------------------*/

void ShaderManager::Render()
{
    shaders.find(ShaderTypes::Skybox)->second->Render();
    for (auto& shader : shaders)
    {
        if (shader.first == ShaderTypes::ShadowMapper || shader.first == ShaderTypes::Skybox || shader.first == ShaderTypes::Debug_3D)
            continue;
        shader.second->Render();
    }
    shaders.find(ShaderTypes::Debug_3D)->second->Render();
}