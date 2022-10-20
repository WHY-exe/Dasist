#pragma once
#include "Bindable.h"
#include "Surface.h"
class Texture: public Bindable
{
public:
	Texture(Graphics& gfx, const std::wstring& path, unsigned int slot = 0);
	void Bind(Graphics& gfx) noexcept override;
	static std::shared_ptr<Bindable> Resolve(Graphics& gfx, const std::wstring& path, unsigned int slot = 0) noexcept(!IS_DEBUG);
	static std::wstring GenUID(const std::wstring& path, unsigned int slot = 0) noexcept;
	std::wstring GetUID() const noexcept override;
private:
	unsigned int m_slot;
	std::wstring m_Path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexView;
};

