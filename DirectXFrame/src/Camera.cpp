#include "Camera.h"
#include "imgui.h"
Camera::Camera()
	:
	m_pos(0.0f, 0.0f, 0.001f),
	m_rot(0.0f, 0.0f, 0.0f)
{
}

DirectX::XMMATRIX Camera::GetMatrix() const
{
	return 
	DirectX::XMMatrixLookAtLH(
		DirectX::XMVector3Transform(
			DirectX::XMVectorSet(0.0f, 0.0f, -m_pos.z, 0.0f),
			DirectX::XMMatrixRotationRollPitchYaw(m_pos.x, -m_pos.y, 0.0f)
		),
		DirectX::XMVectorZero(),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	) * DirectX::XMMatrixRotationRollPitchYaw(m_rot.x , m_rot.y, m_rot.z);
}

void Camera::SpwanControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderAngle("X", &m_pos.x, -89.0f, 89.0f, "%.1f");
		ImGui::SliderAngle("Y", &m_pos.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Z", &m_pos.z, 1.0f, 80.0f, "%.1f");
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
	}
	ImGui::End();

}

