#include "Light.h"
#include "imgui.h"
PointLight::PointLight(Graphics& gfx, int index)
	:
	m_index(index),
	m_pos(0.0f, 500.0f, 0.0f),
	m_indicator(gfx),
	m_szName("PointLight" + std::to_string(index))
{
	m_indicator.SetScale(10.0f);
	DCBuf::RawLayout cBufferLayout;
	cBufferLayout.Add<DCBuf::Integer>("Index");
	m_PL_CBuffer = DCBuf::Buffer(std::move(cBufferLayout));
	m_PL_CBuffer["Index"] = index;
	m_PL_PSCbuf = std::make_unique<CachingPixelConstantBuffer>(gfx, m_PL_CBuffer, 4u);
}

const std::string& PointLight::GetName() const noexcept
{
	return m_szName;
}

void PointLight::Update(Graphics& gfx) noexcept
{	
	m_indicator.SetPos(m_pos);
}

void PointLight::Submit() noexcept
{
	if(m_Enable)
		m_indicator.Submit();
}

void PointLight::LinkTechniques(Rgph::RenderGraph& rg) noexcept(!IS_DEBUG)
{
	m_indicator.LinkTechniques(rg);
}

void PointLight::SpwanControlWidgets() noexcept
{
	ImGui::Text("Position");
	ImGui::SliderFloat("X", &m_pos.x, -2000.0f, 2000.0f, "%.1f");
	ImGui::SliderFloat("Y", &m_pos.y, -2000.0f, 2000.0f, "%.1f");
	ImGui::SliderFloat("Z", &m_pos.z, -2000.0f, 2000.0f, "%.1f");
	ImGui::ColorEdit3("DiffuseColor", &m_diffuseColor.x);
	ImGui::ColorEdit3("AmbientColor", &m_ambient.x);
	ImGui::Text("LightIntensity");
	ImGui::SliderFloat("Intensity", &m_diffuseIntensity, 0.0f, 10.0f, "%.2f");
	ImGui::Text("Attenuation");
	ImGui::SliderFloat("AttConst", &m_attConst, 0.0f, 0.1f, "%.5f");
	ImGui::SliderFloat("AttLinear", &m_attLinear, 0.0f, 0.1f, "%.5f");
	ImGui::SliderFloat("AttQuad", &m_attQuad, 0.0f, 0.1f, "%.5f");
	ImGui::Checkbox("Enable", reinterpret_cast<bool*>(&m_Enable));
}

GlobalLight::GlobalLight(Graphics& gfx)
	:
	m_pos(0.0f, 1000.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f)
{
	DCBuf::RawLayout cBufferLayout;
	cBufferLayout.Add<DCBuf::Float3>("LightPos");
	cBufferLayout.Add<DCBuf::Float3>("Ambient");
	cBufferLayout.Add<DCBuf::Float3>("DiffuseColor");
	cBufferLayout.Add<DCBuf::Float>("DiffuseIntensity");
	cBufferLayout.Add<DCBuf::Bool>("Enable");
	m_cBuffer = DCBuf::Buffer(std::move(cBufferLayout));

	m_cBuffer["LightPos"] = m_pos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
	m_cBuffer["Enable"] = m_Enable;
	m_PSCbuf = std::make_unique<CachingPixelConstantBuffer>(gfx, m_cBuffer, 1u);
}

void GlobalLight::Update(Graphics& gfx) noexcept
{
	
	const auto worPos = DirectX::XMLoadFloat3(&m_pos);
	DirectX::XMFLOAT3 ViewPos;
	DirectX::XMStoreFloat3(&ViewPos, DirectX::XMVector3Transform(worPos, gfx.GetCamera()));
	m_cBuffer["LightPos"] = ViewPos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
	m_cBuffer["Enable"] = m_Enable;
	m_PSCbuf->SetBuffer(m_cBuffer);
	m_PSCbuf->Bind(gfx);
}

void GlobalLight::SpwanControlWindow() noexcept
{
	if (ImGui::Begin("GlobalLight"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -2000.0f, 2000.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -2000.0f, 2000.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -2000.0f, 2000.0f, "%.1f");
		ImGui::ColorEdit3("DiffuseColor", &m_diffuseColor.x);
		ImGui::ColorEdit3("AmbientColor", &m_ambient.x);
		ImGui::Text("LightIntensity");
		ImGui::SliderFloat("Intensity", &m_diffuseIntensity, 0.0f, 10.0f, "%.2f");
		ImGui::Checkbox("Enable", reinterpret_cast<bool*>(&m_Enable));
	}
	ImGui::End();
}
