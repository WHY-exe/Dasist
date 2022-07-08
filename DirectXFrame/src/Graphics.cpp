#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
// link the library
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
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
        auto v = infoManager.GetMessages();\
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


Graphics::Graphics(HWND hWnd, int nWinWidth = 0, int nWinHeight = 0)
    :
    m_nWinWidth(nWinWidth),
    m_nWinHeight(nWinHeight)
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
        &m_pView
    ));
}



void Graphics::DrawTestTriangle(float angle)
{
    INIT_GFX_EXCEPTION;
    struct Vertex
    {
        struct {
            float x;
            float y;
        } pos;
        struct {
            unsigned char r;
            unsigned char g; 
            unsigned char b; 
            unsigned char a;

        } color;
    };
    // let's say it is creating vertex buffer
    const Vertex vertices[] = {
        {0.5f, 0.5f, 255, 0, 0},
        {0.5f, -0.5f, 0, 255, 0},
        {-0.5f, -0.5f, 255, 0, 255},
        {-0.5f, 0.5f, 255, 255, 0}
    };
    // const buffer that will be passed to the shader
    struct ConstantBuffer
    {
        struct {
            float elements[4][4];
        } tranformation;
    };
    const ConstantBuffer cb =
    {
        {
            std::cos(angle),  std::sin(angle), 0.0f, 0.0f,
            (m_nWinWidth / m_nWinHeight) * -std::sin(angle), (m_nWinWidth / m_nWinHeight)* std::cos(angle), 0.0f, 0.0f,
            0.0f,             0.0f,            1.0f, 0.0f,
            0.0f,             0.0f,            0.0f, 1.0f
        }
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
    D3D11_BUFFER_DESC cbbd = {};
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.Usage = D3D11_USAGE_DYNAMIC;
    cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbbd.MiscFlags = 0u;
    cbbd.ByteWidth = sizeof(cb);
    cbbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA sdcb = {};
    sdcb.pSysMem = &cb;
    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&cbbd, &sdcb, &pConstantBuffer));

    m_pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


    Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
    // buffer description
    D3D11_BUFFER_DESC vbbd = {};
    vbbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbbd.Usage = D3D11_USAGE_DEFAULT;
    vbbd.CPUAccessFlags = 0u;
    vbbd.MiscFlags = 0u;
    vbbd.ByteWidth = sizeof(vertices);
    vbbd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sdVerts = {};
    sdVerts.pSysMem = vertices;
    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&vbbd, &sdVerts, &pVertexBuffer));
    // bind the vertex buffer to the pipeline
    const UINT vbstride = sizeof(Vertex);
    const UINT vboffset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &vbstride, &vboffset);

    // create index buffer
    const UINT indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
    D3D11_BUFFER_DESC ibbd = {};
    ibbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibbd.Usage = D3D11_USAGE_DEFAULT;
    ibbd.CPUAccessFlags = 0u;
    ibbd.ByteWidth = sizeof(indices);
    ibbd.StructureByteStride = sizeof(UINT);
    D3D11_SUBRESOURCE_DATA sdIdics = {};
    sdIdics.pSysMem = indices;

    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&ibbd, &sdIdics, &pIndexBuffer));
    m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

    // the blob that hold the shader information
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    // create pixel shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"cso\\PixelShader.cso", &pBlob));
    GFX_THROW_INFO(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
    // bind the Pixel shader to the pipeline
    m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
    // bind render target view after the pixel shader
    m_pContext->OMSetRenderTargets(1u, m_pView.GetAddressOf(), nullptr);
    // create vertex shader
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
    GFX_THROW_INFO(D3DReadFileToBlob(L"cso\\VertexShader.cso", &pBlob));
    GFX_THROW_INFO(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
    // bind vertex shader to the pipeline
    m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
    // create the layout of vertex buffer 
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    // the data that will be passed to the shaders
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    GFX_THROW_INFO(m_pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));
    // bind vertex layout
    m_pContext->IASetInputLayout(pInputLayout.Get());
    // set primitive topology to triangle list
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    // configure viewport
    D3D11_VIEWPORT vp = {};
    vp.Width = (float)m_nWinWidth;
    vp.Height = (float)m_nWinHeight;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    // bind the view port to the pipeline
    m_pContext->RSSetViewports(1u, &vp);
    // draw the triangle
    GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u));
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

