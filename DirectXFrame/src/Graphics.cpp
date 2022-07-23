#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "Model.h"
namespace DX = DirectX;

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

    // let's say it is creating vertex buffer
    Model cube("obj\\cube.obj");
    m_indices_len = cube.GetIndices().size();
    // buffer description
    D3D11_BUFFER_DESC vbbd = {};
    vbbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbbd.Usage = D3D11_USAGE_DEFAULT;
    vbbd.CPUAccessFlags = 0u;
    vbbd.MiscFlags = 0u;
    vbbd.ByteWidth = cube.GetVertices().size() * sizeof(objl::Vertex);
    vbbd.StructureByteStride = sizeof(objl::Vertex);
    D3D11_SUBRESOURCE_DATA sdVerts = {};
    sdVerts.pSysMem = cube.GetVertices().data();
    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&vbbd, &sdVerts, &m_pVertexBuffer));
    // bind the vertex buffer to the pipeline
    const UINT vbstride = sizeof(objl::Vertex);
    const UINT vboffset = 0u;
    m_pContext->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &vbstride, &vboffset);

    // create index buffer
    D3D11_BUFFER_DESC ibbd = {};
    ibbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibbd.Usage = D3D11_USAGE_DEFAULT;
    ibbd.CPUAccessFlags = 0u;
    ibbd.ByteWidth = cube.GetIndices().size() * sizeof(UINT);
    ibbd.StructureByteStride = sizeof(UINT);
    D3D11_SUBRESOURCE_DATA sdIdics = {};
    sdIdics.pSysMem = cube.GetIndices().data();

    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&ibbd, &sdIdics, &m_pIndexBuffer));
    m_pContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

    // the blob that hold the shader information
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    // create pixel shader
    GFX_THROW_INFO(D3DReadFileToBlob(L"cso\\PixelShader.cso", &pBlob));
    GFX_THROW_INFO(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader));
    // bind the Pixel shader to the pipeline
    m_pContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
    // bind render target view after the pixel shader
    m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);
    // create vertex shader
    GFX_THROW_INFO(D3DReadFileToBlob(L"cso\\VertexShader.cso", &pBlob));
    GFX_THROW_INFO(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVertexShader));
    // bind vertex shader to the pipeline
    m_pContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
    // create the layout of vertex buffer 
    Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
    // the data that will be passed to the shaders
    D3D11_INPUT_ELEMENT_DESC ied[] = {
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"Norm", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    GFX_THROW_INFO(m_pDevice->CreateInputLayout(
        ied, (UINT)std::size(ied),
        pBlob->GetBufferPointer(),
        pBlob->GetBufferSize(),
        &pInputLayout
    ));
    // bind vertex layout
    m_pContext->IASetInputLayout(pInputLayout.Get());

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
}



void Graphics::DrawTestTriangle(float angle, float x, float y)
{
    INIT_GFX_EXCEPTION;
    // const buffer that will be passed to the shader
    struct ConstantBuffer
    {
        struct {
            DX::XMMATRIX transform;
        } tranformation;
    };
    const ConstantBuffer cb =
    {
        {
           DX::XMMatrixTranspose(
               DX::XMMatrixRotationZ(angle) * 
               DX::XMMatrixRotationX(angle) *
               DX::XMMatrixTranslation(x,0.0f,4.0f)*
               DX::XMMatrixPerspectiveLH(1.0f, float(WinHeight) / float(WinWidth),0.5f,10.0f)
           )
        }
    };

    D3D11_BUFFER_DESC cbbd = {};
    cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbbd.Usage = D3D11_USAGE_DYNAMIC;
    cbbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbbd.MiscFlags = 0u;
    cbbd.ByteWidth = sizeof(cb);
    cbbd.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA sdcb = {};
    sdcb.pSysMem = &cb;
    GFX_THROW_INFO_ONLY(m_pDevice->CreateBuffer(&cbbd, &sdcb, &m_pConstantBuffer));

    m_pContext->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());

    struct ConstantBuffer2
    {
        struct
        {
            float r;
            float g;
            float b;
            float a;
        } face_colors[6];
    };
    const ConstantBuffer2 cb2 =
    {
        {
            {1.0f,0.0f,1.0f},
            {1.0f,0.0f,0.0f},
            {0.0f,1.0f,0.0f},
            {0.0f,0.0f,1.0f},
            {1.0f,1.0f,0.0f},
            {0.0f,1.0f,1.0f},
        }
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
    D3D11_BUFFER_DESC cbd2;
    cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd2.Usage = D3D11_USAGE_DEFAULT;
    cbd2.CPUAccessFlags = 0u;
    cbd2.MiscFlags = 0u;
    cbd2.ByteWidth = sizeof(cb2);
    cbd2.StructureByteStride = 0u;
    D3D11_SUBRESOURCE_DATA csd2 = {};
    csd2.pSysMem = &cb2;
    GFX_THROW_INFO(m_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

    // bind constant buffer to pixel shader
    m_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

    // set primitive topology to triangle list
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
    // draw the triangle
    GFX_THROW_INFO_ONLY(m_pContext->DrawIndexed(m_indices_len, 0u, 0u));
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

