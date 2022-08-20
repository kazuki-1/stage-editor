#pragma once
#include <Library/Singleton.h>
#include <map>
#include <memory>
#include <d3d11.h>

class OBJECT;
class Shader;

enum class ShaderTypes
{

	Shader_2D,
	Base_3D, 
	Debug_3D,
	LineShader, 
	PhongShader,
	Outline,
	ShadowMapper, 
	Skybox, 
};
class ShaderManager : public Singleton<ShaderManager>
{
	std::map<ShaderTypes, std::shared_ptr<Shader>>shaders;

public:
	HRESULT Initialize();
	std::shared_ptr<Shader>Retrieve(ShaderTypes type);
	void Register(ShaderTypes type, OBJECT* object);
	void Deregister(ShaderTypes type, OBJECT* object);
	void Render();

	~ShaderManager()
	{
		shaders.clear();

	}


};