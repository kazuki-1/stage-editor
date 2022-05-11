#include "ShaderManager.h"
#include "BaseShader3D.h"
#include "DebugShader3D.h"
#include "OutlineShader.h"
#include "PhongShader.h"
#include "Shader2D.h"
#include "SkyboxShader.h"


/*-----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------ShaderManager Class------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------ShaderManager Initialize()-----------------------------------------*/

HRESULT ShaderManager::Initialize()
{
    shaders.emplace(ShaderTypes::Shader_2D, std::make_shared<Shader2D>());
    shaders.emplace(ShaderTypes::PhongShader, std::make_shared<PhongShader>());
    shaders.emplace(ShaderTypes::Base_3D, std::make_shared<BaseShader3D>());
    shaders.emplace(ShaderTypes::Debug_3D, std::make_shared<DebugShader3D>());
    shaders.emplace(ShaderTypes::Outline, std::make_shared<OutlineShader>());
    shaders.emplace(ShaderTypes::Skybox, std::make_shared<SkyboxShader>());

    for (auto& shader : shaders)
        shader.second->Initialize();
    return S_OK;

}

/*---------------------------------------------Shader Retrieve()--------------------------------------------------------------------*/

std::shared_ptr<Shader>ShaderManager::Retrieve(ShaderTypes type)
{
    return shaders.find(type)->second;
}