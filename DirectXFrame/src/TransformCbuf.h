#pragma once
#include "Drawable.h"
#include "ConstantBuffer.h"
class TransformCbuf:public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> m_vcbuf;
	const Drawable& m_parent;
};

