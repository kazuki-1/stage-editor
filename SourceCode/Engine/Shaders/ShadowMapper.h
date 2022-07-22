#pragma once
#include "Shader.h"
#include "../DepthStencil.h"
#include "../RenderBuffer.h"


class DepthStencil;
class RenderBuffer;
class MODEL_RESOURCES;


class ShadowMapper : public Shader
{
	struct CBuffer_Scene
	{
		XMFLOAT4X4 view_projection{};
	};
	std::unique_ptr<DepthStencil>depthStencil;
	std::unique_ptr<RenderBuffer>renderBuffer;
	ComPtr<ID3D11Buffer>sceneConstantBuffer{};
	ComPtr<ID3D11Buffer>meshConstantBuffer{};
	D3D11_VIEWPORT viewport{};
public:
	HRESULT Initialize() override;
	void SetShaders() override;;
	void UpdateConstantBuffers(OBJECT* object) override;
	void SetConstantBuffers() override;

	void Render();

};

