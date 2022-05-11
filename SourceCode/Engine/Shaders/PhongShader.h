#pragma once
#include "Shader.h"


class PhongShader : public Shader
{

	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	PhongShader() {}
	HRESULT Initialize() override;
	void UpdateConstantBuffers(ID3D11DeviceContext* dc, OBJECT* parent) override;
	void SetConstantBuffers(ID3D11DeviceContext* dc) override;
	//void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent) override;
};