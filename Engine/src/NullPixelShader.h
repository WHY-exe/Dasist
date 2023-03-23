#pragma once
#include "Bindable.h"
#include "BindableCodex.h"
class NullPixelShader : public Bindable
{
public:
	NullPixelShader(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept(!_DEBUG) override;
	static std::shared_ptr<NullPixelShader> Resolve(Graphics& gfx) noexcept(!_DEBUG);
	static std::wstring GenUID() noexcept;
	std::wstring GetUID() const noexcept;
};