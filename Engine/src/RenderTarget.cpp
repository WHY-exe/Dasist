#include "RenderTarget.h"
#include "GfxThrowMacro.h"
#include "DepthStencil.h"
RenderTarget::RenderTarget(Graphics& gfx, UINT width, UINT height)
{
	IMPORT_INFOMAN(gfx);
    // create texture resource
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1u;
    texDesc.ArraySize = 1u;
    texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    texDesc.SampleDesc.Count = 1u;
    texDesc.SampleDesc.Quality = 0u;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    texDesc.MiscFlags = 0;
    texDesc.CPUAccessFlags = 0;
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&texDesc, nullptr, &pTexture));
    // create shader resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D = D3D11_TEX2D_SRV{ 0 };
    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_pPSTextureView));
    // create render target view on the texture
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = texDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateRenderTargetView(pTexture.Get(), &rtvDesc, &m_pTarget)
    );

}

void RenderTarget::BindAsTexture(Graphics& gfx, UINT slot) const noexcept
{
    GetContext(gfx)->PSSetShaderResources(slot, 1u, m_pPSTextureView.GetAddressOf());
}

void RenderTarget::BindAsTarget(Graphics& gfx) const noexcept
{
    GetContext(gfx)->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);
}

void RenderTarget::BindAsTarget(Graphics& gfx, const DepthStencil& ds)
{
    GetContext(gfx)->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), ds.GetView().Get());
}
