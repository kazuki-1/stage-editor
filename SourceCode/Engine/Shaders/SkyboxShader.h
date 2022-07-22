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
	void UpdateConstantBuffers(OBJECT* parent) override;
	void SetShaders() override;
	void SetConstantBuffers() override;
	void Render() override;
};