#pragma once
#include "Shader.h"
class OBJECT;
class SkyboxShader : public Shader
{


	ComPtr<ID3D11Buffer>skyboxConstantBuffer;

public:
	struct CBuffer_Skybox
	{
		Math::Vector4 view_position;
		XMFLOAT4X4 inverse_view;
		XMFLOAT4X4 inverse_projection;
	};
	HRESULT Initialize() override;
	void UpdateConstantBuffers(ID3D11DeviceContext* dc, OBJECT* parent) override;
	void SetShaders(ID3D11DeviceContext* dc, OBJECT* parent) override;
	void SetConstantBuffers(ID3D11DeviceContext* dc) override;
};