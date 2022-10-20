#include "Texture.h"
#include "GfxThrowMacro.h"
Texture::Texture(Graphics& gfx, const std::wstring& szPath, unsigned int slot)
	:
	m_slot(slot),
	m_Path(szPath)
{
	IMPORT_INFOMAN(gfx);
	Surface s(UTF8_TO_ANSI_STR(szPath));
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
	sd.SysMemPitch = s.GetWidth() * 4;
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
	GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pTexView.GetAddressOf());
}

std::shared_ptr<Bindable> Texture::Resolve(Graphics& gfx, const std::wstring& path, unsigned int slot) noexcept(!IS_DEBUG)
{
	return CodeX::Resolve<Texture>(gfx, path, slot);
}

std::wstring Texture::GenUID(const std::wstring& path, unsigned int slot) noexcept
{
	using namespace std::string_literals;
	return ANSI_TO_UTF8_STR(typeid(Texture).name() + "#"s + std::to_string(slot)) + L"#"s + path;
}

std::wstring Texture::GetUID() const noexcept
{
	return GenUID(m_Path, m_slot);
}
