#include "Light.h"
#include "imgui.h"
Light::Light(Graphics& gfx)
	:
	m_pos(0.0f, 0.0f, 0.0f),
	m_PSCbuf(gfx)
{
}

void Light::Update(Graphics& gfx) noexcept
{
	m_PSCbuf.Update(
		gfx, 
		LightCBuffer(
			m_pos,
			m_diffuseColor,
			m_diffuseIntensity,
			m_attConst,
			m_attLinear,
			m_attQuad

		)
	);
	m_PSCbuf.Bind(gfx);
}

void Light::SpwanControlWindow() noexcept
{
	if (ImGui::Begin("light"))
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
