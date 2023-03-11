#pragma once
#include "Bindable.h"
#include <memory>
class Sampler : public Bindable
{
public:
	Sampler(Graphics& gfx, bool anisoEnable = true, bool reflect = false);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<Sampler> Resolve(Graphics& gfx, bool anisoEnable = true, bool reflect = false) noexcept(!IS_DEBUG);
	static std::wstring GenUID(bool anisoEnable, bool reflect) noexcept;
	std::wstring GetUID() const noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	bool m_anisoEnable;
	bool m_reflect;
};

