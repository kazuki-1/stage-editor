#pragma once
#include "Shader.h"


class BaseShader3D : public Shader
{
	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	BaseShader3D() {};
	HRESULT Initialize() override;
	void UpdateConstantBuffers(ID3D11DeviceContext* dc, OBJECT* parent);
	void SetConstantBuffers(ID3D11DeviceContext* dc);
};