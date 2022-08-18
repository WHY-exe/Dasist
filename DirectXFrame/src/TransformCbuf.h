#pragma once
#include "Drawable.h"
#include "ConstantBuffer.h"
#include <memory>
class TransformCbuf:public Bindable
{
private:
	struct Transform {
		DirectX::XMMATRIX modelProjView;
		DirectX::XMMATRIX modelWorldView;
	};
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot = 0);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transform>> s_vcbuf;
	const Drawable& m_parent;
};

