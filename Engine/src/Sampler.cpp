#include "Sampler.h"
#include "GfxThrowMacro.h"
#include "StrTransf.h"
#include "BindableCodex.h"
#include <typeinfo>
Sampler::Sampler(Graphics& gfx, bool anisoEnable, bool reflect)
	:
	m_anisoEnable(anisoEnable),
	m_reflect(reflect)
{
	IMPORT_INFOMAN(gfx);
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = anisoEnable ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR :D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR :D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR :D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sampDesc.MinLOD = 0.0f;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sampDesc, &m_pSampler));

}

void Sampler::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetSamplers(0u, 1u, m_pSampler.GetAddressOf());
}

std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx, bool anisoEnable, bool reflect) noexcept(!IS_DEBUG)
{
	return CodeX::Resolve<Sampler>(gfx, anisoEnable, reflect);
}

std::wstring Sampler::GenUID(bool anisoEnable, bool reflect) noexcept
{
	using namespace std::string_literals;
	return ANSI_TO_UTF8_STR(typeid(Sampler).name() + "#"s + (anisoEnable ? "A"s : "a"s) + (reflect ? "R"s : "W"s));
}

std::wstring Sampler::GetUID() const noexcept
{
	return GenUID(m_anisoEnable, m_reflect);
}
