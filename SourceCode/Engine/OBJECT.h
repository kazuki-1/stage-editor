#pragma once
#include "Shaders/ShaderManager.h"
#include <vector>
class ShaderManager;
enum class ShaderTypes;

class OBJECT
{
    std::vector<ShaderTypes>shaders;
protected:
    bool isVisible{};
    int* indices{};
    int indexCount{}, vertexCount{};
public:
    void RegisterShader(ShaderTypes type);
    void DeregisterShader(ShaderTypes type);
    virtual void Finalize();

    // Enables rendering for the object
    void EnableRendering();

    // Disables rendering for the object
    virtual void DisableRendering();

    // Returns true if render is to be rendered
    bool GetRenderState();
};
