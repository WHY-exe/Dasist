#include "Light.h"
#include "imgui.h"
Light::Light(Graphics& gfx)
	:
	m_pos(0.0f, 0.0f, 0.0f),
	m_PSCbuf(gfx)
{
}

void Light::Bind(Graphics& gfx) noexcept
{
	m_PSCbuf.Update(
		gfx, 
		LightCBuffer(
			m_pos,
			0.0f,
			m_Ambient,
			0.0f,
			m_diffuseColor,
			0.0f,
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
		ImGui::Text("Ambient");
		ImGui::SliderFloat("R", &m_Ambient.x, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("G", &m_Ambient.y, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("B", &m_Ambient.z, 0.0f, 1.0f, "%.2f");
		ImGui::Text("LightColor");
		ImGui::SliderFloat("LightColorR", &m_diffuseColor.x, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("LightColorG", &m_diffuseColor.y, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat("LightColorB", &m_diffuseColor.z, 0.0f, 1.0f, "%.2f");
		ImGui::Text("LightIntensity");
		ImGui::SliderFloat("Intensity", &m_diffuseIntensity, 0.0f, 10.0f, "%.2f");
	}
	ImGui::End();
}
