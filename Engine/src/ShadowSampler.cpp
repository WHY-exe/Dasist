#include "ShadowSampler.h"
#include "GfxThrowMacro.h"
ShadowSampler::ShadowSampler(Graphics& gfx)
{
	IMPORT_INFOMAN(gfx);
	D3D11_SAMPLER_DESC sampDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
	sampDesc.BorderColor[0] = 1.0f;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sampDesc, &m_pSampler));
}

void ShadowSampler::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
	IMPORT_INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetSamplers(0u, 1u, m_pSampler.GetAddressOf()));
}
