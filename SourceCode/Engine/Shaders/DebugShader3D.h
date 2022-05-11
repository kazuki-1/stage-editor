#pragma once
#include "Shader.h"


class DebugShader3D : public Shader
{
	ComPtr<ID3D11Buffer>meshConstantBuffer;
public:
	HRESULT Initialize() override;
	void SetConstantBuffers(ID3D11DeviceContext* dc) override;
	void UpdateConstantBuffer(ID3D11DeviceContext* dc, OBJECT* parent);
};