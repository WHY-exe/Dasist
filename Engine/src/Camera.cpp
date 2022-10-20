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
	// ��һ�˳��ӽ�������ľ���任
	const DirectX::XMVECTOR forwardBaseVec = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// ��Z�᷽���ϵĵ�λ��������X�ᡢY���ϵ���ת������ˣ��õ�����������ĳ���
	// (���ڵ�һ�˳��ӽ���˵��ֻ��X���Y��������ת����)
	const DirectX::XMVECTOR lookVec = DirectX::XMVector3Transform(
		forwardBaseVec, DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, 0.0f)
	);
	// �����������Ұ�����������λ����ӵõ�������ĳ�������
	const DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&m_pos);
	const DirectX::XMVECTOR camTarget = camPos + lookVec;
	// ����������е�����ʩ�ӱ任����ʹ���簴���е�������ӽǱ仯
	// ʹ��һ�����λ�á�һ�����ϵķ����һ������Ϊ��������ϵ����һ����ͼ����
	// ������Y��Ϊ���ϵķ���
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

