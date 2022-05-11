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
	void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent) override;
	void UpdateConstantBuffers(ID3D11DeviceContext* dc, OBJECT* parent) override;
	void SetConstantBuffers(ID3D11DeviceContext* dc);
};