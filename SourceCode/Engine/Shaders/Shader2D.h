#pragma once
#include "Shader.h"

class Shader2D : public Shader
{

public:

	HRESULT Initialize() override;
	void Render() override;
};