#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent, UINT slot)
	:
	m_parent(parent)
{
	if (!s_vcbuf)
	{
		s_vcbuf = std::make_unique<VertexConstantBuffer<Transform>>(gfx, slot);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	const Transform tf =
	{
		DirectX::XMMatrixTranspose(m_parent.GetTransformXM()),
		DirectX::XMMatrixTranspose(m_parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection())
	};

	s_vcbuf->Update(gfx, tf);
	s_vcbuf->Bind(gfx);
}
std::unique_ptr<VertexConstantBuffer<TransformCbuf::Transform>> TransformCbuf::s_vcbuf;