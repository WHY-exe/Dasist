#include "Camera.h"
#include "imgui.h"
#include "MathTool.h"
#include <algorithm>
Camera::Camera()
	:
	m_pos(0.0f, 0.0f, -10.0f),
	m_rot(0.0f, 0.0f, 0.0f)
{
}

DirectX::XMMATRIX Camera::GetMatrix() const
{
	if (m_bView)
	{
		return GetFPMatrix();
	}
	else
	{
		return GetTPMatrix();
	}
}

void Camera::Rotate(float dx, float dy) noexcept
{
	if (m_bView)
	{
		m_rot.y = math_tool::wrap_angle(m_rot.y + dx * m_rot_speed);
		m_rot.x = std::clamp(m_rot.x + dy * m_rot_speed, 0.995f * (-PI) / 2.0f, 0.995f * PI / 2.0f);
	}
}

void Camera::Translate(float dx, float dy, float dz)
{
	using namespace DirectX;
	DirectX::XMVECTOR delta_pos = {dx, dy, dz};
	const DirectX::XMVECTOR lookVec = DirectX::XMVector3Transform(
		delta_pos, DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, 0.0f)
	);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_pos);
	pos += lookVec;
	DirectX::XMStoreFloat3(&m_pos, pos);
}

DirectX::XMMATRIX Camera::GetFPMatrix() const
{
	using namespace DirectX;
	// 第一人称视角摄像机的矩阵变换
	const DirectX::XMVECTOR forwardBaseVec = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// 将Z轴方向上的单位向量，与X轴、Y轴上的旋转矩阵相乘，得到现在摄像机的朝向
	// (对于第一人称视角来说，只有X轴和Y轴两个旋转方向)
	const DirectX::XMVECTOR lookVec = DirectX::XMVector3Transform(
		forwardBaseVec, DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, 0.0f)
	);
	// 将摄像机的视野朝向与摄像机位置相加得到摄像机的朝向向量
	const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&m_pos);
	const DirectX::XMVECTOR camTarget = camPos + lookVec;
	// 最后向世界中的物体施加变换矩阵使世界按现有的摄像机视角变化
	// 使用一个相机位置、一个向上的方向和一个焦点为左手坐标系构建一个视图矩阵。
	// 在这里Y轴为向上的方向
	return DirectX::XMMatrixLookAtLH(camPos, camTarget, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

DirectX::XMMATRIX Camera::GetTPMatrix() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
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
		if (m_bView)
		{
			ImGui::SliderAngle("AngleX", &m_rot.x, 0.995 * -90.0f, 0.995 * 90.0f, "%.1f");
			ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		}
		else
		{
			ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
			ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
		}
		if (ImGui::Button("First Person View"))
		{
			m_bView = true;
			m_rot = { 0.0f, 0.0f ,0.0f };
		}
		if (ImGui::Button("Thrid Person View"))
		{
			m_bView = false;
			m_rot = { 0.0f, 0.0f ,0.0f };
		}
	}
	ImGui::End();

}

