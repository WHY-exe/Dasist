#pragma once
#include "Bindable.h"
class Stencil : public Bindable
{
public:
	enum class Mod
	{
		Off,
		Write,
		Mask
	};
public:
	Stencil(Graphics& gfx, Mod mod)
	{
		D3D11_DEPTH_STENCIL_DESC depthDesc;
		depthDesc.DepthEnable = TRUE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDesc.StencilEnable = FALSE;
		depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		D3D11_DEPTH_STENCILOP_DESC defaultStencilOp;
		defaultStencilOp.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		defaultStencilOp.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		defaultStencilOp.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		defaultStencilOp.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.FrontFace = defaultStencilOp;
		depthDesc.BackFace = defaultStencilOp;
		if (mod == Mod::Write)
		{
			depthDesc.StencilEnable = TRUE;
			depthDesc.StencilWriteMask = 0xFF;
			depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		}
		if (mod == Mod::Mask)
		{
			depthDesc.DepthEnable = FALSE;
			depthDesc.StencilEnable = TRUE;
			depthDesc.StencilReadMask = 0xFF;
			depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
			depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		}
		GetDevice(gfx)->CreateDepthStencilState(&depthDesc, &m_pStencilState);
	}
	void Bind(Graphics& gfx) noexcept override
	{
		GetContext(gfx)->OMSetDepthStencilState(m_pStencilState.Get(), 0xFF);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pStencilState;
};