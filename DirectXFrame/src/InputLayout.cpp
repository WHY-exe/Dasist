#include "InputLayout.h"
#include "GfxThrowMacro.h"
InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode)
{
    IMPORT_INFOMAN(gfx);
    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        layout.data(), layout.size(),
        pVertexShaderByteCode->GetBufferPointer(),
        pVertexShaderByteCode->GetBufferSize(),
        &m_pInputLayout
    ));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}
