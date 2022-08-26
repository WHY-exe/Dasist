#pragma once
#include "GfxThrowMacro.h"
#include "Bindable.h"
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<UINT>& indicies);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetSize() const;
private:
	UINT m_uBufferSize;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
};

