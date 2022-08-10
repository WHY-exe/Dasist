#include "Camera.h"
#include "imgui.h"
Camera::Camera()
	:
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f)
{
}

DirectX::XMMATRIX Camera::GetMatrix() const
{
	return 
		DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
		DirectX::XMMatrixTranslation(-m_pos.x, -m_pos.y, -m_pos.z);
}

void Camera::SpwanControlWindow()
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderAngle("X", &m_pos.x, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderAngle("Y", &m_pos.y, -1000.0f, 1000.0f, "%.1f");
		ImGui::SliderAngle("Z", &m_pos.z, -1000.0f, 1000.0f, "%.1f");
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
	}
	ImGui::End();

}

