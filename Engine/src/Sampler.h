#pragma once
#include "Bindable.h"
class Sampler : public Bindable
{
public:
	Sampler(Graphics& gfx);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<Bindable> Resolve(Graphics& gfx) noexcept(!IS_DEBUG);
	static std::wstring GenUID() noexcept;
	std::wstring GetUID() const noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
};

