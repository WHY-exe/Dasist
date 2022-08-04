#include "PixelShader.h"
#include <d3dcompiler.h>
#include "GfxThrowMacro.h"

PixelShader::PixelShader(Graphics& gfx, std::wstring szPath)
{
	IMPORT_INFOMAN(gfx);
    Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
    GFX_THROW_INFO(D3DReadFileToBlob(szPath.c_str(), &pBlob));
    // create vertex shader
    GFX_THROW_INFO(
        GetDevice(gfx)->CreatePixelShader(
            pBlob->GetBufferPointer(),
            pBlob->GetBufferSize(),
            nullptr, &m_pPixelShader
        )
    );
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
    GetContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}
