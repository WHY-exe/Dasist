#include "Light.h"
#include "imgui.h"
PointLight::PointLight(Graphics& gfx)
	:
	m_pos(0.0f, 5.0f, 0.0f),
	m_PSCbuf(gfx)
{
	Scene::RenderOption op;
	op.szModelPath = "res\\model\\sphere.obj";
	op.szPSPath = L"res\\cso\\lightBall.cso";
	m_lightBall = Scene::Model(gfx, op);
	m_lightBall.Scale(0.003f);
}

void PointLight::Update(Graphics& gfx) noexcept
{
	m_PSCbuf.Update(
		gfx, 
		PointLightCBuffer(
			m_pos,
			m_diffuseColor,
			m_diffuseIntensity,
			m_attConst,
			m_attLinear,
			m_attQuad

		)
	);
	m_PSCbuf.Bind(gfx);
	m_lightBall.SetPos(m_pos);
}

void PointLight::SpwanControlWindow() noexcept
{
	if (ImGui::Begin("Point light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::ColorEdit3("LightColor", &m_diffuseColor.x);
		ImGui::Text("LightIntensity");
		ImGui::SliderFloat("Intensity", &m_diffuseIntensity, 0.0f, 10.0f, "%.2f");
	}
	ImGui::End();
}

void PointLight::Draw(Graphics& gfx) noexcept
{
	m_lightBall.Draw(gfx);
}

GlobalLight::GlobalLight(Graphics& gfx)
	:
	m_pos(0.0f, 10.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_PSCbuf(gfx, 1u)
{
}

void GlobalLight::Update(Graphics& gfx) noexcept
{
	m_PSCbuf.Update(
		gfx,
		GlobalLightCBuffer(
			m_pos,
			m_diffuseColor,
			m_diffuseIntensity
		)
	);
	m_PSCbuf.Bind(gfx);
}

void GlobalLight::SpwanControlWindow() noexcept
{
	if (ImGui::Begin("GlobalLight"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::ColorEdit3("LightColor", &m_diffuseColor.x);
		ImGui::Text("LightIntensity");
		ImGui::SliderFloat("Intensity", &m_diffuseIntensity, 0.0f, 10.0f, "%.2f");
	}
	ImGui::End();
}
