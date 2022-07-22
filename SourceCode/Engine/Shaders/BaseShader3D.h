#pragma once
#include "Shader.h"


class BaseShader3D : public Shader
{
	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	BaseShader3D() {};
	HRESULT Initialize() override;
	void UpdateConstantBuffers(OBJECT* parent) override;
	void SetConstantBuffers() override;
};