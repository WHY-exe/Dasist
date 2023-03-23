#include "Light.h"
#include "imgui.h"
PointLight::PointLight(Graphics& gfx)
	:
	m_pos(0.0f, 500.0f, 0.0f)
{
	Scene::ModelSetting op;
	op.szModelPath = "res\\model\\sphere.obj";
	op.szModelName = "PointLight";
	m_lightBall = Scene::Model(gfx, op);
	m_lightBall.Scale(0.3f);

	DCBuf::RawLayout cBufferLayout;
	cBufferLayout.Add<DCBuf::Float3>("LightPos");
	cBufferLayout.Add<DCBuf::Float3>("Ambient");
	cBufferLayout.Add<DCBuf::Float3>("DiffuseColor");
	cBufferLayout.Add<DCBuf::Float>("DiffuseIntensity");
	cBufferLayout.Add<DCBuf::Float>("attConst");
	cBufferLayout.Add<DCBuf::Float>("attLin");
	cBufferLayout.Add<DCBuf::Float>("attQuad");
	m_cBuffer = DCBuf::Buffer(std::move(cBufferLayout));

	m_cBuffer["LightPos"] = m_pos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
	m_cBuffer["attConst"] = m_attConst;
	m_cBuffer["attLin"] = m_attLinear;
	m_cBuffer["attQuad"] = m_attQuad;

	m_PSCbuf = std::make_unique<CachingPixelConstantBuffer>(gfx, m_cBuffer, 0u);
}

void PointLight::Update(Graphics& gfx, DirectX::FXMMATRIX viewTF) noexcept
{	
	const auto worPos = DirectX::XMLoadFloat3(&m_pos);
	DirectX::XMFLOAT3 ViewPos;
	DirectX::XMStoreFloat3(&ViewPos, DirectX::XMVector3Transform(worPos, viewTF));
	m_cBuffer["LightPos"] = ViewPos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
	m_cBuffer["attConst"] = m_attConst;
	m_cBuffer["attLin"] = m_attLinear;
	m_cBuffer["attQuad"] = m_attQuad;

	m_PSCbuf->SetBuffer(m_cBuffer);
	m_PSCbuf->Bind(gfx);
	m_lightBall.SetPos(m_pos);
	m_lightBall.ApplyTransformation();
}

void PointLight::Submit() noexcept
{
	m_lightBall.Submit();
}

void PointLight::LinkTechniques(Rgph::RenderGraph& rg) noexcept(!_DEBUG)
{
	m_lightBall.LinkTechniques(rg);
}

void PointLight::SpwanControlWindow() noexcept
{
	if (ImGui::Begin("Point light"))
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
	}
	ImGui::End();
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

	m_cBuffer = DCBuf::Buffer(std::move(cBufferLayout));

	m_cBuffer["LightPos"] = m_pos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
	m_PSCbuf = std::make_unique<CachingPixelConstantBuffer>(gfx, m_cBuffer, 1u);
}

void GlobalLight::Update(Graphics& gfx, DirectX::FXMMATRIX viewTF) noexcept
{
	
	const auto worPos = DirectX::XMLoadFloat3(&m_pos);
	DirectX::XMFLOAT3 ViewPos;
	DirectX::XMStoreFloat3(&ViewPos, DirectX::XMVector3Transform(worPos, viewTF));
	m_cBuffer["LightPos"] = ViewPos;
	m_cBuffer["Ambient"] = m_ambient;
	m_cBuffer["DiffuseColor"] = m_diffuseColor;
	m_cBuffer["DiffuseIntensity"] = m_diffuseIntensity;
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
	}
	ImGui::End();
}
