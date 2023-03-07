#include "Sampler.h"
#include "GfxThrowMacro.h"
#include "StrTransf.h"
#include "BindableCodex.h"
#include <typeinfo>
Sampler::Sampler(Graphics& gfx)
{
	IMPORT_INFOMAN(gfx);
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

std::shared_ptr<Bindable> Sampler::Resolve(Graphics& gfx) noexcept(!IS_DEBUG)
{
	return CodeX::Resolve<Sampler>(gfx);
}

std::wstring Sampler::GenUID() noexcept
{
	return ANSI_TO_UTF8_STR(typeid(Sampler).name());
}

std::wstring Sampler::GetUID() const noexcept
{
	return GenUID();
}
