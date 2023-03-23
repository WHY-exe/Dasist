#pragma once
#include "Bindable.h"
#include "BufferResource.h"
class DepthStencil : public Bindable, public BufferResource
{
	friend class RenderTarget;
public:
	DepthStencil(Graphics& gfx, UINT width, UINT height);
	DepthStencil(Graphics& gfx);
	void BindAsBuffer(Graphics& gfx) noexcept(!_DEBUG) override;
	void BindAsBuffer(Graphics& gfx, BufferResource* renderTarget) noexcept(!_DEBUG) override;
	void BindAsBuffer(Graphics& gfx, class RenderTarget* renderTarget) noexcept(!_DEBUG);
	void Clear(Graphics& gfx) noexcept(!_DEBUG) override;
	const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetView() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDSV;
};

class DepthStencilAsShaderInput :public DepthStencil
{
public:
	DepthStencilAsShaderInput(Graphics& gfx, UINT width, UINT height, UINT slot);
	DepthStencilAsShaderInput(Graphics& gfx, UINT slot);
	void Bind(Graphics& gfx) noexcept(!_DEBUG) override;
	void Resize(Graphics& gfx, UINT width, UINT height) noexcept(!_DEBUG) override;
private:
	UINT m_slot;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
};

class DepthStencilAsTraget : public DepthStencil
{
public:
	DepthStencilAsTraget(Graphics& gfx, UINT width, UINT height);
	DepthStencilAsTraget(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept(!_DEBUG) override;
	void Resize(Graphics& gfx, UINT width, UINT height) noexcept(!_DEBUG) override;
};