#pragma once
#include "Bindable.h"
class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, std::wstring szPath);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
};

