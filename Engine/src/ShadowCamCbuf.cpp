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

void ShadowCamCbuf::Update(Graphics& gfx) const noexcept
{
	Transform tf = {
		m_pCamera->GetCameraMatrix() * m_pCamera->GetPerspectiveViewMX()
	};
	m_vcbuf->Update(gfx, tf);
}

void ShadowCamCbuf::SetCamera(const Camera& cam) noexcept
{
	m_pCamera = &cam;
}

