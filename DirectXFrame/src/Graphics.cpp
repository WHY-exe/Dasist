#include "Graphics.h"
#include <sstream>
// link the library
#pragma comment(lib, "d3d11.lib")

#ifndef NDEBUG
#define INIT_GFX_EXCEPTION\
    HRESULT hr = S_OK
#define GFX_THORW(hrcall)\
	if(FAILED(hr = hrcall))\
		throw Graphics::GfxExcepion( __LINE__, __FILE__, hr, infoManager.GetMessages())
#define GFX_THROW_INFO(hrcall)\
    if (FAILED(hr = hrcall))\
        infoManager.Set();\
     GFX_THORW(hr)
#define GFX_DEVICE_REMOVED_CALL(hrcall) throw Graphics::GfxExcepion( __LINE__, __FILE__, (hrcall), infoManager.GetMessages())
#define GFX_THROW_INFO_ONLY(call)\
    infoManager.Set();\
    (call);\
    {\
        auto v = infoManager.GetMessage();\
        if (!v.empty())\
        {\
            throw Graphics::GfxInfoOnlyException(__LINE__, __FILE__, v);\
        }\
    }
#else
#define INIT_GFX_EXCEPTION\
    HRESULT hr = S_OK
#define GFX_THORW(hrcall)\
	if(FAILED(hr = hrcall))\
		throw Graphics::GfxExcepion( __LINE__, __FILE__, hrcall\)
#define GFX_THROW_INFO(hrcall) GFX_THORW(hrcall)
#define GFX_THROW_INFO_ONLY(call) call
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
        &m_pView
    ));
}

void Graphics::DrawTestTriangle()
{

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
    m_pContext->ClearRenderTargetView(m_pView.Get(), color);
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

