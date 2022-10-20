#include "InputLayout.h"
#include "GfxThrowMacro.h"
#include <typeinfo>
InputLayout::InputLayout(Graphics& gfx, const Vertex::Layout& layout, ID3DBlob* pVertexShaderByteCode)
    :
    m_layout(layout)
{
    auto D3DLayout = m_layout.GetD3DLayout();
    IMPORT_INFOMAN(gfx);
    GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(
        D3DLayout.data(), (UINT)D3DLayout.size(),
        pVertexShaderByteCode->GetBufferPointer(),
        pVertexShaderByteCode->GetBufferSize(),
        &m_pInputLayout
    ));
}

void InputLayout::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}

std::shared_ptr<Bindable> InputLayout::Resolve(Graphics& gfx, const Vertex::Layout& layout, ID3DBlob* pVertexShaderByteCode) noexcept
{
    return CodeX::Resolve<InputLayout>(gfx, layout, pVertexShaderByteCode);
}

std::wstring InputLayout::GetUID() const noexcept
{
    return GenUID(m_layout);
}

std::wstring InputLayout::GenUID_(const Vertex::Layout& layout) noexcept
{
    using namespace std::string_literals;
    return ANSI_TO_UTF8_STR(typeid(InputLayout).name()) + L"#" + ANSI_TO_UTF8_STR(layout.GetCode());
}
