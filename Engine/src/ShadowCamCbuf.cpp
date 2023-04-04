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
	for (auto i = m_pCameras.begin(); i != m_pCameras.end(); i++)
	{
		if ((*i)->ShouldDelete())
		{
			m_pCameras.erase(i);
		}
	}
	Transform tf;
	tf.cur_light_num = (UINT)m_pCameras.size();
	for (size_t i = 0; i < MAX_LIGHT_NUM && i < m_pCameras.size(); i++)
	{
		tf.Viewproj[i] = m_pCameras[i]->GetCameraMatrix() * m_pCameras[i]->GetPerspectiveViewMX();
	}
	m_vcbuf->Update(gfx, tf);
}

void ShadowCamCbuf::SetCamera(std::shared_ptr<Camera> cam) noexcept
{
	m_pCameras.push_back(std::move(cam));
}

