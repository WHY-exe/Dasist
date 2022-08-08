#include "Texture.h"
#include "GfxThrowMacro.h"
Texture::Texture(Graphics& gfx, const Surface& s)
{
	IMPORT_INFOMAN(gfx);
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = s.GetWidth();
	texDesc.Height = s.GetHeight();
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = s.GetBufferPtr();
	sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTex;
	GFX_THROW_INFO(
		GetDevice(gfx)->CreateTexture2D(
			&texDesc, &sd, &pTex
		)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(
		GetDevice(gfx)->CreateShaderResourceView(
			pTex.Get() ,&srvDesc, &m_pTexView
		)
	);
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShaderResources(0u, 1u, m_pTexView.GetAddressOf());
}
