#pragma once
#include "Bindable.h"
#include "Surface.h"
class CubeTexture : public Bindable
{
public:
	CubeTexture(Graphics& gfx, const std::string& base_path,  const std::vector<std::string>& paths, UINT slot = 0u);
	void Bind(Graphics& gfx) noexcept(!IS_DEBUG) override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pCubeTex;
	UINT m_slot;
};

