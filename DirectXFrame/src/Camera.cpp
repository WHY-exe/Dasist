#include "Camera.h"
#include "imgui.h"
Camera::Camera()
	:
	m_pos(0.0f, 0.0f, -10.0f),
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
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");		
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
	}
	ImGui::End();

}

