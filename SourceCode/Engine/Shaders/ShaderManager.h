#pragma once
#include "../Singleton.h"
#include <map>
#include <memory>
#include <d3d11.h>
class Shader;

enum class ShaderTypes

{

	Shader_2D,
	Base_3D, 
	Debug_3D,
	PhongShader,
	Outline,
	Skybox, 
};
class ShaderManager : public Singleton<ShaderManager>
{
	std::map<ShaderTypes, std::shared_ptr<Shader>>shaders;

public:
	HRESULT Initialize();
	std::shared_ptr<Shader>Retrieve(ShaderTypes type);



	~ShaderManager()
	{
		shaders.clear();

	}


};