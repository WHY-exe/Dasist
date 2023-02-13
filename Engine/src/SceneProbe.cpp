#include "SceneProbe.h"
#include "Technique.h"
#include "DynamicConstantBuffer.h"
#include "imgui.h"
bool Scene::Probe::VisitBuffer(DCBuf::Buffer material_data)
{
	bool dirty = false;
	auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
	ImGui::Text("Material");
	if (auto v = material_data["enNormal"]; v.Exists())
	{
		dcheck(ImGui::Checkbox("Normal Map", &v));
	}
	return dirty;
}

void Scene::Probe::OnSetTechnique()
{
	using namespace std::string_literals;
	ImGui::TextColored({ 0.4f, 0.1f, 0.6f, 1.0f }, m_pTech->GetTechName().c_str());
	bool active = m_pTech->IsActive();
	ImGui::Checkbox(("Tech Active##"s + m_pTech->GetTechName()).c_str(), &active);
	m_pTech->SetActiveState(active);
}

void Scene::Probe::OnSetStep()
{
}
