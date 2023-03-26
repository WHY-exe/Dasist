#include "Camera.h"
#include "imgui.h"
#include "MathTool.h"
#include "Graphics.h"
#include <algorithm>
Camera::Camera(Graphics& gfx, std::string szName)
	:
	m_szName(szName),	
	m_defaultViewWidth(gfx.GetWindowWidth()),
	m_defaultViewHeight(gfx.GetWindowHeight()),
	m_viewWidth((float)(gfx.GetWindowWidth())),
	m_viewHeight((float)(gfx.GetWindowHeight())),
	m_NearZ(0.5f),
	m_FarZ(100000.0f),
	m_pos(0.0f, 100.0f, -25.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_indicator(gfx)
{
	m_indicator.SetPos(m_pos);
	m_indicator.SetRot(m_rot);
}

DirectX::XMMATRIX Camera::GetCameraMatrix() const
{
	return GetFPMatrix();
}

DirectX::XMMATRIX Camera::GetPerspectiveViewMX() const noexcept(!IS_DEBUG)
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, (float)(m_viewHeight/m_viewWidth), m_NearZ, m_FarZ);
}

const std::string& Camera::GetName() const noexcept
{
	return m_szName;
}

void Camera::Rotate(float dx, float dy) noexcept
{
	m_rot.y = math_tool::wrap_angle(m_rot.y + dx * m_rot_speed);
	m_rot.x = std::clamp(m_rot.x + dy * m_rot_speed, 0.995f * (-PI) / 2.0f, 0.995f * PI / 2.0f);
}

void Camera::Translate(float dx, float dy, float dz)
{
	using namespace DirectX;
	DirectX::XMVECTOR delta_pos = {dx, dy, dz};
	// ����������������ǰ���������ӽ������ƶ��ı䣬ֻ�轫m_rot.x, �滻Ϊ0.0f����
	const DirectX::XMVECTOR delta_lookVec = DirectX::XMVector3Transform(
		delta_pos, DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, 0.0f)
	);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_pos);
	pos += delta_lookVec;
	DirectX::XMStoreFloat3(&m_pos, pos);
}

bool Camera::MouseStatus() const noexcept
{
	return m_hideMouse;
}

void Camera::UpdateDefaultValues(Graphics& gfx) noexcept
{
	m_defaultViewHeight = gfx.GetWindowHeight();
	m_defaultViewWidth = gfx.GetWindowWidth();
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

void Camera::ShowControlWidget() noexcept(!IS_DEBUG)
{
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
	ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
	ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");		
	m_indicator.SetPos(m_pos);
	ImGui::Text("Rotation");
	ImGui::SliderAngle("AngleX", &m_rot.x, 0.995f * -90.0f, 0.995f * 90.0f, "%.1f");
	ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
	m_indicator.SetRot(m_rot);
	ImGui::Text("Projection");
	ImGui::SliderFloat("ViewWidth", &m_viewWidth, -0, 2000.0f, "%.1f");
	ImGui::SliderFloat("ViewHeight", &m_viewHeight, -0, 2000.0f, "%.1f");
	ImGui::SliderFloat("NearZ", &m_NearZ, 0.1f, 80.0f, "%.1f");
	ImGui::SliderFloat("FarZ", &m_FarZ, 0.1f, 10000.0f, "%.1f");
	if (ImGui::Button("Reset Projection"))
	{
		m_viewWidth  = float(m_defaultViewWidth);
		m_viewHeight = float(m_defaultViewHeight);
		m_NearZ = 0.5f;
		m_FarZ = 10000.0f;
	}
	ImGui::Text("Reset to default");
	if (ImGui::Button("Reset"))
	{
		m_pos = homePos;
		m_rot = { 0.0f, 0.0f ,0.0f };
		m_viewWidth = float(m_defaultViewWidth);
		m_viewHeight = float(m_defaultViewHeight);
		m_NearZ = 0.5f;
		m_FarZ = 10000.0f;
	}
	ImGui::Text("First Person Shooter experience");
	if (!m_hideMouse)
	{
		if (ImGui::Button("First Person Mouse"))
		{
			HideMouse();
		}
	}
	else
	{
		ImGui::Text("Press escape to exit this mod");
	}
}

void Camera::HideMouse() noexcept
{
	m_hideMouse = true;
}

void Camera::ShowMouse() noexcept
{
	m_hideMouse = false;
}

void Camera::LinkTechniques(Rgph::RenderGraph& rg)
{
	m_indicator.LinkTechniques(rg);
}

void Camera::Submit() const
{
	m_indicator.Submit();
}
