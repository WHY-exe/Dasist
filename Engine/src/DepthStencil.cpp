#include "DepthStencil.h"
#include "RenderTarget.h"
#include "GfxThrowMacro.h"
#include <stdexcept>
DXGI_FORMAT MapUsageTypeless(DepthStencil::Usage usage)
{
    switch (usage)
    {
    case DepthStencil::Usage::DepthStencil:
        return DXGI_FORMAT_R24G8_TYPELESS;
        break;
    case DepthStencil::Usage::DepthForShadow:
        return DXGI_FORMAT_R32_TYPELESS;
        break;
    }
    throw std::runtime_error("Base usage for Typeless format map in DepthStencil.");
    return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT MapUsageTyped(DepthStencil::Usage usage)
{
    switch (usage)
    {
    case DepthStencil::Usage::DepthStencil:
        return DXGI_FORMAT_D24_UNORM_S8_UINT;
        break;
    case DepthStencil::Usage::DepthForShadow:
        return DXGI_FORMAT_D32_FLOAT;
        break;
    }
    throw std::runtime_error("Base usage for Typed format map in DepthStencil.");
    return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT MapUsageColored(DepthStencil::Usage usage)
{
    switch (usage)
    {
    case DepthStencil::Usage::DepthStencil:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        break;
    case DepthStencil::Usage::DepthForShadow:
        return DXGI_FORMAT_R32_FLOAT;
        break;
    }
    throw std::runtime_error("Base usage for Typed format map in DepthStencil.");
    return DXGI_FORMAT_UNKNOWN;
}
DepthStencil::DepthStencil(Graphics& gfx, bool bindShaderResource, UINT width, UINT height, Usage usage)
    :
    m_width(width),
    m_height(height)
{
    IMPORT_INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencilTex;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = MapUsageTypeless(usage);
    depthDesc.SampleDesc.Count = 1u;
    depthDesc.SampleDesc.Quality = 0u;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | (bindShaderResource ? D3D11_BIND_SHADER_RESOURCE : 0);
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&depthDesc, nullptr, &pDepthStencilTex));
    D3D11_DEPTH_STENCIL_VIEW_DESC descView = {};
    descView.Format = MapUsageTyped(usage);
    descView.Flags = 0;
    descView.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descView.Texture2D.MipSlice = 0;
    GFX_THROW_INFO(GetDevice(gfx)->CreateDepthStencilView(
        pDepthStencilTex.Get(), &descView, &m_pDSV
    ));
}

DepthStencil::DepthStencil(Graphics& gfx, bool bindShaderResource, Usage usage)
    :
    DepthStencil(gfx, bindShaderResource, gfx.GetWindowWidth(), gfx.GetWindowHeight(), usage)
{
}

void DepthStencil::BindAsBuffer(Graphics& gfx) noexcept(!IS_DEBUG)
{
    GetContext(gfx)->OMSetRenderTargets(0u, nullptr, m_pDSV.Get());
}

void DepthStencil::BindAsBuffer(Graphics& gfx, BufferResource* renderTarget) noexcept(!IS_DEBUG)
{
    assert(dynamic_cast<RenderTarget*>(renderTarget) != nullptr);
    BindAsBuffer(gfx, static_cast<RenderTarget*>(renderTarget));
}

void DepthStencil::BindAsBuffer(Graphics& gfx, RenderTarget* renderTarget) noexcept(!IS_DEBUG)
{
    renderTarget->BindAsBuffer(gfx, this);
}

void DepthStencil::Clear(Graphics& gfx) noexcept(!IS_DEBUG)
{
    GetContext(gfx)->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

UINT DepthStencil::GetWidth() const noexcept
{
    return m_width;
}

UINT DepthStencil::GetHeight() const noexcept
{
    return m_height;
}

const Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& DepthStencil::GetView() const noexcept
{
    return m_pDSV;
}

DepthStencilAsShaderInput::DepthStencilAsShaderInput(Graphics& gfx, UINT width, UINT height, UINT slot, Usage usage)
    :
    DepthStencil(gfx, true, width, height, usage),
    m_slot(slot)
{
    IMPORT_INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3D11Resource> pRes;
    m_pDSV->GetResource(&pRes);
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = MapUsageColored(usage); // this will need to be fixed
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
        pRes.Get(), &srvDesc, &m_pShaderResourceView
    ));
}

DepthStencilAsShaderInput::DepthStencilAsShaderInput(Graphics& gfx, UINT slot, Usage usage)
    :
    DepthStencilAsShaderInput(gfx, gfx.GetWindowWidth(), gfx.GetWindowHeight(), slot, usage)
{
}

void DepthStencilAsShaderInput::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
    IMPORT_INFOMAN(gfx);
    GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShaderResources(m_slot, 1u, m_pShaderResourceView.GetAddressOf()));
}

void DepthStencilAsShaderInput::Resize(Graphics& gfx, UINT width, UINT height) noexcept(!IS_DEBUG)
{
    *this = DepthStencilAsShaderInput(gfx, width, height, m_slot);
}

DepthStencilAsTraget::DepthStencilAsTraget(Graphics& gfx, UINT width, UINT height)
    :
    DepthStencil(gfx, false, width, height, Usage::DepthStencil)
{
}

DepthStencilAsTraget::DepthStencilAsTraget(Graphics& gfx)
    :
    DepthStencil(gfx,false, Usage::DepthStencil)
{
}

void DepthStencilAsTraget::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
    assert("Cannot bind OuputOnlyDepthStencil as shader input" && false);
}

void DepthStencilAsTraget::Resize(Graphics& gfx, UINT width, UINT height) noexcept(!IS_DEBUG)
{
    *this = DepthStencilAsTraget(gfx, width, height);
}
