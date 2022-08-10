#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	m_parent(parent)
{
	if (!s_vcbuf)
	{
		s_vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	s_vcbuf->Update(
		gfx,
		DirectX::XMMatrixTranspose(m_parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection() )
	);
	s_vcbuf->Bind(gfx);
}
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCbuf::s_vcbuf;