#pragma once
#include "Drawable.h"
#include "ConstantBuffer.h"
#include <memory>
class TransformCbuf:public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> s_vcbuf;
	const Drawable& m_parent;
};

