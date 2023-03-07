#include "DepthStencil.h"
#include "GfxThrowMacro.h"
DepthStencil::DepthStencil(Graphics& gfx, UINT width, UINT height)
{
    IMPORT_INFOMAN(gfx);
    // 2. create depth sencil texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTex;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1u;
    depthDesc.SampleDesc.Quality = 0u;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthDesc, nullptr, &pDepthStencilTex));

    GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(
        pDepthStencilTex.Get(), nullptr, &m_pDSV
    ));
}

void DepthStencil::BindAsDepthStencil(Graphics& gfx) const noexcept
{
    GetContext(gfx)->OMSetRenderTargets(0u, nullptr, m_pDSV.Get());
}

void DepthStencil::Resize(Graphics& gfx, int width, int height) noexcept
{
    *this = DepthStencil(gfx, width, height);
}

void DepthStencil::Clear(Graphics& gfx) const noexcept
{
    GetContext(gfx)->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DepthStencil::GetView() const noexcept
{
    return m_pDSV;
}
