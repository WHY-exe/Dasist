#include "DepthStencil.h"
#include "RenderTarget.h"
#include "GfxThrowMacro.h"
DepthStencil::DepthStencil(Graphics& gfx, UINT width, UINT height)
{
    IMPORT_INFOMAN(gfx);
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

DepthStencil::DepthStencil(Graphics& gfx)
    :
    DepthStencil(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight())
{
}

void DepthStencil::BindAsBuffer(Graphics& gfx) noexcept(!_DEBUG)
{
    GetContext(gfx)->OMSetRenderTargets(0u, nullptr, m_pDSV.Get());
}

void DepthStencil::BindAsBuffer(Graphics& gfx, BufferResource* renderTarget) noexcept(!_DEBUG)
{
    assert(dynamic_cast<RenderTarget*>(renderTarget) != nullptr);
    BindAsBuffer(gfx, static_cast<RenderTarget*>(renderTarget));
}

void DepthStencil::BindAsBuffer(Graphics& gfx, RenderTarget* renderTarget) noexcept(!_DEBUG)
{
    renderTarget->BindAsBuffer(gfx, this);
}

void DepthStencil::Clear(Graphics& gfx) noexcept(!_DEBUG)
{
    GetContext(gfx)->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DepthStencil::GetView() const noexcept
{
    return m_pDSV;
}

DepthStencilAsShaderInput::DepthStencilAsShaderInput(Graphics& gfx, UINT width, UINT height, UINT slot)
    :
    DepthStencil(gfx, width, height),
    m_slot(slot)
{
    IMPORT_INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3D11Resource> pRes;
    m_pDSV->GetResource(&pRes);
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // this will need to be fixed
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
        pRes.Get(), &srvDesc, &m_pShaderResourceView
    ));
}

DepthStencilAsShaderInput::DepthStencilAsShaderInput(Graphics& gfx, UINT slot)
    :
    DepthStencil(gfx),
    m_slot(slot)
{
}

void DepthStencilAsShaderInput::Bind(Graphics& gfx) noexcept(!_DEBUG)
{
    IMPORT_INFOMAN(gfx);
    GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pShaderResourceView.GetAddressOf()));
}

void DepthStencilAsShaderInput::Resize(Graphics& gfx, UINT width, UINT height) noexcept(!_DEBUG)
{
    *this = DepthStencilAsShaderInput(gfx, width, height, m_slot);
}

DepthStencilAsTraget::DepthStencilAsTraget(Graphics& gfx, UINT width, UINT height)
    :
    DepthStencil(gfx, width, height)
{
}

DepthStencilAsTraget::DepthStencilAsTraget(Graphics& gfx)
    :
    DepthStencil(gfx)
{
}

void DepthStencilAsTraget::Bind(Graphics& gfx) noexcept(!_DEBUG)
{
    assert("Cannot bind OuputOnlyDepthStencil as shader input" && false);
}

void DepthStencilAsTraget::Resize(Graphics& gfx, UINT width, UINT height) noexcept(!_DEBUG)
{
    *this = DepthStencilAsTraget(gfx, width, height);
}
