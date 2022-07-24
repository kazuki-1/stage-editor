#pragma once
#include "Shader.h"

class DebugShader3D : public Shader
{
protected:
	struct CBuffer_Mesh
	{
		XMFLOAT4X4 world{};
		Math::Vector4 colour{};
	};
	ComPtr<ID3D11Buffer>meshConstantBuffer;

public:
	HRESULT Initialize() override;
	void SetConstantBuffers() override;
	void UpdateConstantBuffers(OBJECT* parent) override;
	void Render() override;
};