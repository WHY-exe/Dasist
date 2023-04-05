#include "ShadowCamCbuf.h"

ShadowCamCbuf::ShadowCamCbuf(Graphics& gfx, UINT slot)
	:
	m_vcbuf(std::make_unique<VertexConstantBuffer<Transform>>(gfx, slot))
{
}

void ShadowCamCbuf::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
	m_vcbuf->Bind(gfx);
}

void ShadowCamCbuf::Update(Graphics& gfx) noexcept
{
	const Transform tf{
		DirectX::XMMatrixTranspose(
			m_pCamera->GetCameraMatrix() * m_pCamera->GetPerspectiveViewMX()
		)
	};
	m_vcbuf->Update(gfx, tf);
}

void ShadowCamCbuf::SetCamera(std::shared_ptr<Camera> cam) noexcept
{
	m_pCamera = std::move(cam);
}

