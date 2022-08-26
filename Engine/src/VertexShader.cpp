#include "VertexShader.h"
#include "GfxThrowMacro.h"
#include <d3dcompiler.h>
VertexShader::VertexShader(Graphics& gfx, const std::wstring szPath)
{
    IMPORT_INFOMAN(gfx);
    // read the compiled vertex shader to the memory
    GFX_THROW_INFO(D3DReadFileToBlob(szPath.c_str(), &m_pByteCodeBlob));
    // create vertex shader
    GFX_THROW_INFO(
        GetDevice(gfx)->CreateVertexShader(
            m_pByteCodeBlob->GetBufferPointer(),
            m_pByteCodeBlob->GetBufferSize(),
            nullptr, &m_pVertexShader
        )
    );
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
    // bind vertex shader to the pipeline
    GetContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
    return m_pByteCodeBlob.Get();
}
