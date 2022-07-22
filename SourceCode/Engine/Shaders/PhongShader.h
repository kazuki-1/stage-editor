#pragma once
#include "Shader.h"


class PhongShader : public Shader
{

	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	PhongShader() {}
	HRESULT Initialize() override;
	void UpdateConstantBuffers(OBJECT* parent) override;
	void SetConstantBuffers() override;
	//void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent) override;
};