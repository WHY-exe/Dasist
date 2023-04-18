#include "SceneProbe.h"
#include "Technique.h"
#include "Scene.h"
#include "MathTool.h"
#include "DynamicConstantBuffer.h"
#include "imgui.h"
bool Scene::MaterialProbe::VisitBuffer(DCBuf::Buffer& material_data)
{
	bool dirty = false;
	if (m_selected)
	{
		auto dcheck = [&dirty](bool change) {dirty = dirty || change; };
		if (auto v = material_data["normalMapWeight"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(("normalMapWeight##" + m_cur_mesh_name).c_str(), &v, 0.0f, 3.0f));
		}
		if (auto v = material_data["SpecIntensity"]; v.Exists())
		{
			dcheck(ImGui::SliderFloat(("SpecIntensity##" + m_cur_mesh_name).c_str(), &v, 0.0f, 3.0f));
		}
		if (auto v = material_data["enNormal"]; v.Exists())
		{
			dcheck(ImGui::Checkbox(("Normal Map##" + m_cur_mesh_name).c_str(), &v));
		}
	}
	return dirty;
}

void Scene::MaterialProbe::SetCurMeshName(std::string cur_mesh_name) noexcept
{
	m_cur_mesh_name = std::move(cur_mesh_name);
}

void Scene::MaterialProbe::SetSelectStatus(bool status) noexcept
{
	m_selected = status;
}

void Scene::MaterialProbe::OnSetTechnique()
{
	using namespace std::string_literals;
	if (m_pTech->GetTechName() == "OutLine")
	{
		m_pTech->SetActiveState(m_selected);
	}
	if (m_selected)
	{
		if (m_pTech->GetTechName() != "OutLine")
		{ 
			ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.0f }, m_pTech->GetTechName().c_str());
			bool active = m_pTech->IsActive();
			ImGui::Checkbox(("Tech Active##"s + m_pTech->GetTechName() + m_cur_mesh_name).c_str(), &active);
			m_pTech->SetActiveState(active);
		}
	}

}


bool Scene::MaterialProbe::IsSelected() const noexcept
{
	return m_selected;
}


Scene::TNodeProbe::TNodeProbe() noexcept
{
	DirectX::XMStoreFloat4x4(&m_transformation, DirectX::XMMatrixIdentity());
}

bool Scene::TNodeProbe::VisitNode(Node& node) noexcept(!IS_DEBUG)
{
	bool dirty = false;
	const bool isParentSelected = node.ParentSelected();
	const int selected_id = !m_pSelectedNode ? -1 : m_pSelectedNode->GetId();
	bool isActive = (selected_id == node.GetId());
	if (!isParentSelected)
	{
		node.SetSelectStatus(isActive);
	}
	if (isActive)
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

		if (ImGui::Button("Reset", ImVec2(50, 40)))
		{
			dirty = true;
			scalin_x = 1.0f;
			scalin_y = 1.0f;
			scalin_z = 1.0f;
			pitch = 0.0f;
			yaw = 0.0f;
			roll = 0.0f;
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
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

bool Scene::TNodeProbe::PushNode(Node& node) noexcept(!IS_DEBUG)
{
	const int selected_id = (m_pSelectedNode == nullptr) ? -1 : m_pSelectedNode->GetId();
	const auto imgui_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((node.GetId() == selected_id) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((node.HasChild() ? ImGuiTreeNodeFlags_Leaf : 0));
	// render the node
	const auto expand = ImGui::TreeNodeEx((void*)(intptr_t)node.GetId(), imgui_flags, node.GetName().c_str());
	if (ImGui::IsItemClicked() || ImGui::IsItemActivated())
	{
		m_pSelectedNode = &node;
	}
	return expand;
}

void Scene::TNodeProbe::PopNode() const noexcept(!IS_DEBUG)
{
	ImGui::TreePop();
}

DirectX::XMMATRIX Scene::TNodeProbe::GetTransformMatrix() noexcept
{
	auto matrix = DirectX::XMLoadFloat4x4(&m_transformation);
	return matrix;
}

void Scene::ModelProbe::SpwanControlWindow(Model& model) noexcept(!IS_DEBUG)
{
	if (ImGui::Begin(model.GetName().c_str()))
	{
		ImGui::Columns(2, nullptr, true);
		model.AcceptToShowTree(np);
		ImGui::NextColumn();
		model.Accept(np);
	}		
	ImGui::End();
}

void Scene::ModelProbe::SpwanControlWindow(Scene& scene) noexcept(!IS_DEBUG)
{
	for (auto& i : scene.GetModels())
	{
		SpwanControlWindow(*i);
	}
}
