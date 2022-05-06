#pragma once
#include "Shader.h"


class PhongShader : public Shader
{

	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	PhongShader() { Initialize(); }
	void Initialize();
	void UpdateConstantBuffer(ID3D11DeviceContext* dc, OBJECT* parent);
	void SetConstantBuffers(ID3D11DeviceContext* dc) override;
	void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent);
};