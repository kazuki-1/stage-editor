#include "OBJECT.h"
#include "Shaders/ShaderManager.h"

/*-----------------------------------------------------------------------------------------*/
/*---------------------------------------------OBJECT Class--------------------------------*/
/*-----------------------------------------------------------------------------------------*/
/*---------------------------------------------OBJECT RegisterShader()---------------------*/
/*-----------------------------------------------------------------------------------------*/

void OBJECT::RegisterShader(ShaderTypes type)
{
    shaders.push_back(type);
    ShaderManager::Instance()->Register(type, this);
}

/*---------------------------------------------OBJECT DeregisterShader()---------------------*/

void OBJECT::DeregisterShader(ShaderTypes type)
{
    int ind{};
    for (auto& shader : shaders)
    {
        if (type == shader)
        {
            shaders.erase(shaders.begin() + ind);
            break;
        }
        ++ind;
    }
    ShaderManager::Instance()->Deregister(type, this);
}

/*---------------------------------------------OBJECT Finalize()---------------------*/

void OBJECT::Finalize()
{
    for (auto& shader : shaders)
        DeregisterShader(shader);
}

/*---------------------------------------------OBJECT EnableRendering()---------------------*/

void OBJECT::EnableRendering()
{
    isVisible = true;
}

/*---------------------------------------------OBJECT DisableRendering()---------------------*/

void OBJECT::DisableRendering()
{
    isVisible = false;
}

/*---------------------------------------------OBJECT GetRenderState()---------------------*/

bool OBJECT::GetRenderState()
{
    return isVisible;
}

/*---------------------------------------------OBJECT CheckForShader()---------------------*/

bool OBJECT::CheckForShader(ShaderTypes type)
{
    for (auto& shader : shaders)
        if (type == shader)
            return true;
    return false;
}
