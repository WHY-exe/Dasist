#pragma once
#include "Bindable.h"
#include <memory>
class Sampler : public Bindable
{
public:
	enum class Type
	{
		Anisotropic,
		Bilinear,
		Point,
	};
public:
	Sampler(Graphics& gfx, Type type = Type::Anisotropic, bool reflect = false);
	void Bind(Graphics& gfx) noexcept(!_DEBUG) override;
	static std::shared_ptr<Sampler> Resolve(Graphics& gfx, Type type = Type::Anisotropic, bool reflect = false) noexcept(!_DEBUG);
	static std::wstring GenUID(Type type, bool reflect) noexcept;
	std::wstring GetUID() const noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	Type m_type;
	bool m_reflect;
};

