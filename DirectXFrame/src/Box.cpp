#include "Box.h"
#include "Model.h"
Box::Box(Graphics& gfx, UINT box_id)
	:
	ModelObj<Box>(m_obj, gfx, Surface("./images/box.png"), L"cso\\TexVS.cso", L"cso\\TexPS.cso"),
	m_box_id(box_id)
{
}

void Box::SpwanControlWindow() noexcept
{
	ModelObj<Box>::SpwanControlWindow("Box", m_box_id);
}

DirectX::XMMATRIX Box::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

const Model Box::m_obj("obj\\cube.obj");