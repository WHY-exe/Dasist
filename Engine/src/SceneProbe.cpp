#include "SceneProbe.h"
#include "Technique.h"
#include "Scene.h"
#include "MathTool.h"
#include "DynamicConstantBuffer.h"
#include "imgui.h"
bool Scene::MaterialProbe::VisitBuffer(DCBuf::Buffer& material_data)
{
	bool dirty = false;
	if (m_node_active)
	{
		auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
		ImGui::Text("Material");
		if (auto v = material_data["enNormal"]; v.Exists())
		{
			dcheck(ImGui::Checkbox("Normal Map", &v));
		}
	}
	return dirty;
}

void Scene::MaterialProbe::OnSetTechnique()
{
	using namespace std::string_literals;
	if (m_pTech->GetTechName() == "OutLine")
	{
		m_pTech->SetActiveState(m_node_active);
	}
	if (m_node_active)
	{
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.0f }, m_pTech->GetTechName().c_str());
		bool active = m_pTech->IsActive();
		ImGui::Checkbox(("Tech Active##"s + m_pTech->GetTechName()).c_str(), &active);
		m_pTech->SetActiveState(active);
	}

}

bool Scene::MaterialProbe::SetActive(bool active) noexcept
{
	m_node_active = active;
	return m_node_active;
}

bool Scene::MaterialProbe::IsActive() const noexcept
{
	return m_node_active;
}

Scene::NodeProbe::NodeProbe() noexcept
{
	DirectX::XMStoreFloat4x4(&m_transformation, DirectX::XMMatrixIdentity());
}

bool Scene::NodeProbe::VisitNode(Node& node) noexcept(!IS_DEBUG)
{
	bool dirty = false;
	
	if (m_matProbe.SetActive(m_selected_node_id == node.GetId()))
	{
		auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
		const auto& nodeTransform = node.GetAppliedTransform();
		const auto euler_angle = math_tool::ExtraEulerAngle(nodeTransform);
		const auto translation = math_tool::ExtraTranslation(nodeTransform);
		const auto scaling = math_tool::ExtraScaling(nodeTransform);

		float scalin_x = scaling.x;
		float scalin_y = scaling.y;
		float scalin_z = scaling.z;
		float pitch = euler_angle.x;
		float yaw = euler_angle.y;
		float roll = euler_angle.z;
		float x = translation.x;
		float y = translation.y;
		float z = translation.z;

		ImGui::Text("Position");
		dcheck(ImGui::SliderFloat("X", &x, -80.0f, 80.0f, "%.1f"));
		dcheck(ImGui::SliderFloat("Y", &y, -80.0f, 80.0f, "%.1f"));
		dcheck(ImGui::SliderFloat("Z", &z, -80.0f, 80.0f, "%.1f"));
		ImGui::Text("Angle");
		dcheck(ImGui::SliderAngle("AngleX", &roll, -180.0f, 180.0f, "%.1f"));
		dcheck(ImGui::SliderAngle("AngleY", &yaw, -180.0f, 180.0f, "%.1f"));
		dcheck(ImGui::SliderAngle("AngleZ", &pitch, -180.0f, 180.0f, "%.1f"));	
		ImGui::Text("Scale");
		dcheck(ImGui::SliderFloat("Scalin_x", &scalin_x, 0.0f, 2.0f, "%.3f"));
		dcheck(ImGui::SliderFloat("Scalin_y", &scalin_y, 0.0f, 2.0f, "%.3f"));
		dcheck(ImGui::SliderFloat("Scalin_z", &scalin_z, 0.0f, 2.0f, "%.3f"));

		if (dirty)
		{
			DirectX::XMStoreFloat4x4(
				&m_transformation,
				DirectX::XMMatrixScaling(scalin_x, scalin_y, scalin_z) *
				DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
				DirectX::XMMatrixTranslation(x, y, z));
		}
	}
	node.Accept(m_matProbe);
	return dirty;
}

void Scene::NodeProbe::SetSelectedNodeId(int node_id) noexcept
{
	m_selected_node_id = node_id;
}

DirectX::XMMATRIX Scene::NodeProbe::GetTransformMatrix() noexcept
{
	auto matrix = DirectX::XMLoadFloat4x4(&m_transformation);
	return matrix;
}
