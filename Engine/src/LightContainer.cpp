#include "LightContainer.h"
#include "Signal.h"
#include <imgui.h>
LightContainer::LightContainer(Graphics& gfx)
	:
	m_gfx(gfx)
{
	DCBuf::RawLayout cBufferLayout;
	for (auto& i : m_ConstantBufferElements)
	{
		cBufferLayout.Add<DCBuf::Array>(i);
		if(i  == "lightPositions" || i == "lightAmbients" || i == "lightDiffuses")
			cBufferLayout[i].Set<DCBuf::Float3>(MAX_LIGHT_NUM);
		else if (i == "lightIntensities" || i == "attConst" || i == "attLinear" || i == "attQuad")
			cBufferLayout[i].Set<DCBuf::Float>(MAX_LIGHT_NUM);
		else if (i == "Enable")
			cBufferLayout[i].Set<DCBuf::Bool>(MAX_LIGHT_NUM);
	}
	cBufferLayout.Add<DCBuf::Integer>("cur_light_num");
	m_PL_CBuffer = DCBuf::Buffer(std::move(cBufferLayout));
	for (size_t i = 0; i < m_point_lights.size(); i++)
	{
		UpdateCBuffer(i);
	}
	m_PL_CBuffer["cur_light_num"] = (int)(m_point_lights.size());
	m_PL_PSCbuf = std::make_unique<CachingPixelConstantBuffer>(gfx, m_PL_CBuffer, 0u);
	//TestCode: add two lights 
	for (size_t i = 0; i < 2; i++)
	{
		AddPointLight(gfx);
	}
	DISABLE_SIGNAL(lightAddedSignal);
}

void LightContainer::Bind(Graphics& gfx) noexcept
{
	for (size_t i = 0; i < m_point_lights.size(); i++)
	{
		m_point_lights[i]->Update();
		UpdateCBuffer(i);
	}
	int cur_light_num = (int)(m_point_lights.size());
	m_PL_CBuffer["cur_light_num"] = cur_light_num;
	m_PL_PSCbuf->SetBuffer(m_PL_CBuffer);
	m_PL_PSCbuf->Bind(gfx);
}

void LightContainer::Submit() noexcept
{
	for (auto& i: m_point_lights)
	{
		i->Submit();
	}
}

void LightContainer::LinkAddedLight(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG)
{
	m_point_lights.back()->LinkTechniques(rg);
}

void LightContainer::LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG)
{
	for (auto& i : m_point_lights)
	{
		i->LinkTechniques(rg);
	}
}

void LightContainer::AddPointLight(Graphics& gfx) noexcept(!IS_DEBUG)
{
	m_point_lights.emplace_back(std::make_unique<PointLight>(gfx, m_life_time_light_num));
	lightAddedSignal = true;
	m_life_time_light_num++;
}

void LightContainer::DeleteCurLight() noexcept
{
	m_point_lights.erase(m_point_lights.begin() + m_curPL_Index);
	m_curPL_Index == 0 ? m_curPL_Index = 0 : m_curPL_Index--;
	for (size_t i = 0; i < m_point_lights.size(); i++)
	{
		m_point_lights[i]->m_index = i;
	}
}

void LightContainer::SpwanControlWindow() noexcept(!IS_DEBUG)
{
	if (ImGui::Begin("Lights"))
	{
		ImGui::Text("Active Light");
		if (ImGui::BeginCombo("Current Light", m_point_lights[m_curPL_Index]->GetName().c_str()))
		{
			for (int n = 0; n < m_point_lights.size(); n++)
			{
				const bool isSelected = m_curPL_Index == n;
				if (ImGui::Selectable(m_point_lights[n]->GetName().c_str(), isSelected))
				{
					m_curPL_Index = n;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Add Light"))
		{
			AddPointLight(m_gfx);
		}
		if (m_point_lights.size() > 1)
		{
			if (ImGui::Button("Delete Current Light"))
			{
				DeleteCurLight();
			}
		}
		m_point_lights[m_curPL_Index]->SpwanControlWidgets();
		ImGui::End();
	}
}

void LightContainer::UpdateCBuffer(size_t index) noexcept(!IS_DEBUG)
{
	const auto worPos = DirectX::XMLoadFloat3(&m_point_lights[index]->m_pos);
	DirectX::XMFLOAT3 ViewPos;
	DirectX::XMStoreFloat3(&ViewPos, DirectX::XMVector3Transform(worPos, m_gfx.GetCamera()));
	m_PL_CBuffer["lightPositions"][index] = ViewPos;
	m_PL_CBuffer["lightAmbients"][index] = m_point_lights[index]->m_ambient;
	m_PL_CBuffer["lightDiffuses"][index] = m_point_lights[index]->m_diffuseColor;
	m_PL_CBuffer["lightIntensities"][index] = m_point_lights[index]->m_diffuseIntensity;
	m_PL_CBuffer["attConst"][index] = m_point_lights[index]->m_attConst;
	m_PL_CBuffer["attLinear"][index] = m_point_lights[index]->m_attLinear;
	m_PL_CBuffer["attQuad"][index] = m_point_lights[index]->m_attQuad;
	m_PL_CBuffer["Enable"][index] = m_point_lights[index]->m_Enable;
}

std::vector<std::string> LightContainer::m_ConstantBufferElements =
{
	"lightPositions",
	"lightAmbients",
	"lightDiffuses",
	"lightIntensities",
	"attConst",
	"attLinear",
	"attQuad",
	"Enable"
};