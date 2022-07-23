#pragma once
#include "Shader.h"

class OutlineShader : public Shader
{
	struct CBuffer_Outline
	{
		Vector4 outline_colour;
		float outline_size;
		Vector3 placeholder{};
	};
	ComPtr<ID3D11Buffer>meshConstantBuffer;
	ComPtr<ID3D11Buffer>outlineConstantBuffer;
public:
	HRESULT Initialize() override;
	void SetShaders() override;
	void UpdateConstantBuffers(OBJECT* parent) override;
	void SetConstantBuffers() override;
	void CleanupShaders() override;
};