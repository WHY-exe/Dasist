#pragma once
#include "GraphicsResource.h"
class DepthStencil : public GraphicsResource
{
public:
	DepthStencil(Graphics& gfx, UINT width, UINT height);
	void BindAsDepthStencil(Graphics& gfx) const noexcept;
	void Resize(Graphics& gfx, int width, int height) noexcept;
	void Clear(Graphics& gfx) const noexcept;
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetView() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDSV;
};

