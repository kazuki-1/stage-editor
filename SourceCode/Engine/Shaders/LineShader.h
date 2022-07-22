#pragma once
#include "DebugShader3D.h"
class OBJECT;
 // This is used on objects which are rendered by LINELIST instead of the typical TRIANGLELIST
class LineShader : public DebugShader3D
{
public:
	void Render() override;
	void UpdateConstantBuffers(OBJECT* parent) override;
};