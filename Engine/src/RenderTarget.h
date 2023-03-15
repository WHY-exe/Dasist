#pragma once
#include "GraphicsResource.h"
#include <array>
class RenderTarget : public GraphicsResource
{
public:
	RenderTarget(Graphics& gfx, UINT width, UINT height);
	void BindAsTexture(Graphics& gfx, UINT slot) const noexcept;
	void BindAsTarget(Graphics& gfx) const noexcept;
	void BindAsTarget(Graphics& gfx, const class DepthStencil& ds) const noexcept;
	void Clear(Graphics& gfx) const noexcept;
	void Clear(Graphics& gfx, std::array<float, 4> color) const noexcept;
	void Resize(Graphics& gfx, UINT width, UINT height) noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  m_pPSTextureView;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pTarget;
};

