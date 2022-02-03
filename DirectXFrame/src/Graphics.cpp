#include "Graphics.h"
#include <sstream>
// link the library
#pragma comment(lib, "d3d11.lib")

#ifndef NDEBUG
#define GFX_THORW_CALL(hrcall)\
	if(FAILED(hr = hrcall))\
		throw Graphics::GfxExcepion( __LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall)\
    if (FAILED(hr = hrcall))\
        infoManager.Set();\
     GFX_THORW_CALL(hrcall)
#else
#define GFX_THROW_CALL(hrcall) GFX_CALL(hrcall)
#endif // 


Graphics::Graphics(HWND hWnd)
{
    // create a reference device
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
    // create device and swap chain
    unsigned int swapChainCreateFlags = 0u;
#ifndef NDEBUG
    swapChainCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // !NDEBUG
    HRESULT hr = S_OK;
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
        &m_pView
    ));
}

void Graphics::EndFrame()
{
    HRESULT hr = S_OK;
    GFX_THROW_INFO(m_pSwapChain->Present(1u, 0u));
}

void Graphics::ClearBuffer(float r, float g, float b, float a)
{
    const float color[4] = {r, g, b, a};
    m_pContext->ClearRenderTargetView(m_pView.Get(), color);
}


Graphics::GfxExcepion::GfxExcepion(int nLine, const char* szFile, HRESULT hr, std::vector<std::string> v_szMsg)
    :
    Exception(nLine, szFile),
    m_hr(hr)
{
    for (const auto& info:v_szMsg)
    {
        m_info += info;
        m_info.append("\n");
    }
}

Graphics::GfxExcepion::GfxExcepion(int nLine, const char* szFile, HRESULT hr)
    :
    Exception(nLine, szFile),
    m_hr(hr)
{
}

const char* Graphics::GfxExcepion::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code]:0x" << std::hex << GetErrorCode() << std::endl
        << "[Description]:" << TransferErrorCode(m_hr) << std::endl 
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
    return "Direx3D Exception";
}

HRESULT Graphics::GfxExcepion::GetErrorCode() const noexcept
{
    return m_hr;
}

std::string Graphics::GfxExcepion::TransferErrorCode(HRESULT hr)
{
    char* szMsgErr;
    unsigned int uMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&szMsgErr), 0, nullptr
    );
    if (uMsgLen == 0)
    {
        return "undefine error code";
    }
    std::string szResult = szMsgErr;
    return szResult;
}

std::string Graphics::GfxExcepion::GetErrorInfo() const noexcept
{
    return m_info;
}
