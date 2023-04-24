#include "Camera.h"
#include "imgui.h"
#include "MathTool.h"
#include "Graphics.h"
#include <algorithm>
Camera::Camera(Graphics& gfx, std::string szName, bool tethered)
	:
	m_tether_state(tethered),
	m_szName(std::move(szName)),	
	m_defaultViewWidth(gfx.GetWindowWidth()),
	m_defaultViewHeight(gfx.GetWindowHeight()),
	m_viewWidth((float)(gfx.GetWindowWidth())),
	m_viewHeight((float)(gfx.GetWindowHeight())),
	m_NearZ(0.5f),
	m_FarZ(3000.0f),
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_indicator(gfx),
	m_fov_indi(gfx, m_viewWidth / 10.0f, m_viewHeight / 10.0f, m_NearZ, m_FarZ),
	m_gfx(gfx)
{
	m_indicator.SetScale(10.0f);
	if (!m_tether_state)
	{
		m_pos = { 0.0f, 100.0f, -25.0f };
		m_indicator.SetPos(m_pos);
		m_fov_indi.SetPos(m_pos);
	}
	m_indicator.SetRot(m_rot);
	m_fov_indi.SetRot(m_rot);
}

DirectX::XMMATRIX Camera::GetCameraMatrix() const
{
	return GetFPMatrix();
}

DirectX::XMMATRIX Camera::GetPerspectiveViewMX() const noexcept(!IS_DEBUG)
{
	return DirectX::XMMatrixPerspectiveLH(1.0f, (float)(m_viewHeight/m_viewWidth), m_NearZ, m_FarZ);
}

void Camera::BindtoGFX(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	gfx.SetProjection(GetPerspectiveViewMX());
	gfx.SetCamera(GetCameraMatrix());
}

const std::string& Camera::GetName() const noexcept
{
	return m_szName;
}

void Camera::SetTetheredState(bool state) noexcept
{
	m_tether_state = state;
}

bool Camera::GetTetherdState() const noexcept
{
	return m_tether_state;
}

void Camera::SetDeleteState(bool should_delete) noexcept
{
	m_should_delete = should_delete;
}

bool Camera::ShouldDelete() const noexcept
{
	return m_should_delete;
}

DirectX::XMFLOAT3 Camera::GetPos() const noexcept
{
	return m_pos;
}

void Camera::SetPos(float x, float y, float z)
{
	m_pos = { x, y, z };
	m_indicator.SetPos(m_pos);
	m_fov_indi.SetPos(m_pos);
}

void Camera::SetPos(DirectX::XMFLOAT3 pos)
{
	SetPos(pos.x, pos.y, pos.z);
}

void Camera::Rotate(float dx, float dy) noexcept
{
	m_rot.y = math_tool::wrap_angle(m_rot.y + dx * m_rot_speed);
	m_rot.x = std::clamp(m_rot.x + dy * m_rot_speed, 0.995f * (-PI) / 2.0f, 0.995f * PI / 2.0f);
	m_indicator.SetRot(m_rot);
	m_fov_indi.SetRot(m_rot);
}

void Camera::Translate(float dx, float dy, float dz)
{
	// 使用重载运算符要用到这个命名空间
	using namespace DirectX;
	if (!m_tether_state)
	{
		DirectX::XMVECTOR delta_pos = { dx, dy, dz };
		// 如果不想让摄像机的前进方向随视角上下移动改变，只需将m_rot.x, 替换为0.0f即可
		const DirectX::XMVECTOR delta_lookVec = DirectX::XMVector3Transform(
			delta_pos, DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, 0.0f)
		);
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_pos);
		pos += delta_lookVec;
		DirectX::XMStoreFloat3(&m_pos, pos);
		m_indicator.SetPos(m_pos);
		m_fov_indi.SetPos(m_pos);
	}
}

void Camera::DisableIndicator() noexcept
{
	m_enCamIndi = false;
	m_enFOVIndi = false;
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

void Camera::ResetProjection() noexcept
{
	m_viewWidth = float(m_defaultViewWidth);
	m_viewHeight = float(m_defaultViewHeight);
	m_NearZ = 0.5f;
	m_FarZ = 3000.0f;
	m_fov_indi.SetVertices(m_gfx, m_viewWidth / 10.0f, m_viewHeight / 10.0f, m_NearZ, m_FarZ);
}

void Camera::Reset() noexcept
{
	if(!m_tether_state)
	{ 
		m_pos = homePos;
		m_indicator.SetPos(m_pos);
		m_fov_indi.SetPos(m_pos);
	}
	m_rot = { 0.0f, 0.0f ,0.0f };		
	m_indicator.SetRot(m_rot);
	m_fov_indi.SetRot(m_rot);
	ResetProjection();	
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

void Camera::ShowControlCameraWidget() noexcept(!IS_DEBUG)
{
	auto check_dirty = [](bool& dirty, bool func) {
		dirty =  dirty || func;
	};
	bool pos_dirty = false;
	if (!m_tether_state)
	{
		ImGui::Text("Position");
		check_dirty(pos_dirty, ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f"));
		check_dirty(pos_dirty, ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f"));
		check_dirty(pos_dirty, ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f"));		

	}
	if (pos_dirty)
	{
		m_indicator.SetPos(m_pos);
		m_fov_indi.SetPos(m_pos);
	}
	bool orien_dirty = false;
	ImGui::Text("Orientation");
	check_dirty(orien_dirty, ImGui::SliderAngle("AngleX", &m_rot.x, 0.995f * -90.0f, 0.995f * 90.0f, "%.1f"));
	check_dirty(orien_dirty, ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f"));
	if (orien_dirty)
	{
		m_indicator.SetRot(m_rot);
		m_fov_indi.SetRot(m_rot);
	}
	bool proj_dirty = false;	
	ImGui::Text("FOV");
	check_dirty(proj_dirty, ImGui::SliderFloat("ViewWidth", &m_viewWidth, -0, 2000.0f, "%.1f"));
	check_dirty(proj_dirty, ImGui::SliderFloat("ViewHeight", &m_viewHeight, -0, 2000.0f, "%.1f"));
	check_dirty(proj_dirty, ImGui::SliderFloat("NearZ", &m_NearZ, 0.1f, 80.0f, "%.3f", ImGuiSliderFlags_Logarithmic));
	check_dirty(proj_dirty, ImGui::SliderFloat("FarZ", &m_FarZ, 0.1f, 3000.0f, "%.1f"));
	if (proj_dirty)
	{
		m_fov_indi.SetVertices(m_gfx, m_viewWidth / 10.0f, m_viewHeight / 10.0f, m_NearZ, m_FarZ);
	}
	ImGui::Text("Reset FOV");
	if (ImGui::Button("Reset controled camera FOV"))
	{
		ResetProjection();
	}
	ImGui::Text("Reset to default");
	if (ImGui::Button("Reset controled camera"))
	{
		Reset();
	}

	ImGui::Checkbox("Enable Camera Indicator", &m_enCamIndi);
	ImGui::Checkbox("Enable Camera FOV Indicator", &m_enFOVIndi);
}

void Camera::ShowActiveCameraWidget() noexcept(!IS_DEBUG)
{
	ImGui::Text("Reset FOV");
	if (ImGui::Button("Reset FOV"))
	{
		ResetProjection();
	}
	ImGui::Text("Reset to default camera value");
	if (ImGui::Button("Reset"))
	{
		Reset();
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
	m_fov_indi.LinkTechniques(rg);
}

void Camera::Submit(size_t channel) const
{
	if(m_enCamIndi)
		m_indicator.Submit(channel);
	if (m_enFOVIndi)
		m_fov_indi.Submit(channel);
}
