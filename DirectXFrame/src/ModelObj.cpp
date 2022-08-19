#include "ModelObj.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "imgui.h"
#include "Model.h"
ModelObj::ModelObj(Graphics& gfx, const Surface& Tex, const std::string& szObjPath, 
	const std::string& szModelName, const std::wstring& szPSPath, const std::wstring& szVSPath)
	:
	m_szObjName(szModelName),
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_Ambient(0.25f, 0.25f, 0.25f),
	m_PSCbuf(gfx, 1u)
{
	GenerateName();
	if (!IsStaticInitialized(m_szObjName))
	{
		InitStaticSlot(m_szObjName);
	}
	if (!IsSlotInitialized(m_szObjName))
	{
		Model obj(szObjPath);
		AddStaticBind(m_szObjName, std::make_unique<VertexBuffer>(gfx, obj.GetVertices()));
		AddStaticBind(m_szObjName, std::make_unique<Texture>(gfx, Tex));
		AddStaticBind(m_szObjName, std::make_unique<Sampler>(gfx));
		AddStaticIndexBuffer(m_szObjName, std::make_unique<IndexBuffer>(gfx, obj.GetIndices()));
		auto pvs = std::make_unique<VertexShader>(gfx, szVSPath);
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(m_szObjName, std::move(pvs));
		AddStaticBind(m_szObjName, std::make_unique<PixelShader>(gfx, szPSPath));
		AddStaticBind(m_szObjName, std::make_unique<InputLayout>(gfx, Model::GetVertexLayout(), pvsbc));
		AddStaticBind(m_szObjName, std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexfromStatic(m_szObjName);
	}
	Drawable::AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	m_uObjNum++;
}

ModelObj::ModelObj(Graphics& gfx, const std::string& szObjPath, const std::string& szModelName, 
	const std::wstring& szPSPath, const std::wstring& szVSPath)
	:
	m_szObjName(szModelName),
	m_pos(0.0f, 0.0f, 0.0f),
	m_rot(0.0f, 0.0f, 0.0f),
	m_Ambient(0.25f, 0.25f, 0.25f),
	m_PSCbuf(gfx, 1u)
{
	GenerateName();
	if (!IsStaticInitialized(m_szObjName))
	{
		InitStaticSlot(m_szObjName);
	}
	if (!IsSlotInitialized(m_szObjName))
	{
		Model obj(szObjPath);
		AddStaticBind(m_szObjName, std::make_unique<VertexBuffer>(gfx, obj.GetVertices()));
		AddStaticBind(m_szObjName, std::make_unique<Sampler>(gfx));
		AddStaticIndexBuffer(m_szObjName, std::make_unique<IndexBuffer>(gfx, obj.GetIndices()));
		auto pvs = std::make_unique<VertexShader>(gfx, szVSPath);
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(m_szObjName, std::move(pvs));
		AddStaticBind(m_szObjName, std::make_unique<PixelShader>(gfx, szPSPath));
		AddStaticBind(m_szObjName, std::make_unique<InputLayout>(gfx, Model::GetVertexLayout(), pvsbc));
		AddStaticBind(m_szObjName, std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexfromStatic(m_szObjName);
	}
	Drawable::AddBind(std::make_unique<TransformCbuf>(gfx, *this));
	m_uObjNum++;
}

void ModelObj::Update(Graphics& gfx) noexcept
{
	m_PSCbuf.Update(gfx, ModelCBuffer(m_Ambient));
	m_PSCbuf.Bind(gfx);
}

void ModelObj::SpwanControlWindow() noexcept
{
	std::string szTitle;
	std::stringstream ss2;
	ss2 << m_szObjName << "-" << m_uObjId;
	szTitle = ss2.str();

	if (ImGui::Begin(szTitle.c_str()))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &m_pos.x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &m_pos.y, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Z", &m_pos.z, -80.0f, 80.0f, "%.1f");
		ImGui::Text("Angle");
		ImGui::SliderAngle("AngleX", &m_rot.x, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleY", &m_rot.y, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("AngleZ", &m_rot.z, -180.0f, 180.0f, "%.1f");
		ImGui::Text("Ambient");
		ImGui::ColorEdit3("AmbientColor", &m_Ambient.x);
	}
	ImGui::End();
}

const std::string& ModelObj::GetModelName() const noexcept
{
	return m_szObjName;
}

DirectX::XMMATRIX ModelObj::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z) *
		DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void ModelObj::GenerateName() noexcept
{
	if (m_szObjName == "Object")
	{
		std::stringstream ss;
		ss << m_szObjName << m_uObjNum;
		m_szObjName = ss.str();
	}
	auto it = m_NameMarker.find(m_szObjName);
	if (it != m_NameMarker.end())
	{
		m_uObjId = (it->second)++;
		return;
	}
	m_NameMarker.emplace(m_szObjName, 1);
	it = m_NameMarker.find(m_szObjName);
	m_uObjId = (it->second)++;
}

UINT ModelObj::m_uObjNum = 1u;
std::map<std::string, UINT> ModelObj::m_NameMarker;