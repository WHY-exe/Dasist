#include "Stencil.h"
#include "BindableCodex.h"
#include "StrTransf.h"
#include <typeinfo>
Stencil::Stencil(Graphics& gfx, Mod mod)
	:
	m_mode(mod)
{
	D3D11_DEPTH_STENCIL_DESC depthDesc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT{});

	switch (mod)
	{
	case Stencil::Mod::Off:
		break;
	case Stencil::Mod::Write:
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.StencilEnable = TRUE;
		depthDesc.StencilWriteMask = 0xFF;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case Stencil::Mod::Mask:
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.StencilEnable = TRUE;
		depthDesc.StencilReadMask = 0xFF;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case Stencil::Mod::MaskAndWrite:
		depthDesc.DepthEnable = FALSE;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.StencilEnable = TRUE;
		depthDesc.StencilReadMask = 0xFF;
		depthDesc.StencilWriteMask = 0xFF;
		depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	}

	GetDevice(gfx)->CreateDepthStencilState(&depthDesc, &m_pStencilState);
}
void Stencil::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->OMSetDepthStencilState(m_pStencilState.Get(), 0xFF);
}
std::shared_ptr<Stencil> Stencil::Resolve(Graphics& gfx, Mod mode) noexcept(!IS_DEBUG)
{
	return CodeX::Resolve<Stencil>(gfx, mode);
}

std::wstring Stencil::GenUID(Mod mode) noexcept
{
	using namespace std::string_literals;
	std::string mode_str;
	switch (mode)
	{
	case Stencil::Mod::Off:
		mode_str = "Off";
		break;
	case Stencil::Mod::Write:
		mode_str = "Write";
		break;
	case Stencil::Mod::Mask:
		mode_str = "Mask";
		break;
	}
	return ANSI_TO_UTF8_STR(typeid(Stencil).name() + "#"s + mode_str);
}

std::wstring Stencil::GetUID() const noexcept
{
	return GenUID(m_mode);
}
