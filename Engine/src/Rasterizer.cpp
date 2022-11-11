#include "Rasterizer.h"
#include "BindableCodex.h"
#include "GfxThrowMacro.h"
#include <typeinfo>

Rasterizer::Rasterizer(Graphics& gfx, bool twoside)
	:
	m_TwoSide(twoside)
{
	IMPORT_INFOMAN(gfx);
	D3D11_RASTERIZER_DESC raDesc = {};
	raDesc.FillMode = D3D11_FILL_SOLID;
	raDesc.CullMode = D3D11_CULL_BACK;
	if (m_TwoSide)
	{
		raDesc.CullMode = D3D11_CULL_NONE;
	}
	raDesc.FrontCounterClockwise = FALSE;
	raDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
	raDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
	raDesc.DepthClipEnable = TRUE;
	raDesc.ScissorEnable = FALSE;
	raDesc.MultisampleEnable = FALSE;
	raDesc.AntialiasedLineEnable = FALSE;

	GFX_THROW_INFO(GetDevice(gfx)->CreateRasterizerState(&raDesc, &m_pRasterizerState));
}

void Rasterizer::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->RSSetState(m_pRasterizerState.Get());
}

std::shared_ptr<Rasterizer> Rasterizer::Resolve(Graphics& gfx, bool twoside)
{
	return CodeX::Resolve<Rasterizer>(gfx, twoside);
}

std::wstring Rasterizer::GetUID() noexcept
{
	return GenUID(m_TwoSide);
}

std::wstring Rasterizer::GenUID(bool twoside) noexcept
{
	return ANSI_TO_UTF8_STR(typeid(Rasterizer).name() + '#' + std::to_string(twoside));
}
