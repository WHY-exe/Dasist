#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include "GfxThrowMacro.h"

namespace DX = DirectX;

// link the library
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND hWnd, int nWinWidth = 0, int nWinHeight = 0)
    :
    WinWidth(nWinWidth),
    WinHeight(nWinHeight)
{
    // create a swap chain descripter
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
    // determine wether it is on debug mod
    unsigned int swapChainCreateFlags = 0u;
#ifndef NDEBUG
    swapChainCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG
    // create device and swap chain
    INIT_GFX_EXCEPTION;
    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapChainCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &m_pSwapChain,
        &m_pDevice,
        nullptr,
        &m_pContext
    ));
    Microsoft::WRL::ComPtr<ID3D11Resource> pBackResource;
    GFX_THROW_INFO(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackResource));

    GFX_THROW_INFO(m_pDevice->CreateRenderTargetView(
        pBackResource.Get(),
        nullptr,
        &m_pTarget
    ));

    // set the z-buffer
    // 1. create depth stencil state 
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
    GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&dsDesc, pDSState.GetAddressOf()));

    m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);
    // 2. create depth sencil texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = WinWidth;
    depthDesc.Height = WinHeight;
    depthDesc.MipLevels = 1u;
    depthDesc.ArraySize = 1u;
    depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthDesc.SampleDesc.Count = 1u;
    depthDesc.SampleDesc.Quality = 0u;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    GFX_THROW_INFO(m_pDevice->CreateTexture2D(&depthDesc, nullptr, &pDepthStencil));

    // 3. create view of depth stensil texture
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0u;
    GFX_THROW_INFO(m_pDevice->CreateDepthStencilView(
        pDepthStencil.Get(), &dsvDesc, &m_pDSV
    ));

    // 4. bind depth stensil view to OM
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), m_pDSV.Get());

    // configure viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = (float)WinWidth;
    vp.Height = (float)WinHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // bind the view port to the pipeline
    m_pContext->RSSetViewports(1u, &vp);
}

void Graphics::DrawIndexed(UINT index_count)
{
    m_pContext->DrawIndexed(index_count, 0u, 0u);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
    m_projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
    return m_projection;
}


void Graphics::EndFrame()
{
    INIT_GFX_EXCEPTION;
#ifndef NDEBUG
    infoManager.Set();
#endif
    if (FAILED(hr = m_pSwapChain->Present(1u, 0u)))
    {
        if (hr == DXGI_ERROR_DEVICE_REMOVED)
        {
            GFX_DEVICE_REMOVED_CALL(m_pDevice->GetDeviceRemovedReason());
        }
        else
        {
            GFX_THROW_INFO(hr);
        }
    }
}

void Graphics::ClearBuffer(float r, float g, float b, float a)
{
    const float color[4] = {r, g, b, a};
    m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
    m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

Graphics::GfxExcepion::GfxExcepion(int nLine, const char* szFile, HRESULT hr, std::vector<std::string> v_szMsg)
    :
    WinException(nLine, szFile, hr)
{
    for (const auto& info:v_szMsg)
    {
        m_info += info;
        m_info.append("\n");
    }
}

Graphics::GfxExcepion::GfxExcepion(int nLine, const char* szFile, HRESULT hr)
    :
    WinException(nLine, szFile, hr)
{
}

const char* Graphics::GfxExcepion::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code]:0x" << std::hex << GetErrorCode() << std::endl
        << "[Description]:" << TranslateErrorCode(m_ErrorCode) << std::endl
        << GetInfoString() << std::endl
#ifndef NDEBUG
        << "[DxErrInfo]:" << GetErrorInfo()
#endif // !NDEBUG        
        ;
    WhatInfoBuffer = oss.str();
    return WhatInfoBuffer.c_str();
}

const char* Graphics::GfxExcepion::GetType() const noexcept
{
    return "Direct3D Exception";
}

std::string Graphics::GfxExcepion::GetErrorInfo() const noexcept
{
    return m_info;
}

Graphics::GfxInfoOnlyException::GfxInfoOnlyException(int nLine, const char* szFile, std::vector<std::string>& v_szInfo)
    :
    Exception(nLine, szFile)
{
    for (const auto& i : v_szInfo)
    {
        m_szInfo += i;
        m_szInfo.push_back('\n');
    }
    if (!m_szInfo.empty())
    {
        m_szInfo.pop_back();
    }
}

const char* Graphics::GfxInfoOnlyException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl
        << GetInfoString();
    WhatInfoBuffer = oss.str();
    return WhatInfoBuffer.c_str();
}

const char* Graphics::GfxInfoOnlyException::GetType() const noexcept
{
    return "Graphics Info Exception";
}

std::string Graphics::GfxInfoOnlyException::GetErrorInfo() const noexcept
{
    return m_szInfo;
}

