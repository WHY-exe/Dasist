#include "CameraContainer.h"
#include "Signal.h"
#include <imgui.h>
CameraContainer::CameraContainer(Graphics& gfx)
	:
	m_defaultViewWidth(float(gfx.GetWindowWidth())),
	m_defaultViewHeight(float(gfx.GetWindowHeight()))
{
	Add(gfx);
}
Camera& CameraContainer::GetCamera() noexcept
{
	return *m_Container[m_cur_idx];
}
void CameraContainer::UpdateDefaultValues(Graphics& gfx) noexcept(!IS_DEBUG)
{
	m_defaultViewHeight = gfx.GetWindowHeight();
	m_defaultViewWidth = gfx.GetWindowWidth();
}
void CameraContainer::Bind(Graphics& gfx) noexcept(!IS_DEBUG)
{
	UpdateDefaultValues(gfx);
	SIGNAL(gfx.sizeSignalPrj, m_Container[m_cur_idx]->UpdateDefaultValues(gfx));
	gfx.SetProjection(m_Container[m_cur_idx]->GetPerspectiveViewMX());
	gfx.SetCamera(m_Container[m_cur_idx]->GetCameraMatrix());
}

void CameraContainer::SpawControlWindow() noexcept(!IS_DEBUG)
{	
	if (ImGui::Begin("Cameras"))
	{
		if (ImGui::BeginCombo("Current Camera", m_Container[m_cur_idx]->GetName().c_str()))
		{
			for (int n = 0; n < m_Container.size(); n++)
			{
				const bool isSelected = m_cur_idx == n;
				if (ImGui::Selectable(m_Container[n]->GetName().c_str(), isSelected))
				{
					m_cur_idx = n;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Add Camera"))
		{
			Add(m_defaultViewWidth, m_defaultViewHeight);
		}
		m_Container[m_cur_idx]->ShowControlWidget();
		ImGui::End();
	}
}

void CameraContainer::Add(std::unique_ptr<Camera>& camera) noexcept(!IS_DEBUG)
{
	m_Container.push_back(std::move(camera));
}

void CameraContainer::Add(float ViewWidth, float ViewHeight) noexcept(!IS_DEBUG)
{
	m_Container.emplace_back(std::make_unique<Camera>(ViewWidth, ViewHeight, "Camera" + std::to_string(m_Container.size() + 1)));
}

void CameraContainer::Add(Graphics& gfx) noexcept(!IS_DEBUG)
{
	m_Container.emplace_back(std::make_unique<Camera>(gfx, "Camera" + std::to_string(m_Container.size()+1)));
}
